#pragma once

struct SerializedString : Serializer
{
public:
    SerializedString();
    SerializedString(std::string _string);
    SerializedString(char* _buffer, UINT& _pointer);

private:
    std::string m_String;

public:
    void SetSize(size_t _size) {m_String.resize(_size);}
    size_t GetSize() const override {return sizeof(UINT) + sizeof(char) * m_String.size();}
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    const char* GetBuffer() const {return m_String.data();}
    size_t GetLength() const {return m_String.size();}
    std::string& GetString() const {return const_cast<std::string&>(m_String);}

    operator std::string() const {return m_String;}
    bool operator < (const SerializedString& _right) const {return m_String < _right.m_String;}
};
