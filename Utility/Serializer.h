#pragma once

struct Serializer
{
public:
    Serializer() = default;
    virtual ~Serializer() {}
    
    virtual void Serialize(char* _buffer, UINT& _pointer) const = 0;
    virtual void Deserialize(char* _buffer, UINT& _pointer) = 0;
    virtual size_t GetSize() const = 0;

protected:
    void CopyTo(char* _buffer, UINT& _pointer, const void* _src, size_t _size) const;
    void CopyFrom(char* _buffer, UINT& _pointer, void* _dst, size_t _size);
};
