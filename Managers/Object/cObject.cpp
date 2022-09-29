#include "DXUT.h"
#include "cObject.h"

#include "../../Components/cComponent.h"

void cObject::Init()
{
}

void cObject::Update()
{
    for(auto& iter : m_Components)
    {
        iter->Update();
    }

    for(auto iter = m_Alarms.begin(); iter != m_Alarms.end();)
    {
        (*iter).frameLeft--;
        if ((*iter).frameLeft <= 0)
        {
            for(auto& component : m_Components)
                component->OnAlarm((*iter).key);
            iter = m_Alarms.erase(iter);
        }
        else
            iter++;
    }
}

void cObject::Render()
{
    for(auto& iter : m_Components)
    {
        iter->Render();
    }
}

void cObject::Release()
{
    for(auto& iter : m_Components)
    {
        iter->Release();
        delete iter;
    }
}

void cObject::Serialize(char* _buffer, UINT& _pointer) const
{
}

void cObject::Deserialize(char* _buffer, UINT& _pointer)
{
}

size_t cObject::GetSize() const
{
    return 0;
}

void cObject::AddAlarm(std::string _key, int _frame)
{
    m_Alarms.push_back(Alarm{_key, _frame});
}

int cObject::staticUID = 0;