#include "Cube.h"

void arc::Cube::getRenderables(DynamicObjectPool<Renderable>& pool, std::vector<Renderable*>& renderables) {

    auto renderable = pool.new_object();
    renderable->meshPart.set("cube", &_mesh, 0, 9, GL_TRIANGLES);
    renderable->material = &_material;
    renderable->worldTransform = Mat4::identity();
    renderables.emplace_back(renderable);
}

arc::Cube::Cube(arc::Color color) :
        _mesh(true, 24, 36,
              new VertexAttributes(
                      {
                              VertexAttribute(VertexUsage::Position, 3, "a_position"),
                              VertexAttribute(VertexUsage::Normal, 3, "a_normal")
                      })),
                      _material(new DiffuseColorAttribute(color)){

}
