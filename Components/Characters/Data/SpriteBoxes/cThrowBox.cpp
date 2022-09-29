#include "DXUT.h"
#include "cThrowBox.h"

void cThrowBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
    CopyTo(_buffer, _pointer, &m_CanThrowMidair, sizeof(int));
}

void cThrowBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
    CopyFrom(_buffer, _pointer, &m_CanThrowMidair, sizeof(int));
}

size_t cThrowBox::GetSize() const
{
    return cSpriteBox::GetSize() + sizeof(int);
}
