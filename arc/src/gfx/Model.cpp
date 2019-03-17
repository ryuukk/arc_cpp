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
    for(auto& node : nodes)
    {
        Node* n = loadNode(node);
        this->nodes.emplace_back(n);
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
                    printf("Found the MeshPart!\n");
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
                    printf("Found the Material!\n");
                    meshMaterial = material;
                    break;
                }
            }
        }

        if(meshPart == nullptr || meshMaterial == nullptr)
        {
            printf("Node: %s MeshPart: %s MeshMaterial: %s\n", node->id.c_str(), modelNodePart.meshPartId.c_str(), modelNodePart.materialId.c_str());
            printf("Mesh: %s\n", meshPart == nullptr ? "false":"true");
            printf("Material: %s\n", meshMaterial == nullptr ? "false":"true");
            throw std::invalid_argument("invalid node: %s"); // todo: figure out exceptions
        }

        auto* nodePart = new NodePart;
        nodePart->meshPart = meshPart;
        nodePart->material = meshMaterial;
        node->parts.emplace_back(nodePart);
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

}

void arc::Model::loadMaterials(std::vector<arc::ModelMaterial>& materials) {
    for(auto& modelMaterial : materials)
    {
        auto* material = new Material;
        material->id = modelMaterial.id;

        printf("Material: %s\n", material->id.c_str());

        this->materials.emplace_back(material);
    }
}
