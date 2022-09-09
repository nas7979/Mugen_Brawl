#include "DXUT.h"
#include "SerializedString.h"

SerializedString::SerializedString()
{
}

SerializedString::SerializedString(std::string _string)
{
    m_String = _string;
}

SerializedString::SerializedString(char* _buffer, UINT& _pointer)
{
    Deserialize(_buffer, _pointer);
}

void SerializedString::Serialize(char* _buffer, UINT& _pointer) const
{
    size_t textSize = sizeof(char) * m_String.size();
    UINT length = m_String.size();
    memcpy(_buffer + _pointer, &length, sizeof(UINT));
    _pointer += sizeof(UINT);
    memcpy(_buffer + _pointer, m_String.data(), textSize);
    _pointer += textSize;
}

void SerializedString::Deserialize(char* _buffer, UINT& _pointer)
{
    UINT length;
    memcpy(&length, _buffer + _pointer, sizeof(UINT));
    size_t textSize = sizeof(char) * length;
    _pointer += sizeof(UINT);
    m_String.resize(length);
    memcpy((void*)m_String.data(), _buffer + _pointer, textSize);
    _pointer += textSize;
}
