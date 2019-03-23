#include <fstream>
#include <glad/glad.h>
#include "ModelData.h"
#include "../../utils/json11.hpp"
#include "../../utils/DirUtils.h"
#include "../../utils/FileUtils.h"
#include "../../Core.h"

arc::ModelData arc::ModelData::load(const std::string& path) {

    std::string file_contents = arc::file::readFile(path);

    std::string error;
    auto json = json11::Json::parse(file_contents, error, json11::JsonParse::COMMENTS);

    ModelData data;

	auto version = json["version"].array_items();
    int lo = version[0].int_value();
    int hi = version[1].int_value();

    std::string id = json["id"].string_value();
    data.id = id.length() > 0 ? id : path;

    Core::logger->info("Loading Model: {} version: {}:{} id: {}", path.c_str(), lo, hi, data.id);

    parseMeshes(data, json);
    parseMaterials(data, json, arc::dirName(path));
    parseNodes(data, json);
    parseAnimations(data, json);

    return data;
}

void arc::ModelData::parseMeshes(arc::ModelData& data, json11::Json& json) {

    auto meshes = json["meshes"].array_items();

    data.meshes.resize(meshes.size());
    for (auto i = 0; i < meshes.size(); ++i) {
        auto mesh = meshes[i];
        ModelMesh jsonMesh;

        jsonMesh.id = mesh["id"].string_value();

        auto attributes = mesh["attributes"];
        auto vertices = mesh["vertices"];
        auto parts = mesh["parts"];

        parseAttributes(jsonMesh, attributes);
        parseVertices(jsonMesh, vertices);
        parseMeshParts(jsonMesh, parts);

        data.meshes[i] = jsonMesh;
    }
}

void arc::ModelData::parseMeshParts(ModelMesh& data, json11::Json& json) {

    auto array = json.array_items();
    data.parts.resize(array.size());

    for (auto i = 0; i < array.size(); ++i) {
        auto meshPart = array[i];
        ModelMeshPart jsonpart;
        jsonpart.id = meshPart["id"].string_value();

        auto type = meshPart["type"].string_value();
        jsonpart.primitiveType = parseType(type);

        auto indices = meshPart["indices"];
        parseIndices(jsonpart, indices);
        data.parts[i] = jsonpart;
    }
}

void arc::ModelData::parseNodes(arc::ModelData& data, json11::Json& json) {
    auto array = json["nodes"].array_items();
    data.nodes.resize(array.size());

    for (auto i = 0; i < array.size(); ++i) {
        data.nodes[i] = parseNodesRecursively(array[i]);
    }
}

arc::ModelNode arc::ModelData::parseNodesRecursively(json11::Json& json) {
    ModelNode jsonNode;
    jsonNode.id = json["id"].string_value();

    jsonNode.translation = json["translation"].is_null() ? Vec3{0, 0, 0} : parseVec3(json["translation"]);
    jsonNode.scale = json["scale"].is_null() ? Vec3{1, 1, 1} : parseVec3(json["scale"]);
    jsonNode.rotation = json["rotation"].is_null() ? Quat::identity() : parseQuat(json["rotation"]);

    jsonNode.meshId = json["mesh"].string_value();

    if (!json["parts"].is_null()) {
        auto parts = json["parts"].array_items();
        jsonNode.parts.resize(parts.size());
        for (auto i = 0; i < parts.size(); ++i) {
            auto material = parts[i];
            ModelNodePart nodePart;

            nodePart.meshPartId = material["meshpartid"].string_value();
            nodePart.materialId = material["materialid"].string_value();

            if (!material["bones"].is_null()) {
                auto bones = material["bones"].array_items();
                nodePart.bones.resize(bones.size());
                for (auto j = 0; j < bones.size(); ++j) {
                    auto bone = bones[j];
                    std::string nodeId = bone["node"].string_value();

                    auto transform = Mat4::identity();

                    auto translation = parseVec3(bone["translation"]);
                    auto rotation = parseQuat(bone["rotation"]);
                    auto scale = parseVec3(bone["scale"]);

                    transform.set(translation, rotation, scale);

                    nodePart.bones[j] = {nodeId, transform};
                    //nodePart->bones[j] = std::pair(nodeId, transform);
                    //nodePart->bones.emplace_back(std::pair(nodeId, transform));
                }
            }

            jsonNode.parts[i] = nodePart;
        }
    }

    if (!json["children"].is_null()) {
        auto children = json["children"].array_items();
        jsonNode.children.resize(children.size());
        for (auto i = 0; i < children.size(); ++i) {
            jsonNode.children[i] = parseNodesRecursively(children[i]);
        }
    }

    return jsonNode;
}

