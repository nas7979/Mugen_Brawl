#include "DXUT.h"
#include "cHitBox.h"

void cHitBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
    CopyTo(_buffer, _pointer, &m_Damage, sizeof(short) * 8 + sizeof(float) * 4);
}

void cHitBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
    if (GET_CURRENT_DATA_VERSION == 3)
    {
        CopyFrom(_buffer, _pointer, &m_Damage, sizeof(short) * 9 + sizeof(float) * 3);
    }
    else
    {
        CopyFrom(_buffer, _pointer, &m_Damage, sizeof(short) * 8 + sizeof(float) * 4);
    }
}

size_t cHitBox::GetSize() const
{
    if (GET_CURRENT_DATA_VERSION == 3)
        return cSpriteBox::GetSize() + sizeof(short) * 9 + sizeof(float) * 3;
    else
        return cSpriteBox::GetSize() + sizeof(short) * 8 + sizeof(float) * 4;
}

float cHitBox::CalculateKnockBack(cCharacter* _from, cCharacter* _to, float _damageDealt) const
{
    float baskKnockBack =  m_BaseKnockBack;
    float growthKnockBack = m_GrowthKnockBack;
    if (_to->HasFlag(cCharacter::Flag::InAir))
    {
        baskKnockBack = m_AirBaseKnockBack;
        growthKnockBack = m_AirGrowthKnockBack;
    }
    
    return baskKnockBack + (_to->GetDamage() * 0.3f + (_to->GetDamage() * 0.1f) * _damageDealt) * (sqrt(_to->GetWeight()) * 0.1f) * (growthKnockBack * 0.01f);
}

float cHitBox::CalculateDamage(cCharacter* _from, cCharacter* _to) const
{
    float multiplier = 1 + Clamp((_from->GetDamage() - 50.f) * 0.001f, 0, 0.1f);
    return m_Damage * multiplier;
}
