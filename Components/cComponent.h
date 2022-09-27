#pragma once
#include "../Managers/Object/cObject.h"

class cComponent
{
public:
    cComponent(cObject* _owner)
    {
        m_Owner = _owner;
    }
    virtual ~cComponent() {}
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Release() = 0;
    virtual void OnCollision(cObject* _other) = 0;
    virtual void OnAlarm(std::string _key) = 0;

protected:
    cObject* m_Owner;

public:
    template<typename T>
    T* AddComponent()
    {
        return m_Owner->AddComponent<T>();
    }

    template<typename T>
    T* GetComponent()
    {
        return m_Owner->GetComponent<T>();
    }

    cObject* GetOwner() const {return m_Owner;}
};
