#pragma once

class cSound : public Serializer
{
public:
    ~cSound() override;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    BYTE m_Volume = 255;
    BYTE m_PlayChance = 255;
    BYTE m_isLoop = 0;
    BYTE m_Padding = 0;
    WAVEFORMATEX m_WaveFormat;
    CSound* m_Sound;

    SerializedString m_SoundBinary;
    
public:
    void SetVolume(BYTE _volume) {m_Volume = _volume;}
    BYTE GetVolume() const {return m_Volume;}
    void SetPlayChance(BYTE _chance) {m_PlayChance = _chance;}
    BYTE GetPlayChance() const {return m_PlayChance;}
    void SetIsLoop(bool _isLoop) {m_isLoop = _isLoop;}
    bool IsLoop() const {return m_isLoop;}

    void SetSound(std::wstring _path);
    CSound* GetSound() const {return m_Sound;}
};
