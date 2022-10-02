#pragma once

class cCharacter;

class cGameManager : public cSingleton<cGameManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    int m_FrameTimer;
    cCharacter* m_Characters[MAX_PLAYER] = {nullptr,};

public:
    int GetFrameTimer() const {return m_FrameTimer;}
    void SetCharacter(cCharacter* _character, int _playerIndex) {m_Characters[_playerIndex] = _character;}
    cCharacter* GetCharacter(int _playerIndex) const {return m_Characters[_playerIndex];}
};


#define GAME cGameManager::GetInstance()
#define FRAME_TIMER GAME->GetFrameTimer()