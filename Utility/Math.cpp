#include "DXUT.h"
#include "Math.h"

int Math::Random(int _min, int _max)
{
    return _min + (rand() % (_max - _min + 1));
}

float Math::Random(float _min, float _max)
{
    return (float)Random((int)(_min * 10000), (int)(_max * 10000)) * 0.0001f;
}

float Math::Clamp(float _value, float _min, float _max)
{
    return max(_min, min(_max, _value));
}

float Math::Distance(Vec3 _a, Vec3 _b)
{
    return Distance(Vec2(_a.x,_a.y), Vec2(_b.x, _b.y));
}

float Math::Distance(Vec2 _a, Vec2 _b)
{
    Vec2 pow = Vec2(_b.x - _a.x, _b.y - _a.y);
    return sqrt(pow.x * pow.x + pow.y + pow.y);
}

float Math::Sign(float _value)
{
    if (_value > 0)
        return 1;
    if (_value < 0)
        return -1;
    return 0;
}

