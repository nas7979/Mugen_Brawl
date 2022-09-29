#pragma once
#include "cSound.h"

class cSoundSet : public Serializer
{
public:
    ~cSoundSet() override;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    DynamicSerializedVector<cSound*> m_Sounds;

public:
    void AddSound(cSound* _sound);
    void RemoveSound(int _index, bool _delete);
    int GetSoundsCount() const {return m_Sounds.GetLength();}
    cSound* GetSound(int _index) {return m_Sounds[_index];}
    cSound* PickSound() {return m_Sounds[Random(0, m_Sounds.GetLength() - 1)];}
};
