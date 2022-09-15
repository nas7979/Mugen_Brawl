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
    virtual void OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect) = 0;
    virtual void OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect) = 0;
    virtual void OnCollisionWithCharacter(cCharacter* _with, RECT _overlappedRect) = 0;
    virtual void OnCollisionWithMap(cCharacter* _with, RECT _overlappedRect) = 0;
 
    virtual void HandleAnimationEvent(const std::string& _key, const std::string& _value) = 0;
    virtual void HandleSpriteEvent(const std::string& _key, const std::string& _value) = 0;
    virtual void HandleHurtBoxEvent(const std::string& _key, const std::string& _value) = 0;
    virtual void HandleHitBoxEvent(const std::string& _key, const std::string& _value) = 0;
    virtual void HandleThrowBoxEvent(const std::string& _key, const std::string& _value) = 0;
    virtual void HandleBodyBoxEvent(const std::string& _key, const std::string& _value) = 0;
};
