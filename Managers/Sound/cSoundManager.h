#pragma once
#include "SDKsound.h"
#include "SDKwavefile.h"

class cSoundManager : public cSingleton<cSoundManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    CSoundManager m_Manager;
    std::map<std::string, CSound*> m_Sounds;
    std::list<LPDIRECTSOUNDBUFFER> m_Channels;

public:
    void AddSound(std::wstring& _path, std::string& _key);
    CSound* CreateSound(std::wstring& _path);
    CSound* CreateFromMemory(LPWAVEFORMATEX _format, char* _buffer, UINT& _pointer, size_t _size);
    LPDIRECTSOUNDBUFFER Play(std::string _key, int _volume, bool _addToList = true);
    LPDIRECTSOUNDBUFFER Play(CSound* _sound, int _volume, bool _addToList = true);
    void StopAll();
};

#define SOUND cSoundManager::GetInstance()