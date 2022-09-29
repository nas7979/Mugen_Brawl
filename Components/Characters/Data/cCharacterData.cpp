#include "DXUT.h"
#include "cCharacterData.h"

cCharacterData::cCharacterData()
{
}

cCharacterData::~cCharacterData()
{
    for (const auto& iter : m_Animations)
        delete iter.second;

    for (int i = 0; i < m_Palettes.GetLength(); i++)
        delete m_Palettes[i];

    for (const auto& iter : m_SoundSets)
        delete iter.second;
}

void cCharacterData::Serialize(char* _buffer, UINT& _pointer) const
{
    short animationCount = m_Animations.size();
    short soundCount = m_SoundSets.size();

    CopyTo(_buffer, _pointer, &animationCount, sizeof(short));
    CopyTo(_buffer, _pointer, &soundCount, sizeof(short));
    CopyTo(_buffer, _pointer, &m_Version, sizeof(float) * 1 + sizeof(short) * 12);
    m_Name.Serialize(_buffer, _pointer);
    
    for (auto& iter : m_Animations)
    {
        iter.first.Serialize(_buffer, _pointer);
        iter.second->Serialize(_buffer, _pointer);
    }

    for (auto& iter : m_SoundSets)
    {
        iter.first.Serialize(_buffer, _pointer);
        iter.second->Serialize(_buffer, _pointer);
    }
    
    m_Palettes.Serialize(_buffer, _pointer);
}

void cCharacterData::Deserialize(char* _buffer, UINT& _pointer)
{
    short animationCount;
    short soundCount;
    
    CopyFrom(_buffer, _pointer, &animationCount, sizeof(short));
    CopyFrom(_buffer, _pointer, &soundCount, sizeof(short));
    CopyFrom(_buffer, _pointer, &m_Version, sizeof(float) * 1 + sizeof(short) * 12);
    m_Name.Deserialize(_buffer, _pointer);
    
    for (int i = 0; i < animationCount; i++)
    {
        SerializedString key;
        cCharacterAnimation* animation = new cCharacterAnimation();
        key.Deserialize(_buffer, _pointer);
        animation->Deserialize(_buffer, _pointer);
        animation->SetKey(key);
        m_Animations[key] = animation;
    }

    for (int i = 0; i < soundCount; i++)
    {
        SerializedString key;
        cSoundSet* soundSet = new cSoundSet();
        key.Deserialize(_buffer, _pointer);
        soundSet->Deserialize(_buffer, _pointer);
        m_SoundSets[key] = soundSet;
    }

    m_Palettes.Deserialize(_buffer, _pointer);

    m_Commands.clear();

    for (char i = 'a'; i <= 'c'; i++)
    {
        std::string command = std::string("626") + i;
        auto find = m_Animations.find(command);
        if (find != m_Animations.end())
            m_Commands.push_back(command);
    }
    
    for (char i = 'a'; i <= 'c'; i++)
    {
        std::string command = std::string("26") + i;
        auto find = m_Animations.find(command);
        if (find != m_Animations.end())
            m_Commands.push_back(command);
    }
    
    m_Version = CURRENT_VERSION;
}

size_t cCharacterData::GetSize() const
{
    size_t size = sizeof(float) * 1 + sizeof(short) * 14 + m_Name.GetSize() + m_Palettes.GetSize();
    
    for (auto& iter : m_Animations)
        size += iter.first.GetSize() + iter.second->GetSize();
    for (auto& iter : m_SoundSets)
        size += iter.first.GetSize() + iter.second->GetSize();
    
    return size;
}

cCharacterAnimation* cCharacterData::AddAnimation(std::string _key)
{
    cCharacterAnimation* anim = new cCharacterAnimation();
    m_Animations[_key] = anim;
    return anim;
}

void cCharacterData::RemoveAnimation(std::string _key)
{
    delete m_Animations[_key];
    m_Animations.erase(_key);
}

cCharacterAnimation* cCharacterData::GetAnimation(std::string _animName)
{
    auto find = m_Animations.find(_animName);
    if (find == m_Animations.end())
        return nullptr;
    return (*find).second;
}

cCharacterSprite* cCharacterData::GetSprite(std::string _animName, int _frame)
{
    return m_Animations[_animName]->GetSprite(_frame);
}

void cCharacterData::SetPalette(cPalette* _palette)
{
    for (int i = 0; i < m_Palettes.GetLength(); i++)
    {
        if (m_Palettes[i]->GetIndex() == _palette->GetIndex())
        {
            delete m_Palettes[i];
            m_Palettes[i] = _palette;
            return;
        }
    }

    m_Palettes.GetValue()->push_back(_palette);
    std::sort(m_Palettes.GetValue()->begin(), m_Palettes.GetValue()->end(), [](cPalette* a, cPalette* b)->bool{return a->GetIndex() < b->GetIndex();});
}

cPalette* cCharacterData::GetPalette(int _index)
{
    if (_index < FIXED_PALETTE_INDEX && m_Palettes.GetLength() > _index)
        return m_Palettes[_index];
    
    for (int i = m_Palettes.GetLength() - 1; i >= 0; i--)
    {
        if (m_Palettes[i]->GetIndex() == _index)
            return m_Palettes[i];
    }

    return nullptr;
}

void cCharacterData::RemovePalette(int _index)
{
    for (auto iter = m_Palettes.GetValue()->begin(); iter != m_Palettes.GetValue()->end(); iter++)
    {
        if ((*iter)->GetIndex() == _index)
        {
            m_Palettes.GetValue()->erase(iter);
            return;
        }
    }
}

cSoundSet* cCharacterData::AddSoundSet(std::string _key)
{
    cSoundSet* set = new cSoundSet();
    m_SoundSets[_key] = set;
    return set;
}

void cCharacterData::RemoveSoundSet(std::string _key)
{
    delete m_SoundSets[_key];
    m_SoundSets.erase(_key);
}

cSoundSet* cCharacterData::GetSoundSet(std::string _key)
{
    auto find = m_SoundSets.find(_key);
    if (find == m_SoundSets.end())
        return nullptr;
    return (*find).second;
}
