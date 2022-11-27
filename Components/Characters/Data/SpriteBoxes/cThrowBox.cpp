#include "DXUT.h"
#include "cThrowBox.h"

void cThrowBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
    CopyTo(_buffer, _pointer, &m_CanThrowAir, sizeof(short) * 2);
}

void cThrowBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
    CopyFrom(_buffer, _pointer, &m_CanThrowAir, sizeof(short) * 2);
}

size_t cThrowBox::GetSize() const
{
    return cSpriteBox::GetSize() + sizeof(short) * 2;
}
