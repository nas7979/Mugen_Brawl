#pragma once

class cEventKey
{
public:
    virtual ~cEventKey();
    
protected:
    SerializedString m_EventKey;
    std::vector<std::pair<std::string, std::string>> m_SeparatedEventKeys;

public:
    void SetEventKey(const std::string& _eventKey) {m_EventKey = _eventKey;}
    std::string GetEventKey() const {return m_EventKey;}
    bool FindEventKey(const std::string& _key, std::string* _result = nullptr) const;
};
