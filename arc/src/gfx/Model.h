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
        std::vector<Material> materials;
        std::vector<Node> nodes;
        std::vector<Animation> animations;
        std::vector<Mesh*> meshes;
        std::vector<MeshPart*> meshParts;

        Model(ModelData& data);
        void calculateTransforms();
    private:
        void loadMeshes(std::vector<ModelMesh>& meshes);
        void convertMesh(ModelMesh& modelMesh);
        void loadNodes(std::vector<ModelNode>& nodes);
        Node loadNode(ModelNode& modelNode);
    };
}


