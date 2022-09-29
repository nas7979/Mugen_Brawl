#include "DXUT.h"
#include "cHurtBox.h"

void cHurtBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
}

void cHurtBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
}

size_t cHurtBox::GetSize() const
{
    return cSpriteBox::GetSize();
}
