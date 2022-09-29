#pragma once

class cGGPOManager : public cSingleton<cGGPOManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    bool m_IsOffline = true;

public:
    void SetOffline(bool _set) {m_IsOffline = _set;}
    bool GetIsOffline() const {return m_IsOffline;}
};

#define GGPO cGGPOManager::GetInstance()