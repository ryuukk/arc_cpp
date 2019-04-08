#pragma once

#include <string>
#include "nodes/Node.h"
#include "data/ModelData.h"
#include "Material.h"
#include "Animation.h"

namespace arc
{
    class Model
    {
    public:
        std::string id;
        std::vector<Material*> materials;
        std::vector<Node*> nodes;
        std::vector<Animation*> animations;
        std::vector<Mesh*> meshes;
        std::vector<MeshPart*> meshParts;

        Model(ModelData& data);
        ~Model()
        {
            for (auto* mat : materials)
                delete mat;
            for (auto* node : nodes)
                delete node;
            for (auto* anim : animations)
                delete anim;
            for (auto* mesh : meshes)
                delete mesh;
            for (auto* part : meshParts)
                delete part;
        }
        void calculateTransforms();
    private:
        void loadMeshes(std::vector<ModelMesh>& modelMeshes);
        void convertMesh(ModelMesh& modelMesh);
        void loadNodes(std::vector<ModelNode>& modelNodes);
        void loadAnimations(std::vector<ModelAnimation>& modelAnimations);
        void loadMaterials(std::vector<ModelMaterial>& modelMaterials);
        Node* loadNode(ModelNode& modelNode);

        std::unordered_map<NodePart*, std::vector<std::pair<std::string, Mat4>>> nodePartBones;
    };
}


