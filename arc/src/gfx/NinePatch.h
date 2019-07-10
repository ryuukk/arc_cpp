#pragma once

#include <vector>
#include "SpriteBatch.h"
#include "TextureRegion.h"


namespace arc
{
    namespace ninepatch
    {
       static const int TOP_LEFT = 0;
       static const int TOP_CENTER = 1;
       static const int TOP_RIGHT = 2;
       static const int MIDDLE_LEFT = 3;
       static const int MIDDLE_CENTER = 4;
       static const int MIDDLE_RIGHT = 5;
       static const int BOTTOM_LEFT = 6;
       static const int BOTTOM_CENTER = 7;
       static const int BOTTOM_RIGHT = 8;
    }

    class NinePatch
    {
    public:
        NinePatch(TextureRegion* region, int left, int right, int top, int bottom);

        void draw(SpriteBatch* batch, float x, float y, float width, float height);
        void draw(SpriteBatch* batch, float x, float y, float originX, float originY, float width, float height, float scaleX,
                   float scaleY, float rotation);
        void setPadding(float left, float right, float top, float bottom);

    private:
        Texture2D* _texture = nullptr;
        int bottomLeft = -1, bottomCenter = -1, bottomRight = -1;
        int middleLeft = -1, middleCenter = -1, middleRight = -1;
        int topLeft = -1, topCenter = -1, topRight = -1;
        float leftWidth{}, rightWidth{}, middleWidth{}, middleHeight{}, topHeight{}, bottomHeight{};
        std::vector<float> vertices[9 * 4 * 5];
        int idx{};
        Color color = Color::WHITE;
        float padLeft = -1, padRight = -1, padTop = -1, padBottom = -1;

        int add(TextureRegion* region, float color, bool isStretchW, bool isStretchH);
        void set(int idx, float x, float y, float width, float height, float color);
        void prepareVertices(SpriteBatch* batch, float x, float y, float width, float height);

    };
}

