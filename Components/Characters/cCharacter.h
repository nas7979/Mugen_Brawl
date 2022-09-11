#pragma once
#include "cCharacterEventHandler.h"
#include "CharacterAnimationPlayer/cCharacterAnimationPlayer.h"
#include "Data/cCharacterAnimation.h"
#include "Data/cCharacterData.h"

class cCharacterAnimation;
class cCharacterData;

class cCharacter : public cComponent, Serializer, cCharacterEventHandler
{
    enum class State
    {
        Idle,
        Action,
        Hit,
        Down,
        Wake,
        Thrown
    };

    enum class Flag
    {
        InAir = 0x00000001,
        Standing = 0x00000002,
        Crouching = 0x00000004,
        Counter = 0x00000008,
        Shield_Mid = 0x00000010,
        Shield_Low = 0x00000020,
        Shield_High = 0x00000040,
        Shield_Air = 0x00000080,
        DefLess = 0x00000100,
        ThrowInv = 0x00000200,
        SuperArmor = 0x00000400,
        Dashing = 0x00000800,
    };
    
public:
    cCharacter(cObject* _owner) : cComponent(_owner) {}
    virtual ~cCharacter() = default;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    virtual void OnAnimationEnd(cCharacterAnimation* _anim);

    void OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect) override;
    void OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect) override;
    void OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect) override;
    void OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect) override;
    void OnCollisionWithCharacter(cCharacter* _with, RECT _overlappedRect) override;
    void OnCollisionWithMap(cCharacter* _with, RECT _overlappedRect) override;

    void HandleAnimationEvent(std::string _eventKey) override;
    void HandleSpriteEvent(std::string _eventKey) override;
    void HandleHurtBoxEvent(std::string _eventKey) override;
    void HandleHitBoxEvent(std::string _eventKey) override;
    void HandleThrowBoxEvent(std::string _eventKey) override;
    void HandleBodyBoxEvent(std::string _eventKey) override;

    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    cCharacterData* m_Data = nullptr;
    int m_Flag;
    State m_State;
    Vec2 m_Velocity;
    Vec2 m_Friction;
    float m_Damage;

    cCharacterAnimationPlayer* m_AnimPlayer;

    void SetAnimation(const std::string& _key) const {m_AnimPlayer->SetAnimation(m_Data->GetAnimation(_key));}
    bool CheckCurAnimation(const std::string& _key) const {return m_AnimPlayer->GetCurrentAnimation()->GetKey() == _key;}
public:
    void SetData(cCharacterData* _data);
    void SetPalette(int _index);
    void Reset();

    bool HasFlag(Flag _flag) const {return (m_Flag & (int)_flag) != 0;}
    bool AddFlag(Flag _flag) {return m_Flag |= (int)_flag;}
    bool RemoveFlag(Flag _flag) {return m_Flag &= ~(int)_flag;}
    void SetDirection(int _dir);
};
