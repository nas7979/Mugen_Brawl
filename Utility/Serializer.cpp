#include "DXUT.h"
#include "Serializer.h"

void Serializer::CopyTo(char* _buffer, UINT& _pointer, const void* _src, size_t _size) const
{
    memcpy(_buffer + _pointer, _src, _size);
    _pointer += _size;
}

void Serializer::CopyFrom(char* _buffer, UINT& _pointer, void* _dst, size_t _size)
{
    memcpy(_dst, _buffer + _pointer, _size);
    _pointer += _size;
}