#pragma once
#include "cObject.h"

class cObjectManager : public cSingleton<cObjectManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    std::list<cObject*> m_Objects[Obj_End];
    std::map<int, cObject*> m_RollbackObjects;

public:
    template<typename T>
    T* AddObject(std::string _name, Vec3 _pos, ObjectTag _tag)
    {
        cObject* obj = new cObject(_tag);
        obj->SetName(_name);
        obj->SetPos(_pos);
        obj->SetCreatedAt(FRAME_TIMER);
        m_Objects[_tag].push_back(obj);
        return obj->AddComponent<T>();
    }

    cObject* FindObject(std::string _name, ObjectTag _tag);
    void FindObjects(std::vector<cObject*>& _out, ObjectTag _tag, std::string _name = "");
    const std::list<cObject*>& GetObjects(ObjectTag _tag) const;
};

#define OBJECT cObjectManager::GetInstance()