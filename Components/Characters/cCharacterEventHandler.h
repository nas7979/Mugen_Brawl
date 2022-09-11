#pragma once

class cBodyBox;
class cHurtBox;
class cThrowBox;
class cHitBox;
class cCharacter;

class cCharacterEventHandler
{
public:
    virtual ~cCharacterEventHandler() = default;
    
    virtual void OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect) = 0;
    virtual void OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect) = 0;
    virtual void OnThrew(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect) = 0;
    virtual void OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect) = 0;
    virtual void OnCollisionWithCharacter(cCharacter* _with, RECT _overlappedRect) = 0;
    virtual void OnCollisionWithMap(cCharacter* _with, RECT _overlappedRect) = 0;
 
    virtual void HandleAnimationEvent(std::string _eventKey) = 0;
    virtual void HandleSpriteEvent(std::string _eventKey) = 0;
    virtual void HandleHurtBoxEvent(std::string _eventKey) = 0;
    virtual void HandleHitBoxEvent(std::string _eventKey) = 0;
    virtual void HandleThrowBoxEvent(std::string _eventKey) = 0;
    virtual void HandleBodyBoxEvent(std::string _eventKey) = 0;
};
