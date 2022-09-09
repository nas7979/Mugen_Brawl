#pragma once
#include "cSpriteBox.h"

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
};
