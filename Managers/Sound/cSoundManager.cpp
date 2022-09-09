#include "DXUT.h"
#include "cSoundManager.h"

void cSoundManager::Init()
{
    m_Manager.Initialize(DXUTGetHWND(), 1);
}

void cSoundManager::Update()
{
    DWORD status;
    for (auto iter = m_Channels.begin(); iter != m_Channels.end();)
    {
        (*iter)->GetStatus(&status);
        if ((status & DSBSTATUS_PLAYING) == 0)
        {
            (*iter)->Release();
            iter = m_Channels.erase(iter);
        }
        else
            iter++;
    }
}

void cSoundManager::Render()
{
}

void cSoundManager::Release()
{
    StopAll();
    for (auto& iter : m_Sounds)
    {
        delete iter.second;
    }
}

void cSoundManager::AddSound(std::wstring& _path, std::string& _key)
{
    m_Sounds[_key] = CreateSound(_path);
}

CSound* cSoundManager::CreateSound(std::wstring& _path)
{
    CSound* sound;
    m_Manager.Create(&sound, (LPWSTR)_path.c_str(), DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN);
    return sound;
}

CSound* cSoundManager::CreateFromMemory(LPWAVEFORMATEX _format, char* _buffer, UINT& pointer, size_t _size)
{
    CSound* sound;
    pointer += sizeof(WAVEFORMATEX);
    m_Manager.CreateFromMemory(&sound, (BYTE*)_buffer + pointer, _size, _format, DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN);
    pointer += _size;
    return sound;
}

LPDIRECTSOUNDBUFFER cSoundManager::Play(std::string _key, int volume)
{
    return Play(m_Sounds[_key], volume);
}

LPDIRECTSOUNDBUFFER cSoundManager::Play(CSound* _sound, int volume)
{
    LPDIRECTSOUNDBUFFER buffer;
    m_Manager.GetDirectSound()->DuplicateSoundBuffer(_sound->GetBuffer(0), &buffer);
    buffer->SetVolume(DSBVOLUME_MIN + (int)((float)volume * (100.f / 255.f)) * 100);
    buffer->Play(0,0,0);
    m_Channels.push_back(buffer);
    return buffer;
}

void cSoundManager::StopAll()
{
    for (auto& iter : m_Channels)
    {
        iter->Stop();
        iter->Release();
    }
    m_Channels.clear();
}
