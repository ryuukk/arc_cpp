#include "Model.h"

arc::Model::Model(ModelData& data) {
    id = data.id;
    loadMeshes(data.meshes);
    loadMaterials(data.materials);
    loadNodes(data.nodes);
    loadAnimations(data.animations);
    calculateTransforms();
}

void arc::Model::calculateTransforms() {
    auto n = nodes.size();
    for (int i = 0; i < n; ++i) {
        nodes[i]->calculateTransforms(true);
    }
    for (int i = 0; i < n; ++i) {
        nodes[i]->calculateBoneTransforms(true);
    }
}

void arc::Model::loadMeshes(std::vector<arc::ModelMesh>& meshes) {
    for(auto& mesh : meshes)
        convertMesh(mesh);
}

void arc::Model::convertMesh(arc::ModelMesh& modelMesh) {
    int numIndices = 0;
    for(auto& part : modelMesh.parts)
        numIndices += part.indices.size();

    bool hasIndices = numIndices > 0;

    auto* attributes = new VertexAttributes(modelMesh.attributes);
    int numVertices = (int) modelMesh.vertices.size() / (attributes->vertexSize / 4);

    auto* mesh = new Mesh(true, numVertices, numIndices, attributes);
    meshes.emplace_back(mesh);

    mesh->setVertices(modelMesh.vertices);

    std::vector<short> indices;
    int offset = 0;

    meshParts.resize(modelMesh.parts.size());
    for (int i = 0; i < modelMesh.parts.size(); ++i) {
        auto& part = modelMesh.parts[i];

        auto* meshPart = new MeshPart;
        meshPart->id = part.id;
        meshPart->primitiveType = part.primitiveType;
        meshPart->offset = offset;
        meshPart->size = hasIndices ? part.indices.size() : numVertices;
        meshPart->mesh = mesh;
        if(hasIndices)
            indices.insert(std::end(indices), std::begin(part.indices), std::end(part.indices));


        offset += meshPart->size;

        meshParts[i] = meshPart;
    }

    mesh->setIndices(indices);
    for(auto& part : meshParts)
    {
        // todo: update bounding box
    }
}

void arc::Model::loadNodes(std::vector<arc::ModelNode>& nodes) {
    nodePartBones.clear();

    for(auto& node : nodes)
    {
        Node* n = loadNode(node);
        this->nodes.emplace_back(n);
    }

    for(auto& e : nodePartBones)
    {
        e.first->invBoneTransforms.clear();

        for (int i = 0; i < e.second.size(); ++i) {
            auto& pair = e.second[i];
            auto* node = Node::getNode(this->nodes, pair.first);
            auto invTransform = Mat4::inv(pair.second);

            e.first->invBoneTransforms.emplace_back(std::pair(node, invTransform));
        }
    }
}

arc::Node* arc::Model::loadNode(arc::ModelNode& modelNode) {
    auto* node = new Node;
    node->id = modelNode.id;
    node->translation = modelNode.translation;
    node->rotation = modelNode.rotation;
    node->scale = modelNode.scale;

    for(auto& modelNodePart : modelNode.parts)
    {
        MeshPart* meshPart = nullptr;
        Material* meshMaterial = nullptr;

        if(!modelNodePart.meshPartId.empty())
        {
            for(auto* part : meshParts)
            {
                if(modelNodePart.meshPartId == part->id)
                {
                    meshPart = part;
                    break;
                }
            }
        }

        if(!modelNodePart.materialId.empty())
        {
            for(auto* material : materials)
            {
                if(modelNodePart.materialId == material->id)
                {
                    meshMaterial = material;
                    break;
                }
            }
        }

        if(meshPart == nullptr || meshMaterial == nullptr)
        {
            throw std::invalid_argument("invalid node: %s"); // todo: figure out exceptions
        }

        auto* nodePart = new NodePart;
        nodePart->meshPart = meshPart;
        nodePart->material = meshMaterial;
        node->parts.emplace_back(nodePart);
        if(!modelNodePart.bones.empty())
            nodePartBones[nodePart] = modelNodePart.bones;
    }

    if(!modelNode.children.empty())
    {
        for(auto& child : modelNode.children)
        {
            Node* c = loadNode(child);
            c->parent = node;
            node->addChild(c);
        }
    }

    return node;
}

