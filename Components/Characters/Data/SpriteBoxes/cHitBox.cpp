#include "DXUT.h"
#include "cHitBox.h"

void cHitBox::Serialize(char* _buffer, UINT& _pointer) const
{
    cSpriteBox::Serialize(_buffer, _pointer);
    if (GET_CURRENT_DATA_VERSION == 1)
    {
        CopyTo(_buffer, _pointer, &m_Damage, sizeof(short) * 4 + sizeof(float) * 3);
    }
    else
    {
        CopyTo(_buffer, _pointer, &m_Damage, sizeof(short) * 5 + sizeof(float) * 3);
    }
}

void cHitBox::Deserialize(char* _buffer, UINT& _pointer)
{
    cSpriteBox::Deserialize(_buffer, _pointer);
    if (GET_CURRENT_DATA_VERSION == 1)
    {
        CopyFrom(_buffer, _pointer, &m_Damage, sizeof(short) * 4 + sizeof(float) * 3);
    }
    else
    {
        CopyFrom(_buffer, _pointer, &m_Damage, sizeof(short) * 5 + sizeof(float) * 3);
    }
}

size_t cHitBox::GetSize() const
{
    if (GET_CURRENT_DATA_VERSION == 1)
        return cSpriteBox::GetSize() + sizeof(short) * 4 + sizeof(float) * 3;
    else
        return cSpriteBox::GetSize() + sizeof(short) * 5 + sizeof(float) * 3;
}

float cHitBox::CalculateKnockback(cCharacter* _from, cCharacter* _to, float _damageDealt) const
{
    return m_BaseKnockBack + (_to->GetDamage() * 0.3f + (_to->GetDamage() * 0.1f) * _damageDealt) * (sqrt(_to->GetWeight()) * 0.1f) * (m_GrowthKnockBack * 0.01f);
}

float cHitBox::CalculateDamage(cCharacter* _from, cCharacter* _to) const
{
    float multiplier = 1 + Clamp((_from->GetDamage() - 50.f) * 0.001f, 0, 0.1f);
    return m_Damage * multiplier;
}
