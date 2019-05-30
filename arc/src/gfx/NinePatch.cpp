#include "NinePatch.h"

void arc::NinePatch::draw(SpriteBatch* batch, float x, float y, float width, float height) {
    prepareVertices(batch, x, y, width, height);
    batch->draw(_texture, *vertices, 0, idx);
}

void arc::NinePatch::draw(SpriteBatch* batch, float x, float y, float originX, float originY, float width, float height,
                          float scaleX, float scaleY, float rotation) {
    prepareVertices(batch, x, y, width, height);
    //float worldOriginX = x + originX, worldOriginY = y + originY;
    //int n = this.idx;
    //if (rotation != 0) {
    //    for (int i = 0; i < n; i += 5) {
    //        float vx = (vertices[i] - worldOriginX) * scaleX, vy = (vertices[i + 1] - worldOriginY) * scaleY;
    //        float cos = MathUtils.cosDeg(rotation), sin = MathUtils.sinDeg(rotation);
    //        vertices[i] = cos * vx - sin * vy + worldOriginX;
    //        vertices[i + 1] = sin * vx + cos * vy + worldOriginY;
    //    }
    //} else if (scaleX != 1 || scaleY != 1) {
    //    for (int i = 0; i < n; i += 5) {
    //        vertices[i] = (vertices[i] - worldOriginX) * scaleX + worldOriginX;
    //        vertices[i + 1] = (vertices[i + 1] - worldOriginY) * scaleY + worldOriginY;
    //    }
    //}
    //batch.draw(texture, vertices, 0, n);
}

int arc::NinePatch::add(TextureRegion* region, float color, bool isStretchW, bool isStretchH) {
    return 0;
}

void arc::NinePatch::set(int idx, float x, float y, float width, float height, float color) {

}

void arc::NinePatch::prepareVertices(SpriteBatch* batch, float x, float y, float width, float height) {

}

void arc::NinePatch::setPadding(float left, float right, float top, float bottom) {

}

arc::NinePatch::NinePatch(arc::TextureRegion* region, int left, int right, int top, int bottom) {
   int middleWidth = region->regionWidth - left - right;
   int middleHeight = region->regionHeight - top - bottom;
   // todo: finish
}
