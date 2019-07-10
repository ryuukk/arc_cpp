#pragma once

#include "Texture2D.h"
#include "../math/Mathf.h"

namespace arc
{
    class TextureRegion
    {
    public:
        Texture2D* texture = nullptr;
        float u{};
        float v{};
        float u2{};
        float v2{};
        int regionWidth{};
        int regionHeight{};

        TextureRegion()
        {}

        TextureRegion(Texture2D* texture)
        {
            this->texture = texture;
            setRegion(0, 0, texture->getWidth(), texture->getHeight());
        }

        void setRegion(int x, int y, int width, int height)
        {
            float invTexWidth = 1.0f / texture->getWidth();
            float invTexHeight = 1.0f / texture->getHeight();
            setRegion(x * invTexWidth, y * invTexHeight, (x + width) * invTexWidth, (y + height) * invTexHeight);
            regionWidth = (int) Mathf::abs(width);
            regionHeight = (int) Mathf::abs(height);
        }

        void setRegion(float u, float v, float u2, float v2)
        {
            int texWidth = texture->getWidth();
            int texHeight = texture->getHeight();
            regionWidth = (int) Mathf::round(Mathf::abs(u2 - u) * texWidth);
            regionHeight = (int) Mathf::round(Mathf::abs(v2 - v) * texHeight);

            // For a 1x1 region, adjust UVs toward pixel center to avoid filtering artifacts on AMD GPUs when drawing very stretched.
            if (regionWidth == 1 && regionHeight == 1)
            {
                float adjustX = 0.25f / texWidth;
                u += adjustX;
                u2 -= adjustX;
                float adjustY = 0.25f / texHeight;
                v += adjustY;
                v2 -= adjustY;
            }

            this->u = u;
            this->v = v;
            this->u2 = u2;
            this->v2 = v2;
        }

        void flip(bool x, bool y)
        {
            if (x) {
                float temp = u;
                u = u2;
                u2 = temp;
            }
            if (y) {
                float temp = v;
                v = v2;
                v2 = temp;
            }
        }
    };
}