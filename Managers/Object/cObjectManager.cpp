#include "DXUT.h"
#include "cObjectManager.h"

void cObjectManager::Init()
{
}

void cObjectManager::Update()
{
    for (int tag = 0; tag < Obj_End; tag++)
    {
        for (auto iter = m_Objects[tag].begin(); iter != m_Objects[tag].end();)
        {
            if (!(*iter)->IsActive())
            {
                iter++;
                continue;
            }

            (*iter)->Update();
            if ((*iter)->IsDestroyed())
            {
                (*iter)->Release();
                delete *iter;
                iter = m_Objects[tag].erase(iter);
            }
            else
                iter++;
        }
    }
}

void cObjectManager::Render()
{
    for (int tag = 0; tag < Obj_End; tag++)
    {
        for (auto& iter : m_Objects[tag])
        {
            if (!iter->IsActive())
                continue;

            iter->Render();
        }
    }
}

void cObjectManager::Release()
{
    for (int tag = 0; tag < Obj_End; tag++)
    {
        for (auto& iter : m_Objects[tag])
        {
            iter->Release();
            delete iter;
        }
        m_Objects[tag].clear();
    }
}

cObject* cObjectManager::FindObject(std::string _name, ObjectTag _tag)
{
    for(auto& iter : m_Objects[_tag])
    {
        if (iter->GetName()._Equal(_name))
            return iter;
    }

    return nullptr;
}

void cObjectManager::FindObjects(std::vector<cObject*>& _out, ObjectTag _tag, std::string _name)
{
    if (_name.empty())
    {
        _out.resize(m_Objects[_tag].size());
        for (auto& iter : m_Objects[_tag])
            _out.push_back(iter);
    }
    else
    {
        for (auto& iter : m_Objects[_tag])
        {
            if (iter->GetName()._Equal(_name))
                _out.push_back(iter);
        }
    }
}
