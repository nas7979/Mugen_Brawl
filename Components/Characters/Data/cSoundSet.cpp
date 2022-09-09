#include "DXUT.h"
#include "cSoundSet.h"

cSoundSet::~cSoundSet()
{
    for (auto& iter : *m_Sounds.GetValue())
    {
        delete iter;
    }
}

void cSoundSet::Serialize(char* _buffer, UINT& _pointer) const
{
    m_Sounds.Serialize(_buffer, _pointer);
}

void cSoundSet::Deserialize(char* _buffer, UINT& _pointer)
{
    m_Sounds.Deserialize(_buffer, _pointer);
}

size_t cSoundSet::GetSize() const
{
    return m_Sounds.GetSize();
}

void cSoundSet::AddSound(cSound* _sound)
{
    m_Sounds.GetValue()->push_back(_sound);
}

void cSoundSet::RemoveSound(int _index, bool _delete)
{
    if (_delete)
        delete m_Sounds[_index];
    m_Sounds.GetValue()->erase(m_Sounds.GetValue()->_Make_iterator_offset(_index));
}
