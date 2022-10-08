#pragma once
#include "cCharacterEventHandler.h"
#include "CharacterAnimationPlayer/cCharacterAnimationPlayer.h"
#include "CharacterEffect/cCharacterEffect.h"
#include "Data/cCharacterAnimation.h"
#include "Data/cCharacterData.h"

class cBlock;
class cCharacterAnimation;
class cCharacterData;

class cCharacter : public cComponent, Serializer, cCharacterEventHandler
{
public:
    enum class State : short
    {
        Idle,
        Action,
        Hit,
        BlockStun,
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
        FastFall = 0x00001000,
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
    
    void OnAnimationEnd(cCharacterAnimation* _anim) override;

    void OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect) override;
    void OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect) override;
    void OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect) override;
    void OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect) override;
    void OnCollisionWithCharacter(cCharacter* _with, const RECT& _bodyRect, const RECT& _overlapped) override;
    void OnCollisionWithMap(cBlock* _with, const RECT& _bodyRect, const RECT& _overlapped) override;

    void OnSpriteChanged(cCharacterSprite* _sprite) override;
    void HandleAnimationEvent(const std::string& _key, const std::string& _value) override;
    void HandleSpriteEvent(const std::string& _key, const std::string& _value) override;
    void HandleHurtBoxEvent(const std::string& _key, const std::string& _value) override;
    void HandleHitBoxEvent(const std::string& _key, const std::string& _value) override;
    void HandleThrowBoxEvent(const std::string& _key, const std::string& _value) override;
    void HandleBodyBoxEvent(const std::string& _key, const std::string& _value) override;

    void CollisionCheck();

    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    cCharacterData* m_Data = nullptr;
    int m_Flag;
    State m_State;
    short m_HitStun;
    BYTE m_AirActionLimit;
    char m_JumpDir;
    BYTE m_PlayerIndex;
    BYTE m_Team;
    short m_HitStop;
    BYTE m_canCancel;
    BYTE m_InstantShieldTimer;
    Vec2 m_Velocity;
    Vec2 m_Friction;
    float m_Damage;
    float m_Weight;
    float m_KnockbackDecPerFrame;
    float m_Shield;
    cCharacter* m_LastAttackedBy = nullptr;
    float m_LastShieldDamage;

    cCharacterAnimationPlayer* m_AnimPlayer;

    std::vector<RECT> m_BodyBoxes;
    std::vector<RECT> m_HurtBoxes;
    std::vector<RECT> m_HitBoxes;
    std::vector<RECT> m_ThrowBoxes;
    SerializedVector<int> m_AttackedCharacters;
    SerializedVector<cCharacterEffect*> m_AttachedEffects;

    void SetAnimation(const std::string& _key) const {m_AnimPlayer->SetAnimation(m_Data->GetAnimation(_key));}
    void SetAnimationImmediately(const std::string& _key) const {m_AnimPlayer->SetAnimationImmediately(m_Data->GetAnimation(_key));}
    bool CheckCurAnimation(const std::string& _key) const {return m_AnimPlayer->GetCurrentAnimation()->GetKey() == _key;}
    bool CheckInputs(std::string* _cancelTable);
    void Jump();
    void SetState(State _state);
    void OnShieldBreak();
    
public:
    cCharacterSprite* GetCurrentSprite() const {return m_AnimPlayer->GetCurrentSprite();}
    void SetPlayerIndex(int _index) {m_PlayerIndex = _index; GAME->SetCharacter(this, _index);}
    short GetPlayerIndex() const {return m_PlayerIndex;}
    void SetTeam(BYTE _team) {m_Team = _team;}
    BYTE GetTeam() const {return m_Team;}
    float GetDamage() const {return m_Damage;}
    float GetWeight() const {return m_Weight;}
    void SetHitStop(int _hitStop) {m_HitStop = _hitStop;}
    int GetHitStop() const override {return m_HitStop;}
    
    void SetData(cCharacterData* _data);
    void SetPalette(int _index);
    void Reset();
    void AddVelocity(const Vec2& _vel, bool _reset = false);
    void RemoveAttachedEffect(cCharacterEffect* _effect);

    bool HasFlag(Flag _flag) const {return (m_Flag & (int)_flag) != 0;}
    bool AddFlag(Flag _flag) {return m_Flag |= (int)_flag;}
    bool RemoveFlag(Flag _flag) {return m_Flag &= ~(int)_flag;}
    void SetDirection(int _dir);
    void UpdateRects();
    const std::vector<RECT>& GetBodyBoxes() const {return m_BodyBoxes;}
    const std::vector<RECT>& GetHurtBoxes() const {return m_HurtBoxes;}
    const std::vector<RECT>& GetHitBoxes() const {return m_HitBoxes;}
    const std::vector<RECT>& GetThrowBoxes() const {return m_ThrowBoxes;}
};