void arc::ModelData::parseMaterials(arc::ModelData& data, json11::Json& json, const std::string& matPath) {
    // todo: finish
    if(json["materials"].is_null())
    {
        // todo: create default material
    } else {

        auto array = json["materials"].array_items();
        data.materials.resize(array.size());

        for (auto i = 0; i < array.size(); ++i) {
            auto material = array[i];
            auto jsonMaterial = ModelMaterial();
            jsonMaterial.id = material["id"].string_value();

            if(!material["textures"].is_null())
            {
                auto textures = material["textures"].array_items();
                jsonMaterial.textures.resize(textures.size());

                for (int j = 0; j < textures.size(); ++j) {
                    auto texture = textures[j];
                    auto jsonTexture = ModelTexture();

                    jsonTexture.id = texture["id"].string_value();

                    jsonTexture.uvTranslation = {0,0};
                    jsonTexture.uvScaling = {1,1};

                    jsonTexture.usage = parseTextureUsage(texture["type"].string_value());

                    auto fileName = texture["filename"].string_value();
                    auto filePath = matPath;

                    jsonTexture.fileName = matPath.empty() ? filePath.append(fileName) : filePath.append("/").append(fileName);

                    if(!arc::file::exists(jsonTexture.fileName))
                    {
                        Core::logger->error("Model: {} Texture not found in disk: {}", data.id, jsonTexture.fileName); // todo: maybe load default texture ?
                    }

                    jsonMaterial.textures[j] = jsonTexture;
                }
            }

            data.materials[i] = jsonMaterial;
        }
    }
}

void arc::ModelData::parseAnimations(arc::ModelData& data, json11::Json& json) {
    // todo: finish
    if(json["animations"].is_null())
    {
    } else {
        auto array = json["animations"].array_items();

        for (auto i = 0; i < array.size(); ++i) {
            auto anim = array[i];

            auto nodes = anim["bones"];
            if(nodes.is_null()) continue;
            auto nodesArray = nodes.array_items();

            auto animation = ModelAnimation();
            animation.id = anim["id"].string_value();

            for (auto j = 0; j < nodesArray.size(); ++j) {
                auto node = nodesArray[j];
                auto nodeAnim = ModelNodeAnimation();
                nodeAnim.nodeId = node["boneId"].string_value();

                // v0.1
                auto keyframes = node["keyframes"];
                if(!keyframes.is_null())
                {
                    auto keyframesArray = keyframes.array_items();
                    for (auto k = 0; k < keyframesArray.size(); ++k) {
                        auto keyframe = keyframesArray[k];

                        float keytime = keyframe["keytime"].number_value() / 1000.0;

                        if(!keyframe["translation"].is_null())
                        {
                            nodeAnim.translation.emplace_back(ModelNodeKeyframe<Vec3>{keytime, parseVec3(keyframe["translation"])});
                        }
                        if(!keyframe["rotation"].is_null())
                        {
                            nodeAnim.rotation.emplace_back(ModelNodeKeyframe<Quat>{keytime, parseQuat(keyframe["rotation"])});
                        }
                        if(!keyframe["scale"].is_null())
                        {
                            nodeAnim.scaling.emplace_back(ModelNodeKeyframe<Vec3>{keytime, parseVec3(keyframe["scale"])});
                        }
                    }
                }

                animation.nodeAnimations.emplace_back(nodeAnim);
            }
            data.animations.emplace_back(animation);
        }
    }
}

