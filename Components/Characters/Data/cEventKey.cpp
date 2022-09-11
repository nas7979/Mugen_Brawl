#include "DXUT.h"
#include "cEventKey.h"

cEventKey::~cEventKey()
{
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
