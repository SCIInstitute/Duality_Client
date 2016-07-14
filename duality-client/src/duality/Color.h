#pragma once

#include "duality/Error.h"

struct Color {
    Color()
        : red(0.0f)
        , green(0.0f)
        , blue(0.0f)
        , alpha(0.0f) {}
    Color(float r, float g, float b, float a)
        : red(r)
        , green(g)
        , blue(b)
        , alpha(a) {
        if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1 || a < 0 || a > 1) {
            throw Error("Invalid color", __FILE__, __LINE__);
        }
    }

    float red;
    float green;
    float blue;
    float alpha;
};