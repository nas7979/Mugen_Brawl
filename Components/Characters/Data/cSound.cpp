#include "DXUT.h"
#include "cSound.h"

#include "../../../Scenes/Editor/cEditorScene.h"

cSound::~cSound()
{
    delete m_Sound;
}

void cSound::Serialize(char* _buffer, UINT& _pointer) const
{
    CopyTo(_buffer, _pointer, &m_Volume, sizeof(BYTE) * 4 + sizeof(WAVEFORMATEX));
    m_SoundBinary.Serialize(_buffer, _pointer);
}

void cSound::Deserialize(char* _buffer, UINT& _pointer)
{
    CopyFrom(_buffer, _pointer, &m_Volume, sizeof(BYTE) * 4 + sizeof(WAVEFORMATEX));
    m_SoundBinary.Deserialize(_buffer, _pointer);

    UINT pointer = 0;
    m_Sound = SOUND->CreateFromMemory(&m_WaveFormat, const_cast<char*>(m_SoundBinary.GetBuffer()), pointer, sizeof(char) * m_SoundBinary.GetLength());
}

size_t cSound::GetSize() const
{
    return sizeof(BYTE) * 4 + sizeof(WAVEFORMATEX) + m_SoundBinary.GetSize();
}

void cSound::SetSound(std::wstring _path)
{
    char* buffer;
    UINT length;
    if (!cEditorScene::OpenFile(_path, &buffer, length))
        return;
    
    m_Sound = SOUND->CreateSound(_path);
    m_Sound->GetBuffer(0)->GetFormat(&m_WaveFormat, sizeof(WAVEFORMATEX), nullptr);

    LPDIRECTSOUNDBUFFER soundBuffer = m_Sound->GetBuffer(0);
    LPVOID audioPtr1 = 0;
    DWORD audioBytes1 = 0;
    LPVOID audioPtr2 = 0;
    DWORD audioBytes2 = 0;
    soundBuffer->Lock(0, 0, &audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, DSBLOCK_ENTIREBUFFER);
    m_SoundBinary.SetSize(audioBytes1 - 1024);
    memcpy(const_cast<char*>(m_SoundBinary.GetBuffer()), buffer + 44, audioBytes1 - 1024);
    soundBuffer->Unlock(&audioPtr1, audioBytes1, &audioPtr2, audioBytes2);

    delete[] buffer;
}
