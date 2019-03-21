#pragma once

#include <string>
#include "../../utils/json11.hpp"
#include "ModelMesh.h"
#include "ModelNode.h"
#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "../../math/Quat.h"
#include "../../math/Mat4.h"
#include "ModelMaterial.h"
#include "ModelAnimation.h"

namespace arc
{
    class ModelData
    {
    public:
        std::string id;
        std::vector<ModelMesh> meshes;
        std::vector<ModelMaterial> materials;
        std::vector<ModelNode> nodes;
        std::vector<ModelAnimation> animations;

        static ModelData load(const std::string& path);

        static void parseMeshes(ModelData& data, json11::Json& json);

        static void parseMeshParts(ModelMesh& data, json11::Json& json);

        static void parseNodes(ModelData& data, json11::Json& json);

        static ModelNode parseNodesRecursively(json11::Json& json);

        static void parseMaterials(ModelData& data, json11::Json& json, const std::string& matPath);

        static void parseAnimations(ModelData& data, json11::Json& json);

        static int parseTextureUsage(const std::string& type);

        static int parseType(const std::string& type);

        static void parseIndices(ModelMeshPart& data, json11::Json& json);

        static void parseVertices(ModelMesh& data, json11::Json& json);

        static void parseAttributes(ModelMesh& data, json11::Json& json);

        static Vec2 parseVec2(const json11::Json& json);

        static Vec3 parseVec3(const json11::Json& json);

        static Quat parseQuat(const json11::Json& json);

    };

}


