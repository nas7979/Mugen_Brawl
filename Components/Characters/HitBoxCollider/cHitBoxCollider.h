#pragma once
#include "../cCharacter.h"

class cCharacterSprite;

class cHitBoxCollider : public cComponent
{
public:
    ~cHitBoxCollider() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

private:
    cCharacter* m_Character = nullptr;
    cCharacterSprite** m_SpriteRef = nullptr;

public:
    void SetCharacter(cCharacter* _character) {m_Character = _character;}
    void SetSpriteRef(cCharacterSprite** _spriteRef) {m_SpriteRef = _spriteRef;}
};
