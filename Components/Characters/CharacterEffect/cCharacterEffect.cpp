#include "DXUT.h"
#include "cCharacterEffect.h"

void cCharacterEffect::Init()
{
    cCharacterAnimationPlayer* animPlayer = AddComponent<cCharacterAnimationPlayer>();
    animPlayer->SetCharacterEventHandler(this);
}

void cCharacterEffect::Update()
{
}

void cCharacterEffect::Render()
{
}

void cCharacterEffect::Release()
{
}

void cCharacterEffect::OnCollision(cObject* _other)
{
}

void cCharacterEffect::OnAlarm(std::string _key)
{
}

void cCharacterEffect::OnAnimationEnd(cCharacterAnimation* _anim)
{
    if (!_anim->FindEventKey("Loop"))
    {
        m_Owner->Destroy();
        m_From->RemoveAttachedEffect(this);
    }
}

void cCharacterEffect::OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect)
{
}

void cCharacterEffect::OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect)
{
}

void cCharacterEffect::OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect)
{
}

void cCharacterEffect::OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect)
{
}

void cCharacterEffect::OnCollisionWithCharacter(cCharacter* _with, const RECT& _bodyRect, const RECT& _overlapped)
{
}

void cCharacterEffect::OnCollisionWithMap(cBlock* _with, const RECT& _bodyRect, const RECT& _overlapped)
{
}

void cCharacterEffect::OnSpriteChanged(cCharacterSprite* _sprite)
{
}

void cCharacterEffect::HandleAnimationEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacterEffect::HandleSpriteEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacterEffect::HandleHurtBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacterEffect::HandleHitBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacterEffect::HandleThrowBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacterEffect::HandleBodyBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacterEffect::SetAnimation(cCharacterData* _charData, cCharacterAnimation* _anim)
{
    cCharacterAnimationPlayer* animPlayer = GetComponent<cCharacterAnimationPlayer>();
    animPlayer->SetAnimationImmediately(_anim);
    animPlayer->SetPalette(_charData->GetPalette(_anim->GetSprite(0)->GetFixedPaletteIndex()));
}

void cCharacterEffect::Serialize(char* _buffer, UINT& _pointer) const
{
}

void cCharacterEffect::Deserialize(char* _buffer, UINT& _pointer)
{
}

size_t cCharacterEffect::GetSize() const
{
    return 0;
}
