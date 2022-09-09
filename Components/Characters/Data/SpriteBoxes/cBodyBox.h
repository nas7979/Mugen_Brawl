#pragma once
#include "cSpriteBox.h"

class cBodyBox : public cSpriteBox
{
public:
    cBodyBox() = default;
    cBodyBox(short _left, short _top, short _right, short _bottom) : cSpriteBox(_left, _top, _right, _bottom) {}
    ~cBodyBox() override = default;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;
};
