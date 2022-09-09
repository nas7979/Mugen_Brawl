#include "DXUT.h"
#include "cHitBox.h"

void cHitBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
    CopyTo(_buffer, _pointer, &m_Damage, sizeof(short) * 4 + sizeof(float) * 3);
}

void cHitBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
    CopyFrom(_buffer, _pointer, &m_Damage, sizeof(short) * 4 + sizeof(float) * 3);
}

size_t cHitBox::GetSize() const
{
    return cSpriteBox::GetSize() + sizeof(short) * 4 + sizeof(float) * 3;
}