int arc::ModelData::parseTextureUsage(const std::string& type) {
    if (type == "AMBIENT")
        return ModelTexture::USAGE_AMBIENT;
    else if (type == "BUMP")
        return ModelTexture::USAGE_BUMP;
    else if (type == "DIFFUSE")
        return ModelTexture::USAGE_DIFFUSE;
    else if (type == "EMISSIVE")
        return ModelTexture::USAGE_EMISSIVE;
    else if (type == "NONE")
        return ModelTexture::USAGE_NONE;
    else if (type == "NORMAL")
        return ModelTexture::USAGE_NORMAL;
    else if (type == "REFLECTION")
        return ModelTexture::USAGE_REFLECTION;
    else if (type == "SHININESS")
        return ModelTexture::USAGE_SHININESS;
    else if (type == "SPECULAR")
        return ModelTexture::USAGE_SPECULAR;
    else if (type == "TRANSPARENCY") return ModelTexture::USAGE_TRANSPARENCY;
    return ModelTexture::USAGE_UNKNOWN;
}

int arc::ModelData::parseType(const std::string& type) {

    if (type == "TRIANGLES") return GL_TRIANGLES;

    throw std::invalid_argument("Unsupported type %s"); // todo: figure out exceptions
}

void arc::ModelData::parseIndices(ModelMeshPart& data, json11::Json& json) {
    auto array = json.array_items();
    data.indices.resize(array.size());
    for (auto i = 0; i < array.size(); ++i) {
        data.indices[i] = (short) array[i].int_value();
    }
}

void arc::ModelData::parseVertices(ModelMesh& data, json11::Json& json) {
    auto array = json.array_items();
    data.vertices.resize(array.size());
    for (auto i = 0; i < array.size(); ++i) {
        data.vertices[i] = (float) array[i].number_value();
    }
}

void arc::ModelData::parseAttributes(ModelMesh& data, json11::Json& json) {
    int unit = 0;
    int blendWeightCount = 0;
    auto array = json.array_items();

    data.attributes.resize(array.size());
    for (auto i = 0; i < array.size(); ++i) {
        auto attribute = array[i].string_value();

        if (attribute.rfind("TEXCOORD", 0) == 0)
        {
            int u = unit;
            unit++;
            data.attributes[i] = VertexAttribute::texCoords(u);
        }
        else if (attribute.rfind("BLENDWEIGHT", 0) == 0)
        {
            int u = blendWeightCount;
            blendWeightCount++;
            data.attributes[i] = VertexAttribute::boneWeight(u);
        }
        else if (attribute == "POSITION")
            data.attributes[i] = VertexAttribute::position();
        else if (attribute == "NORMAL")
            data.attributes[i] = VertexAttribute::normal();
        else if (attribute == "COLOR")
            data.attributes[i] = VertexAttribute::colorUnpacked();
        else if (attribute == "COLORPACKED")
            data.attributes[i] = VertexAttribute::colorPacked();
        else if (attribute == "TANGENT")
            data.attributes[i] = VertexAttribute::tangent();
        else if (attribute == "BINORMAL")
            data.attributes[i] = VertexAttribute::binormal();
        else
            throw std::invalid_argument(attribute); // todo: figure out exceptions

    }
}

arc::Vec2 arc::ModelData::parseVec2(const json11::Json& json) {
    auto array = json.array_items();
    return {(float) array[0].number_value(), (float) array[1].number_value()};
}

arc::Vec3 arc::ModelData::parseVec3(const json11::Json& json) {
    auto array = json.array_items();
    return {(float) array[0].number_value(), (float) array[1].number_value(), (float) array[2].number_value()};
}

arc::Quat arc::ModelData::parseQuat(const json11::Json& json) {
    auto array = json.array_items();
    return {(float) array[0].number_value(), (float) array[1].number_value(), (float) array[2].number_value(),
            (float) array[3].number_value()};
}
