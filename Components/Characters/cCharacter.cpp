#include "DXUT.h"
#include "cCharacter.h"

void cCharacter::Init()
{
}

void cCharacter::Update()
{
}

void cCharacter::Render()
{
}

void cCharacter::Release()
{
}

void cCharacter::OnCollision(cObject* _other)
{
}

void cCharacter::OnAlarm(std::string _key)
{
}

void cCharacter::OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect)
{
}

void cCharacter::OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect)
{
}

void cCharacter::OnThrew(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect)
{
}

void cCharacter::OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect)
{
}

void cCharacter::OnCollisionWithCharacter(cCharacter* _with, RECT _overlappedRect)
{
}

void cCharacter::OnCollisionWithMap(cCharacter* _with, RECT _overlappedRect)
{
}

void cCharacter::HandleAnimationEvent(std::string _eventKey)
{
}

void cCharacter::HandleSpriteEvent(std::string _eventKey)
{
}

void cCharacter::HandleHurtBoxEvent(std::string _eventKey)
{
}

void cCharacter::HandleHitBoxEvent(std::string _eventKey)
{
}

void cCharacter::HandleThrowBoxEvent(std::string _eventKey)
{
}

void cCharacter::HandleBodyBoxEvent(std::string _eventKey)
{
}

void cCharacter::Serialize(char* _buffer, UINT& _pointer) const
{
}

void cCharacter::Deserialize(char* _buffer, UINT& _pointer)
{
}

size_t cCharacter::GetSize() const
{
    return 0;
}
