#pragma once

template<typename T>
struct SerializedArray : Serializer
{
    SerializedArray() : Serializer() {}
    SerializedArray(T* _array, size_t _length) : Serializer(), m_Value(_array), m_ArrayLength(_length) {}
    SerializedArray(const SerializedArray& _serializedArray) : Serializer(), m_Value(_serializedArray.m_Value), m_ArrayLength(_serializedArray.m_ArrayLength) {}
    ~SerializedArray() override {SAFE_DELETE_ARRAY(m_Value)}
private:
    T* m_Value = nullptr;
    size_t m_ArrayLength = 0;

public:
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override {return sizeof(UINT) + sizeof(T) * m_ArrayLength;}

    void SetValue(T* _value, int _length)
    {
        SAFE_DELETE_ARRAY(m_Value)
        m_Value = _value;
        m_ArrayLength = _length;
    }
    T* GetValue() {return m_Value;}
    size_t GetLength() const {return m_ArrayLength;}
    
    SerializedArray& operator=(const SerializedArray& _serializedArray)
    {
        SAFE_DELETE_ARRAY(m_Value)
        m_Value = new T[_serializedArray.m_ArrayLength];
        m_ArrayLength = _serializedArray.m_ArrayLength;
        memcpy(m_Value, _serializedArray.m_Value, m_ArrayLength);
        return *this;
    }

    T& operator[](int _idx)
    {
        return m_Value[_idx];
    }
};

template <typename T>
void SerializedArray<T>::Serialize(char* _buffer, UINT& _pointer) const
{
    memcpy(_buffer + _pointer, &m_ArrayLength, sizeof(UINT));
    _pointer += sizeof(UINT);
    size_t size = sizeof(T) * m_ArrayLength;
    if (m_Value != nullptr)
        memcpy(_buffer + _pointer, m_Value, size);
    _pointer += size;
}

template <typename T>
void SerializedArray<T>::Deserialize(char* _buffer, UINT& _pointer)
{
    memcpy(&m_ArrayLength, _buffer + _pointer, sizeof(UINT));
    _pointer += sizeof(UINT);
    size_t size = sizeof(T) * m_ArrayLength;
    SAFE_DELETE_ARRAY(m_Value);
    m_Value = new T[m_ArrayLength];
    if (m_ArrayLength > 0)
        memcpy(m_Value, _buffer + _pointer, size);
    _pointer += size;
}



template<typename T>
struct SerializedVector : Serializer
{
    SerializedVector() : Serializer() {}
    SerializedVector(const SerializedVector& _serializedVector) : Serializer(), m_Vector(_serializedVector.m_Vector) {}
    ~SerializedVector() override {}
private:
    std::vector<T> m_Vector;

public:
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override {return sizeof(UINT) + sizeof(T) * m_Vector.size();}

    void SetValue(std::vector<T> _vector) {m_Vector = _vector;}
    std::vector<T>* GetValue() const {return const_cast<std::vector<T>*>(&m_Vector);}
    size_t GetLength() const {return m_Vector.size();}
    
    SerializedVector& operator=(const SerializedVector& _serializedVector)
    {
        m_Vector = _serializedVector.m_Vector;
        return *this;
    }

    T& operator[](int _idx)
    {
        return m_Vector[_idx];
    }
};

template <typename T>
void SerializedVector<T>::Serialize(char* _buffer, UINT& _pointer) const
{
    UINT length = m_Vector.size();
    memcpy(_buffer + _pointer, &length, sizeof(UINT));
    _pointer += sizeof(UINT);
    size_t size = sizeof(T) * m_Vector.size();
    memcpy(_buffer + _pointer, m_Vector.data(), size);
    _pointer += size;
}

template <typename T>
void SerializedVector<T>::Deserialize(char* _buffer, UINT& _pointer)
{
    UINT vectorSize;
    memcpy(&vectorSize, _buffer + _pointer, sizeof(UINT));
    _pointer += sizeof(UINT);
    size_t size = vectorSize * sizeof(T);
    m_Vector.resize(vectorSize);
    memcpy(m_Vector.data(), _buffer + _pointer, size);
    _pointer += size;
}



