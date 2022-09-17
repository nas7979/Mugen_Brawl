#pragma once

class cGameManager : public cSingleton<cGameManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    int m_FrameTimer;

public:
    int GetFrameTimer() const {return m_FrameTimer;}
};


#define GAME cGameManager::GetInstance()
#define FRAME_TIMER GAME->GetFrameTimer()