#pragma once
#include "cSpriteBox.h"

class cThrowBox : public cSpriteBox
{
public:
    cThrowBox() = default;
    cThrowBox(short _left, short _top, short _right, short _bottom) : cSpriteBox(_left, _top, _right, _bottom) {}
    ~cThrowBox() override = default;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    int m_CanThrowMidair = false;

public:
    void SetCanThrowMidair(bool _set) {m_CanThrowMidair = _set;}
    bool GetCanThrowMidair() const {return m_CanThrowMidair;}
};
