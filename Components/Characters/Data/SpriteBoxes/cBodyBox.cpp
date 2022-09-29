#include "DXUT.h"
#include "cBodyBox.h"

void cBodyBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
}

void cBodyBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
}

size_t cBodyBox::GetSize() const
{
    return cSpriteBox::GetSize();
}
