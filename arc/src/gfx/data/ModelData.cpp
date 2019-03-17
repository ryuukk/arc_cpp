#include <fstream>
#include <glad/glad.h>
#include "ModelData.h"
#include "../../utils/json11.hpp"
#include "../../utils/DirUtils.h"
#include "../../utils/FileUtils.h"

arc::ModelData arc::ModelData::load(const std::string& path) {

    std::string file_contents = arc::readFile(path);

    std::string error;
    auto json = json11::Json::parse(file_contents, error, json11::JsonParse::COMMENTS);

    ModelData data;

    int lo = json["version"].array_items()[0].int_value();
    int hi = json["version"].array_items()[1].int_value();

    std::string id = json["id"].string_value();
    data.id = id.length() > 0 ? id : path;

    printf("Loading: %s version: %d:%d id:%s\n", path.c_str(), lo, hi, id.c_str());

    parseMeshes(data, json);
    parseNodes(data, json);
    parseMaterials(data, json, arc::dirName(path));

    return data;
}

void arc::ModelData::parseMeshes(arc::ModelData& data, json11::Json& json) {

    auto meshes = json["meshes"].array_items();

    data.meshes.resize(meshes.size());
    for (int i = 0; i < meshes.size(); ++i) {
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

    for (int i = 0; i < array.size(); ++i) {
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

    for (int i = 0; i < array.size(); ++i) {
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
        for (int i = 0; i < parts.size(); ++i) {
            auto material = parts[i];
            ModelNodePart nodePart;

            nodePart.meshPartId = material["meshpartid"].string_value();
            nodePart.materialId = material["materialid"].string_value();

            if (!material["bones"].is_null()) {
                auto bones = material["bones"].array_items();
                nodePart.bones.resize(bones.size());
                for (int j = 0; j < bones.size(); ++j) {
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
        for (int i = 0; i < children.size(); ++i) {
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

        for (int i = 0; i < array.size(); ++i) {
            auto material = array[i];
            auto jsonMaterial = ModelMaterial();
            jsonMaterial.id = material["id"].string_value();

            data.materials[i] = jsonMaterial;
        }
    }
}

void arc::ModelData::parseAnimations(arc::ModelData& data, json11::Json& json) {
    // todo: finish

}

int arc::ModelData::parseTextureUsage(std::string& type) {
    // todo: finish
    return 0;
}

int arc::ModelData::parseType(std::string& type) {

    if (type == "TRIANGLES") return GL_TRIANGLES;

    throw std::invalid_argument("Unsupported type %s"); // todo: figure out exceptions
}

void arc::ModelData::parseIndices(ModelMeshPart& data, json11::Json& json) {
    auto array = json.array_items();
    data.indices.resize(array.size());
    for (int i = 0; i < array.size(); ++i) {
        data.indices[i] = (short) array[i].int_value();
    }
}

void arc::ModelData::parseVertices(ModelMesh& data, json11::Json& json) {
    auto array = json.array_items();
    data.vertices.resize(array.size());
    for (int i = 0; i < array.size(); ++i) {
        data.vertices[i] = (float) array[i].number_value();
    }
}

void arc::ModelData::parseAttributes(ModelMesh& data, json11::Json& json) {
    int unit = 0;
    int blendWeightCount = 0;
    auto array = json.array_items();

    data.attributes.resize(array.size());
    for (int i = 0; i < array.size(); ++i) {
        auto attribute = array[i].string_value();

        if (attribute.rfind("TEXCOORD", 0) == 0)
            data.attributes[i] = VertexAttribute::texCoords(++unit);
        else if (attribute.rfind("BLENDWEIGHT", 0) == 0)
            data.attributes[i] = VertexAttribute::boneWeight(++blendWeightCount);
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
