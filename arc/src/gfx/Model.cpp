#include "Model.h"

arc::Model::Model(ModelData& data) {
    id = data.id;
    loadMeshes(data.meshes);
}

void arc::Model::calculateTransforms() {

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

    VertexAttributes attributes = VertexAttributes(modelMesh.attributes);
    int numVertices = (int) modelMesh.vertices.size() / (attributes.vertexSize / 4);

    Mesh mesh = Mesh(true, numVertices, numIndices, attributes);
    meshes.emplace_back(mesh);

    mesh.setVertices(modelMesh.vertices);

    std::vector<short> indices;
    int offset = 0;

    meshParts.resize(modelMesh.parts.size());
    for (int i = 0; i < modelMesh.parts.size(); ++i) {
        ModelMeshPart part = modelMesh.parts[i];

        MeshPart meshPart;
        meshPart.id = part.id;
        meshPart.primitiveType = part.primitiveType;
        meshPart.offset = offset;
        meshPart.size = hasIndices ? part.indices.size() : numVertices;
        meshPart.mesh = &mesh;
        if(hasIndices)
            indices.insert(std::end(indices), std::begin(part.indices), std::end(part.indices));


        offset += meshPart.size;

        meshParts[i] = meshPart;
    }

    mesh.setIndices(indices);
    for(auto& part : meshParts)
    {
        // todo: update bounding box
    }
}

void arc::Model::loadNodes(std::vector<arc::ModelNode>& nodes) {

}

arc::Node arc::Model::loadNode(arc::ModelNode& modelNode) {
    return arc::Node();
}
