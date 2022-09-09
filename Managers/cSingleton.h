#pragma once

template<typename T>
class cSingleton
{
private:
    static T* _instance;
public:
    static void CreateInstance()
    {
        _instance = new T();
        _instance->Init();
    }

    static T* GetInstance()
    {
        return _instance;
    }

    static void ReleaseInstance()
    {
        _instance->Release();
        delete _instance;
        _instance = nullptr;
    }
};
template<typename T>
T* cSingleton<T>::_instance = nullptr;