void arc::Model::loadAnimations(std::vector<arc::ModelAnimation>& animations)
{
    for(auto& anim : animations)
    {
        auto* animation = new Animation;
        animation->id = anim.id;
        for(auto& nanim : anim.nodeAnimations)
        {
            auto* node = Node::getNode(nodes, nanim.nodeId);
            if(node == nullptr)
                continue;

            auto* nodeAnim = new NodeAnimation;
            nodeAnim->node = node;

            // todo: should figure out why i can't resize the vectors.. i should avoid emplace_back if i already know the final size
            if(!nanim.translation.empty())
            {
                //nodeAnim->translation.resize(nanim.translation.size());
                for (int i = 0; i < nanim.translation.size(); ++i) {
                    auto& kf = nanim.translation[i];
                    if(kf.keytime > animation->duration) animation->duration = kf.keytime;
                    auto kff = NodeKeyframe<Vec3>{kf.keytime, kf.value};
                    nodeAnim->translation.emplace_back(kff);
                }
            }
            if(!nanim.rotation.empty())
            {
                //nodeAnim->rotation.resize(nanim.rotation.size());
                for (int i = 0; i < nanim.rotation.size(); ++i) {
                    auto& kf = nanim.rotation[i];
                    if(kf.keytime > animation->duration) animation->duration = kf.keytime;
                    auto kff = NodeKeyframe<Quat>{kf.keytime, kf.value};
                    nodeAnim->rotation.emplace_back(kff);
                }
            }
            if(!nanim.scaling.empty())
            {
                //nodeAnim->scaling.resize(nanim.scaling.size());
                for (int i = 0; i < nanim.scaling.size(); ++i) {
                    auto& kf = nanim.scaling[i];
                    if(kf.keytime > animation->duration) animation->duration = kf.keytime;
                    auto kff = NodeKeyframe<Vec3>{kf.keytime, kf.value};
                    nodeAnim->scaling.emplace_back(kff);
                }
            }

            if ((!nodeAnim->translation.empty()) || (!nodeAnim->rotation.empty()) || (!nodeAnim->scaling.empty()))
                animation->nodeAnimations.emplace_back(nodeAnim);
        }
        if(!animation->nodeAnimations.empty())
            this->animations.emplace_back(animation);
    }
}

void arc::Model::loadMaterials(std::vector<arc::ModelMaterial>& materials) {
    for(auto& modelMaterial : materials)
    {
        auto* material = new Material;
        material->id = modelMaterial.id;

        if(!modelMaterial.textures.empty())
        {
            std::unordered_map<std::string, Texture2D*> tmp{};

            materials.resize(modelMaterial.textures.size());
            for (int i = 0; i < modelMaterial.textures.size(); ++i) {
                auto& tex = modelMaterial.textures[i];
                Texture2D* texture = nullptr;

                if(tmp.find(tex.fileName) != tmp.end())
                    texture = tmp[tex.fileName];
                else
                {
                    texture = Texture2D::loadFromFile(tex.fileName);
                    tmp[tex.fileName] = texture;
                }
                switch (tex.usage)
                {
                    case ModelTexture::USAGE_DIFFUSE:
                        material->set(new DiffuseTextureAttribute(texture));
                        break;
                    //case ModelTexture::USAGE_SPECULAR:
                    //    material->set(new TextureAttribute(TextureAttribute.Specular, descriptor, offsetU, offsetV, scaleU, scaleV));
                    //    break;
                    //case ModelTexture::USAGE_BUMP:
                    //    material->set(new TextureAttribute(TextureAttribute.Bump, descriptor, offsetU, offsetV, scaleU, scaleV));
                    //    break;
                    //case ModelTexture::USAGE_NORMAL:
                    //    material->set(new TextureAttribute(TextureAttribute.Normal, descriptor, offsetU, offsetV, scaleU, scaleV));
                    //    break;
                    //case ModelTexture::USAGE_AMBIENT:
                    //    material->set(new TextureAttribute(TextureAttribute.Ambient, descriptor, offsetU, offsetV, scaleU, scaleV));
                    //    break;
                    //case ModelTexture::USAGE_EMISSIVE:
                    //    material->set(new TextureAttribute(TextureAttribute.Emissive, descriptor, offsetU, offsetV, scaleU, scaleV));
                    //    break;
                    //case ModelTexture::USAGE_REFLECTION:
                    //    material->set(new TextureAttribute(TextureAttribute.Reflection, descriptor, offsetU, offsetV, scaleU, scaleV));
                    //    break;
                    default:
                        throw std::invalid_argument("Texture usage type: %d not yet supported"); // todo: figure out exceptions
                }

            }
        }


        this->materials.emplace_back(material);
    }
}