template<typename T>
struct DynamicSerializedArray : Serializer
{
    DynamicSerializedArray() : Serializer() {}
    DynamicSerializedArray(const DynamicSerializedArray& _dynamicSerializedArray) = default;
    DynamicSerializedArray(T* _array, size_t _length) : Serializer(), m_Value(_array), m_ArrayLength(_length) {}
    ~DynamicSerializedArray() override {SAFE_DELETE_ARRAY(m_Value)}
private:
    T* m_Value = nullptr;
    size_t m_ArrayLength = 0;

public:
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override
    {
        size_t size = sizeof(UINT);
        for (int i = 0; i < m_ArrayLength; i++)
            size += m_Value[i]->GetSize();
        return size;
    }
    
    void SetValue(T* _value, int _length)
    {
        SAFE_DELETE_ARRAY(m_Value)
        m_Value = _value;
        m_ArrayLength = _length;
    }
    T* GetValue() const {return const_cast<T*>(m_Value);}
    size_t GetLength() const {return m_ArrayLength;}
    
    DynamicSerializedArray& operator=(const DynamicSerializedArray& _dynamicSerializedArray)
    {
        m_ArrayLength = _dynamicSerializedArray.m_ArrayLength;
        m_Value = _dynamicSerializedArray.m_Value;
        return *this;
    }

    T& operator[](int _idx) const
    {
        return m_Value[_idx];
    }
};

template <typename T>
void DynamicSerializedArray<T>::Serialize(char* _buffer, UINT& _pointer) const
{
    memcpy(_buffer + _pointer, &m_ArrayLength, sizeof(UINT));
    _pointer += sizeof(UINT);
    for (int i = 0; i < m_ArrayLength; i++)
        m_Value[i]->Serialize(_buffer, _pointer);
}

template <typename T>
void DynamicSerializedArray<T>::Deserialize(char* _buffer, UINT& _pointer)
{
    typedef typename std::remove_pointer<typename std::decay<T>::type>::type C;
    memcpy(&m_ArrayLength, _buffer + _pointer, sizeof(UINT));
    _pointer += sizeof(UINT);
    SAFE_DELETE_ARRAY(m_Value);
    m_Value = new T[m_ArrayLength];
    for (int i = 0; i < m_ArrayLength; i++)
    {
        C* element = new C;
        element->Deserialize(_buffer, _pointer);
        m_Value[i] = element;
    }
}



template<typename T>
struct DynamicSerializedVector : Serializer
{
    DynamicSerializedVector() : Serializer() {}
    DynamicSerializedVector(const DynamicSerializedVector& _dynamicSerializedVector) : Serializer(), m_Vector(_dynamicSerializedVector.m_Vector) {}
    DynamicSerializedVector(const std::vector<T> _vector) : Serializer(), m_Vector(_vector) {}
    ~DynamicSerializedVector() override {}
private:
    std::vector<T> m_Vector;

public:
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override
    {
        size_t size = sizeof(UINT);
        for (auto& iter : m_Vector)
            size += iter->GetSize();
        return size;
    }
    size_t GetLength() const {return m_Vector.size();}

    void SetValue(std::vector<T> _vector) {m_Vector = _vector;}
    std::vector<T>* GetValue() const {return const_cast<std::vector<T>*>(&m_Vector);}
    
    DynamicSerializedVector& operator=(const DynamicSerializedVector& _dynamicSerializedVector)
    {
        m_Vector = _dynamicSerializedVector.m_Vector;
        return *this;
    }

    T& operator[](int _idx)
    {
        return m_Vector[_idx];
    }
};

template <typename T>
void DynamicSerializedVector<T>::Serialize(char* _buffer, UINT& _pointer) const
{
    UINT size = m_Vector.size();
    memcpy(_buffer + _pointer, &size, sizeof(UINT));
    _pointer += sizeof(UINT);
    for (auto& iter : m_Vector)
        iter->Serialize(_buffer, _pointer);
}

template <typename T>
void DynamicSerializedVector<T>::Deserialize(char* _buffer, UINT& _pointer)
{
    typedef typename std::remove_pointer<typename std::decay<T>::type>::type C;
    UINT vectorSize;
    memcpy(&vectorSize, _buffer + _pointer, sizeof(UINT));
    _pointer += sizeof(UINT);
    m_Vector.resize(vectorSize);
    for (int i = 0; i < vectorSize; i++)
    {
        C* element = new C();
        element->Deserialize(_buffer, _pointer);
        m_Vector[i] = element;
    }
}