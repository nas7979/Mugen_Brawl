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
    m_Sound = SOUND->CreateSound(_path);

    LPDIRECTSOUNDBUFFER soundBuffer = m_Sound->GetBuffer(0);
    soundBuffer->GetFormat(&m_WaveFormat, sizeof(WAVEFORMATEX), nullptr);
    LPVOID audioPtr1 = 0;
    DWORD audioBytes1 = 0;
    soundBuffer->Lock(0, 0, &audioPtr1, &audioBytes1, nullptr, 0, DSBLOCK_ENTIREBUFFER);
    m_SoundBinary.SetSize(audioBytes1);
    memcpy(const_cast<char*>(m_SoundBinary.GetBuffer()), audioPtr1, audioBytes1);
    soundBuffer->Unlock(&audioPtr1, audioBytes1, nullptr, 0);
}
