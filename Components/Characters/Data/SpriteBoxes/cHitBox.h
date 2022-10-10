#pragma once
#include "cSpriteBox.h"
#include "../../cCharacter.h"

enum class Guard : short
{
    Low,
    Mid,
    High,
    All,
    Unblockable,
    Throw,
    End
};

static const char* GuardToStringMap[6] = 
{
    "Low",
    "Mid",
    "High",
    "All",
    "Unblockable",
    "Throw"
};

class cHitBox : public cSpriteBox
{
public:
    cHitBox() = default;
    cHitBox(short _left, short _top, short _right, short _bottom) : cSpriteBox(_left, _top, _right, _bottom) {}
    ~cHitBox() override = default;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    unsigned short m_Damage = 0;
    short m_Direction = 0;
    short m_BaseKnockBack = 0;
    short m_GrowthKnockBack = 0;
    float m_HitStunMul = 1;
    float m_ShieldDamageMul = 1;
    float m_ShieldStunMul = 1;
    Guard m_Guard = Guard::Mid;
    short m_AirDirection = 0;
    short m_AirBaseKnockBack = 0;
    short m_AirGrowthKnockBack = 0;
    float m_AirHitStunMul = 1;

public:
    void SetDamage(unsigned short _damage) {m_Damage = _damage;}
    unsigned short GetDamage() const {return m_Damage;}
    void SetHitStunMul(float _multiplier) {m_HitStunMul = _multiplier;}
    float GetHitStunMul() const {return m_HitStunMul;}
    void SetShieldDamageMul(float _multiplier) {m_ShieldDamageMul = _multiplier;}
    float GetShieldDamageMul() const {return m_ShieldDamageMul;}
    void SetShieldStunMul(float _multiplier) {m_ShieldStunMul = _multiplier;}
    float GetShieldStunMul() const {return m_ShieldStunMul;}
    void SetDirection(short _direction) {m_Direction = _direction;}
    short GetDirection() const {return m_Direction;}
    void SetBaseKnockBack(short _knockBack) {m_BaseKnockBack = _knockBack;}
    short GetBaseKnockBack() const {return m_BaseKnockBack;}
    void SetGrowthKnockBack(short _knockBack) {m_GrowthKnockBack = _knockBack;}
    short GetGrowthKnockBack() const {return m_GrowthKnockBack;}
    void SetGuard(Guard _guard) {m_Guard = _guard;}
    Guard GetGuard() const {return m_Guard;}
    void SetAirDirection(short _direction) {m_AirDirection = _direction;}
    short GetAirDirection() const {return m_AirDirection;}
    void SetAirBaseKnockBack(short _knockBack) {m_AirBaseKnockBack = _knockBack;}
    short GetAirBaseKnockBack() const {return m_AirBaseKnockBack;}
    void SetAirGrowthKnockBack(short _knockBack) {m_AirGrowthKnockBack = _knockBack;}
    short GetAirGrowthKnockBack() const {return m_AirGrowthKnockBack;}
    void SetAirHitStunMul(float _multiplier) {m_AirHitStunMul = _multiplier;}
    float GetAirHitStunMul() const {return m_AirHitStunMul;}

    float CalculateKnockBack(cCharacter* _from, cCharacter* _to, float _damageDealt) const;
    float CalculateDamage(cCharacter* _from, cCharacter* _to) const;
};
