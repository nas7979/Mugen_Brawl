#pragma once
#include "cCharacterAnimation.h"
#include "cCharacterSprite.h"
#include "cPalette.h"
#include "cSoundSet.h"

class cCharacterData : Serializer
{
public:
    cCharacterData();
    ~cCharacterData() override;
    
private:
    short m_Version = CURRENT_VERSION;
    short m_PaletteLength = 256;
    float m_SpriteScale = 1;

    short m_Weight = 100;
    short m_JumpHeight = 500;
    short m_FallSpeed = 500;
    short m_FastFallSpeed = 800;
    short m_WalkSpeed = 200;
    short m_DashSpeed = 500;
    short m_AirSpeed = 200;
    short m_PreJump = 3;
    short m_ShieldSize = 125;
    short m_ShieldDecSpeed = 20;
    
    SerializedString m_Name;
    std::map<SerializedString, cCharacterAnimation*> m_Animations;
    DynamicSerializedVector<cPalette*> m_Palettes;
    std::map<SerializedString, cSoundSet*> m_SoundSets;

public:
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

    void SetWeight(float _weight){m_Weight = _weight;}
    float GetWeight() {return m_Weight;}
    void SetSpriteScale(float _scale) {m_SpriteScale = _scale;}
    float GetSpriteScale() {return m_SpriteScale;}
    void SetName(const std::string& _name) {m_Name = _name;}
    std::string GetName() {return m_Name;}
    
    cCharacterAnimation* AddAnimation(std::string _key);
    void RemoveAnimation(std::string _key);
    cCharacterAnimation* GetAnimation(std::string _animName);
    std::map<SerializedString, cCharacterAnimation*>* GetAnimations() {return &m_Animations;}
    cCharacterSprite* GetSprite(std::string _animName, int _frame);

    void SetPalette(cPalette* _palette);
    cPalette* GetPalette(int _index);
    void RemovePalette(int _index);
    int GetPaletteCount() {return m_Palettes.GetLength();}
    void SetPaletteSize(int _size) {m_PaletteLength = _size;}
    int GetPaletteLength() {return m_PaletteLength;}

    cSoundSet* AddSoundSet(std::string _key);
    void RemoveSoundSet(std::string _key);
    cSoundSet* GetSoundSet(std::string _key);
    std::map<SerializedString, cSoundSet*>* GetSoundSets() {return &m_SoundSets;}
};

#define FIXED_PALETTE_INDEX 1000