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
    short m_CanThrowAir = false;
    short m_CanThrowGround = false;

public:
    void SetCanThrowAir(bool _set) {m_CanThrowAir = _set;}
    bool GetCanThrowAir() const {return m_CanThrowAir;}
    void SetCanThrowGround(bool _set) {m_CanThrowGround = _set;}
    bool GetCanThrowGround() const {return m_CanThrowGround;}
};
