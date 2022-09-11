#include "DXUT.h"
#include "cCharacter.h"

void cCharacter::Init()
{
    if (m_Data == nullptr)
        return;

    Reset();

    m_AnimPlayer = AddComponent<cCharacterAnimationPlayer>();

    m_AnimPlayer->SetAnimation(m_Data->GetAnimation("Idle"));
}

void cCharacter::Update()
{
    if (m_State == State::Idle)
    {
        if (!HasFlag(Flag::Crouching))
        {
            if (INPUT->CheckGameInput(IngameInput::Left))
            {
                if (!(HasFlag(Flag::Dashing) ? CheckCurAnimation("Dash") : CheckCurAnimation("Walk")))
                    SetAnimation(HasFlag(Flag::Dashing) ? "Dash" : "Walk");
            
                Vec3 pos = m_Owner->GetPos();
                pos.x -= (HasFlag(Flag::Dashing) ? m_Data->GetDashSpeed() : m_Data->GetWalkSpeed()) / 60.f;
                m_Owner->SetPos(pos);
            
                SetDirection(-1);
            }

            if (INPUT->CheckGameInput(IngameInput::Right))
            {
                if (!(HasFlag(Flag::Dashing) ? CheckCurAnimation("Dash") : CheckCurAnimation("Walk")))
                    SetAnimation(HasFlag(Flag::Dashing) ? "Dash" : "Walk");
            
                Vec3 pos = m_Owner->GetPos();
                pos.x += (HasFlag(Flag::Dashing) ? m_Data->GetDashSpeed() : m_Data->GetWalkSpeed()) / 60.f;
                m_Owner->SetPos(pos);
            
                SetDirection(1);
            }
        }

        if (!HasFlag(Flag::InAir))
        {
            if (INPUT->CheckGameInput(IngameInput::Down))
            {
                RemoveFlag(Flag::Standing);
                AddFlag(Flag::Crouching);

                if (!CheckCurAnimation("Crouch") && !CheckCurAnimation("CrouchStart"))
                {
                    SetAnimation("CrouchStart");
                }
            }
            else
            {
                RemoveFlag(Flag::Crouching);
                AddFlag(Flag::Standing);

                if (CheckCurAnimation("Crouch"))
                {
                    SetAnimation("CrouchEnd");
                }
            }
        }

        if (INPUT->GetGameInput() == 0)
        {
            if (HasFlag(Flag::Standing) && !CheckCurAnimation("Idle") && !CheckCurAnimation("CrouchEnd"))
            {
                SetAnimation("Idle");
            }
            else if (HasFlag(Flag::Crouching) && !CheckCurAnimation("Crouch") && !CheckCurAnimation("CrouchStart"))
            {
                SetAnimation("Crouch");
            }
            else if (HasFlag(Flag::InAir) && !CheckCurAnimation("Jump"))
            {
                SetAnimation("Jump");
            }
        }
    }
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

void cCharacter::OnAnimationEnd(cCharacterAnimation* _anim)
{
    std::string key = _anim->GetKey();
    
    if (key == "CrouchStart")
    {
        SetAnimation("Crouch");
        return;
    }

    if (key == "CrouchEnd")
    {
        SetAnimation("Idle");
        return;
    }
}

void cCharacter::OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect)
{
}

void cCharacter::OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect)
{
}

void cCharacter::OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect)
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

void cCharacter::SetData(cCharacterData* _data)
{
    m_Data = _data;
    Init();
}

void cCharacter::SetPalette(int _index)
{
    m_AnimPlayer->SetPalette(m_Data->GetPalette(_index));
}

void cCharacter::Reset()
{
    m_Flag = 0;
    m_State = State::Idle;
    m_Damage = 0;
    m_Friction = Vec2(1, 1);
    m_Velocity = Vec2(0, 0);
}

void cCharacter::SetDirection(int _dir)
{
    m_Owner->SetScale(Vec2(_dir, 1) * m_Data->GetSpriteScale());
}
