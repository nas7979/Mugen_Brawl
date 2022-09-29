#include "DXUT.h"
#include "cEventKey.h"

cEventKey::~cEventKey()
{
}

void cEventKey::SeparateEventKey()
{
    if (m_EventKey.GetLength() == 0)
        return;
    
    std::vector<std::string> separated;
    SplitString(&separated, &m_EventKey.GetString(), ',');

    for (auto& iter : separated)
    {
        size_t colonIndex = iter.find(':');
        if (colonIndex == std::string::npos)
            m_SeparatedEventKeys.emplace_back(iter, "");
        else
            m_SeparatedEventKeys.emplace_back(iter.substr(0, colonIndex), iter.substr(colonIndex + 1));
    }
}

bool cEventKey::FindEventKey(const std::string& _key, std::string* _result) const
{
    for (auto& iter : m_SeparatedEventKeys)
    {
        if (iter.first == _key)
        {
            if (_result != nullptr)
                *_result = iter.second;
            return true;
        }
    }
    return false;
}
