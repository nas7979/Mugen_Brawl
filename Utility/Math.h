#pragma once

namespace Math
{
    int Random(int _min, int _max);
    float Random(float _min, float _max);
    float Clamp(float _value, float _min, float _max);
    float Distance(Vec3 _a, Vec3 _b);
    float Distance(Vec2 _a, Vec2 _b);
    float Sign(float _value);
    Vec2 RotateVec2(Vec2 _vec, float _rot);
}
