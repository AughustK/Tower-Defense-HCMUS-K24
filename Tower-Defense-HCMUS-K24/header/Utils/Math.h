#pragma once
#include <array>
#include <cmath>

using std::array;

namespace Math 
{

    // Simple 2D vector
    struct Vec2 { float x, y; };

    // Dot product
    static inline float dot(const Vec2& a, const Vec2& b) 
    {
        return a.x * b.x + a.y * b.y;
    }

    // Perpendicular vector
    static inline Vec2 perp(const Vec2& v) 
    {
        return { -v.y, v.x };
    }

    // Normalize with explicit if/else
    static inline Vec2 normalize(const Vec2& v) 
    {
        float len = std::sqrt(dot(v, v));
        if (len > 0.0f) 
        {
            return { v.x / len, v.y / len };
        }
        else {
            return { 0.0f, 0.0f };
        }
    }

    // SAT?based OBB vs circle
    static inline bool rectangleCircleSAT( float boxX, float boxY, float halfLength, float halfThickness,
                                           float rotation, float cx, float cy, float radius)
    {
        const float c = std::cos(rotation);
        const float s = std::sin(rotation);

        // compute world?space corners
        array<Vec2, 4> local = 
        { {
            { halfLength,  halfThickness},
            {-halfLength,  halfThickness},
            {-halfLength, -halfThickness},
            { halfLength, -halfThickness}
        } };
        array<Vec2, 4> corners;
        for (int i = 0; i < 4; ++i) 
        {
            corners[i] = {
                boxX + c * local[i].x - s * local[i].y,
                boxY + s * local[i].x + c * local[i].y
            };
        }

        // build axes: 4 box normals
        array<Vec2, 8> axes;
        int axisCount = 0;
        for (int i = 0; i < 4; ++i) 
        {
            Vec2 edge = { corners[(i + 1) & 3].x - corners[i].x, corners[(i + 1) & 3].y - corners[i].y };
            axes[axisCount++] = normalize(perp(edge));
        }
        // plus up to 4 corner?circle axes
        Vec2 center{ cx, cy };
        for (int i = 0; i < 4; ++i) 
        {
            Vec2 toCenter = { corners[i].x - center.x, corners[i].y - center.y };
            float d2 = dot(toCenter, toCenter);
            if (d2 > 1e-6f && axisCount < (int)axes.size())
            {
                axes[axisCount++] = normalize(toCenter);
            }
        }

        // project onto each axis
        for (int i = 0; i < axisCount; ++i) 
        {
            Vec2 axis = axes[i];
            float minA = dot(corners[0], axis), maxA = minA;
            for (int j = 1; j < 4; ++j) 
            {
                float p = dot(corners[j], axis);
                if (p < minA) minA = p;
                if (p > maxA) maxA = p;
            }
            float centerProj = dot(center, axis);
            float minB = centerProj - radius;
            float maxB = centerProj + radius;
            if (maxA < minB || maxB < minA)
            {
                return false;
            }
        }
        return true;
    }

    // Swept circle vs circle
    static inline bool sweptCircle( float x0, float y0, float x1, float y1, float r0, float cx, float cy, float radius)
    {
        Vec2 d{ x1 - x0, y1 - y0 };
        float r = r0 + radius;

        // broadphase AABB test
        float minX = std::min(x0, x1) - r;
        float maxX = std::max(x0, x1) + r;
        float minY = std::min(y0, y1) - r;
        float maxY = std::max(y0, y1) + r;
        if (cx < minX || cx > maxX || cy < minY || cy > maxY) 
        {
            return false;
        }

        Vec2 f{ x0 - cx, y0 - cy };
        float a = dot(d, d);
        float b = 2.0f * dot(f, d);
        float c = dot(f, f) - r * r;
        float disc = b * b - 4.0f * a * c;
        if (disc < 0.0f) 
        {
            return false;
        }

        float sqrtDisc = std::sqrt(disc);
        float t = (-b - sqrtDisc) / (2.0f * a);
        if (t >= 0.0f && t <= 1.0f) 
        {
            return true;
        }
        else {
            return false;
        }
    }

} 


