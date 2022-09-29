#include "DXUT.h"
#include "cSpriteBox.h"

void cSpriteBox::Serialize(char* _buffer, UINT& _pointer) const
{
    CopyTo(_buffer, _pointer, &m_Left, sizeof(short) * 4);
    m_EventKey.Serialize(_buffer, _pointer);
}

void cSpriteBox::Deserialize(char* _buffer, UINT& _pointer)
{
    CopyFrom(_buffer, _pointer, &m_Left, sizeof(short) * 4);
    m_EventKey.Deserialize(_buffer, _pointer);
    SeparateEventKey();
}

size_t cSpriteBox::GetSize() const
{
    return sizeof(short) * 4 + m_EventKey.GetSize();
}