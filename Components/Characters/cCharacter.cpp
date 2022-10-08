﻿#include "DXUT.h"
#include "cCharacter.h"

#include "../Maps/cBlock.h"

void cCharacter::Init()
{
    if (m_Data == nullptr)
        return;
    
    m_AnimPlayer = AddComponent<cCharacterAnimationPlayer>();

    m_AnimPlayer->SetCharacterEventHandler(this);
    m_AnimPlayer->SetAnimationImmediately(m_Data->GetAnimation("Idle"));

    Reset();
}

void cCharacter::Update()
{
    if (m_HitStop > 0)
    {
        m_HitStop--;
        for (auto& iter : *m_AttachedEffects.GetValue())
        {
            iter->SetHitStop(m_HitStop);
        }
        return;
    }

    if (m_canCancel && GetCurrentSprite()->FindEventKey("CA"))
    {
        std::string cancelTable;
        if (m_AnimPlayer->GetCurrentAnimation()->FindEventKey("CancelInto", &cancelTable))
        {
            if (CheckInputs(&cancelTable))
            {
                m_AttackedCharacters.GetValue()->clear();
                m_canCancel = false;
                return;
            }
        }
    }
    
    if (HasFlag(Flag::InAir))
    {
        m_Velocity.y = min(m_Velocity.y + m_Weight / 60.f, (HasFlag(Flag::FastFall) ? m_Data->GetFastFallSpeed() : m_Data->GetFallSpeed()) / 60.f);
        if (m_State == State::Idle)
        {
            short fallSpeed = m_Data->GetFallSpeed();
            if (m_Velocity.y >= fallSpeed / 60.f)
            {
                if (!CheckCurAnimation("Fall"))
                    SetAnimationImmediately("Fall");
            }
            else
            {
                if (!CheckCurAnimation("Jump"))
                    SetAnimationImmediately("Jump");
                float jumpAnimLength = m_AnimPlayer->GetCurrentAnimation()->GetLength();
                float velPerFrame = fallSpeed / 30.f / jumpAnimLength;
                m_AnimPlayer->SetFrameImmediately(max(0, round(jumpAnimLength * 0.5f + Clamp(m_Velocity.y, -fallSpeed / 60.f, fallSpeed / 60.f) / velPerFrame) - 1));
            }
        }
    }
    else
    {
        m_Velocity.y = 0;
    }
    
    if (m_State == State::Idle)
    {
        bool isRightPressed = INPUT->CheckGameInput(IngameInput::Right, m_PlayerIndex);
        bool isLeftPressed = INPUT->CheckGameInput(IngameInput::Left, m_PlayerIndex);
        int prevDir = Sign(m_Owner->GetScale().x);

        if (isRightPressed && isLeftPressed)
        {
            if (INPUT->GetGameInputPressTimer(IngameInput::Right, m_PlayerIndex) > INPUT->GetGameInputPressTimer(IngameInput::Left, m_PlayerIndex))
                isRightPressed = false;
            else
                isLeftPressed = false;
        }
        else if (!isRightPressed && !isLeftPressed)
        {
            RemoveFlag(Flag::Dashing);
        }

        if (isLeftPressed)
        {
            SetDirection(-1);
        }

        if (isRightPressed)
        {
            SetDirection(1);
        }
        
        if (!HasFlag(Flag::InAir))
        {
            if (INPUT->CheckGameInput(IngameInput::Down, m_PlayerIndex))
            {
                if (HasFlag(Flag::Standing))
                {
                    SetAnimation("CrouchStart");
                }
                
                RemoveFlag(Flag::Standing);
                AddFlag(Flag::Crouching);
                RemoveFlag(Flag::Dashing);
            }
            else
            {
                if (HasFlag(Flag::Crouching))
                {
                    SetAnimation("CrouchEnd");
                }
                
                RemoveFlag(Flag::Crouching);
                AddFlag(Flag::Standing);
            }
        }
        
        
        if (CheckInputs(nullptr))
        {
            return;
        }

        RemoveFlag(Flag::Shield_High);
        RemoveFlag(Flag::Shield_Mid);
        RemoveFlag(Flag::Shield_Low);
        RemoveFlag(Flag::Shield_Air);
        if (INPUT->CheckGameInput(IngameInput::Shield, m_PlayerIndex) && m_Shield > 0)
        {
            if (HasFlag(Flag::InAir))
                AddFlag(Flag::Shield_Air);
            else if (INPUT->CheckGameInput(IngameInput::Up, m_PlayerIndex))
                AddFlag(Flag::Shield_High);
            else if (HasFlag(Flag::Crouching))
                AddFlag(Flag::Shield_Low);
            else
                AddFlag(Flag::Shield_Mid);

            isLeftPressed = isRightPressed = false;
            m_Shield -= m_Data->GetShieldDecSpeed() / 60.f;
        }
        else
        {
            m_Shield = min(m_Shield + m_Data->GetShieldDecSpeed() / 120.f, m_Data->GetShieldSize());
        }
        
        if (HasFlag(Flag::Standing))
        {
            if (isLeftPressed)
            {
                SetDirection(-1);
                if (!(HasFlag(Flag::Dashing) ? CheckCurAnimation("Dash") : CheckCurAnimation("Walk")) && !CheckCurAnimation("TurnStand"))
                    SetAnimation(HasFlag(Flag::Dashing) ? "Dash" : "Walk");
            
                Vec3 pos = m_Owner->GetPos();
                pos.x -= (HasFlag(Flag::Dashing) ? m_Data->GetDashSpeed() : m_Data->GetWalkSpeed()) / 60.f;
                m_Owner->SetPos(pos);
            }

            if (isRightPressed)
            {
                SetDirection(1);
                if (!(HasFlag(Flag::Dashing) ? CheckCurAnimation("Dash") : CheckCurAnimation("Walk")) && !CheckCurAnimation("TurnStand"))
                    SetAnimation(HasFlag(Flag::Dashing) ? "Dash" : "Walk");
                
                Vec3 pos = m_Owner->GetPos();
                pos.x += (HasFlag(Flag::Dashing) ? m_Data->GetDashSpeed() : m_Data->GetWalkSpeed()) / 60.f;
                m_Owner->SetPos(pos);
            }

            if (prevDir != Sign(m_Owner->GetScale().x))
            {
                RemoveFlag(Flag::Dashing);
                SetAnimation("TurnStand");
            }
        }
        else if (HasFlag(Flag::Crouching))
        {
        }

        if ((!isLeftPressed && !isRightPressed) || HasFlag(Flag::Crouching))
        {
            if (HasFlag(Flag::Standing) && !CheckCurAnimation("Idle") && !CheckCurAnimation("CrouchEnd") && !CheckCurAnimation("TurnStand"))
            {
                SetAnimationImmediately("Idle");
            }
            else if (HasFlag(Flag::Crouching) && !CheckCurAnimation("Crouch") && !CheckCurAnimation("CrouchStart") && !CheckCurAnimation("TurnCrouch"))
            {
                SetAnimationImmediately("Crouch");
            }
        }
    }
    else if (m_State == State::Hit || m_State == State::BlockStun)
    {
        if (m_HitStun <= 0)
        {
            m_HitStun = 0;
            SetState(State::Idle);
        }
        else
        {
            m_HitStun--;

            if (m_State == State::Hit)
            {
                m_Velocity.x -= Sign(m_Velocity.x) * m_KnockbackDecPerFrame;

                if (HasFlag(Flag::InAir))
                {
                    Vec2 velocityDir = m_Velocity;
                    velocityDir.y -= 1;
                    m_Owner->SetRot(D3DXToDegree(atan2(velocityDir.y, velocityDir.x)) - (m_Owner->GetScale().x > 0 ? 180 : 0));
                }
                else
                {
                    if (abs(m_Velocity.x) <= 0.1f)
                    {
                        m_Velocity.x = 0;
                    }
                }   
            }
        }
    }
    
    cCharacterSprite* curSprite = m_AnimPlayer->GetCurrentSprite();
    m_Velocity.x *= curSprite->GetFriction().x;
    m_Velocity.y *= curSprite->GetFriction().y;
    
    m_Owner->SetPos(m_Owner->GetPos() + Vec3(m_Velocity.x, m_Velocity.y, 0));

    for (auto& iter : *m_AttachedEffects.GetValue())
    {
        iter->GetOwner()->SetPos(m_Owner->GetPos());
    }
}

void cCharacter::Render()
{
    if (m_HitStop > 0)
    {
        Vec2 originOffset = GetCurrentSprite()->GetOffset();
        if (m_State == State::Hit)
        {
            originOffset += Vec2(Random(-1.f, 1.f), Random(-1.f, 1.f)) * (pow(min(m_HitStop, 15) * 0.8f, 1.75) - 25);
        }
        else if (m_State == State::BlockStun)
        {
            originOffset += Vec2(Random(-0.5f, 0.5f), Random(-0.5f, 0.5f)) * (pow(min(m_HitStop, 15) * 0.8f, 1.75) - 25);
        }
        GetComponent<cRenderer>()->SetOffset(originOffset);
    }

    if (HasFlag(Flag::Shield_Air) || HasFlag(Flag::Shield_High) || HasFlag(Flag::Shield_Mid) || HasFlag(Flag::Shield_Low))
    {
        Vec2 offset = m_AnimPlayer->GetCurrentAnimation()->GetSprite(0)->GetOffset();
        offset.x *= m_Owner->GetScale().x;
        offset.y *= m_Owner->GetScale().y;
        IMAGE->RenderSprite(IMAGE->GetTexture("Shield"), m_Owner->GetPos() + Vec3(offset.x, offset.y, -0.1), 0, Vec2(1, 1) * m_Shield / 160 * m_Data->GetSpriteScale());
    }
}

void cCharacter::Release()
{
    GAME->SetCharacter(nullptr, m_PlayerIndex);
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
    
    if (key == "CrouchStart" || key == "TurnCrouch")
    {
        SetAnimation("Crouch");
        return;
    }

    if (key == "CrouchEnd" || key == "TurnStand")
    {
        SetAnimation("Idle");
        return;
    }

    if (m_State == State::Idle || key == "Hit_Ground")
        return;

    if (key == "PreJump")
    {
        Jump();
        return;
    }

    SetState(State::Idle);
    m_AttackedCharacters.GetValue()->clear();
}

void cCharacter::OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect)
{
    Flag shieldFlag;
    switch (_enemyHitBox->GetGuard())
    {
    case Guard::High: shieldFlag = Flag::Shield_High; break;
    case Guard::Mid: shieldFlag = Flag::Shield_Mid; break;
    case Guard::Low: shieldFlag = Flag::Shield_Low; break;
    case Guard::Unblockable: shieldFlag = (Flag)0; break;
    case Guard::All: shieldFlag = (Flag)0xffffffff; break;
    }

    float shieldDamageMul = _enemyHitBox->GetShieldDamageMul() * 2;
    bool isShielded = false;
    if (HasFlag(shieldFlag))
    {
        isShielded = true;
    }
    else if (HasFlag((Flag)((int)Flag::Shield_High | (int)Flag::Shield_Mid | (int)Flag::Shield_Low | (int)Flag::Shield_Air)))
    {
        isShielded = true;
        shieldDamageMul *= 2;
    }
    
    int xDir = Sign(_by->m_Owner->GetScale().x);
    SetDirection(-xDir);
    
    float damage = _enemyHitBox->CalculateDamage(_by, this);

    bool isInAir = HasFlag(Flag::InAir);
    float radDir = D3DXToRadian(isInAir ? _enemyHitBox->GetAirDirection() : _enemyHitBox->GetDirection());
    
    Vec2 dirVec = Vec2(cos(radDir) * xDir, sin(radDir));
    float knockBack = _enemyHitBox->CalculateKnockBack(_by, this, damage);
    
    m_HitStun = 8 + ceil(knockBack * 0.015f * (isInAir ? _enemyHitBox->GetAirHitStunMul() : _enemyHitBox->GetHitStunMul()));
    
    std::string fixedHitStop;
    _enemyHitBox->FindEventKey("HitStop", &fixedHitStop);
    m_HitStop = fixedHitStop.empty() ? min(8 + ceil(sqrt(pow(_enemyHitBox->GetDamage(), 1.75f)) * 0.5f), 16) : atoi(fixedHitStop.c_str());
    _by->SetHitStop(m_HitStop);

    if (isShielded)
    {
        m_Shield -= damage * shieldDamageMul;
        m_HitStun = ceil(3 + damage * 0.5f * _enemyHitBox->GetShieldStunMul());
        SetState(State::BlockStun);
        
        if (m_Shield < 0)
        {
            OnShieldBreak();
        }
        return;
    }

    m_Damage += damage;

    m_KnockbackDecPerFrame = (knockBack * abs(dirVec.x) / (m_HitStun * (m_HitStun * 0.5f))) * (dirVec.y != 0 || HasFlag(Flag::InAir) ? 0.5f : 1);
    
    AddVelocity(Vec2(dirVec.x * knockBack / (m_HitStun * 0.5f), dirVec.y * sqrt(knockBack) * 2), true);

    SetState(State::Hit);
}

void cCharacter::OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect)
{
    m_canCancel = true;
    m_AttackedCharacters.GetValue()->push_back(_to->GetOwner()->GetUID());
}

void cCharacter::OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect)
{
}

void cCharacter::OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect)
{
}

void cCharacter::OnCollisionWithCharacter(cCharacter* _with, const RECT& _bodyRect, const RECT& _overlapped)
{
    float collDirX = Sign(m_Owner->GetPos().x - _with->GetOwner()->GetPos().x);
    
    m_Owner->SetPos(m_Owner->GetPos() + Vec3((_overlapped.right - _overlapped.left + 2) * collDirX, 0, 0) * 0.5f);
    _with->GetOwner()->SetPos(_with->GetOwner()->GetPos() + Vec3((_overlapped.right - _overlapped.left + 2) * -collDirX, 0, 0) * 0.5f);

    UpdateRects();
    _with->UpdateRects();
}

void cCharacter::OnCollisionWithMap(cBlock* _with, const RECT& _bodyRect, const RECT& _overlapped)
{
    Vec2 prevPos = Vec2(m_Owner->GetPos().x - m_Velocity.x, m_Owner->GetPos().y - m_Velocity.y);
    Vec2 nearestPosToChar = Vec2(Clamp((int)prevPos.x, _with->GetRect().left, _with->GetRect().right), Clamp((int)prevPos.y, _with->GetRect().top, _with->GetRect().bottom));
    Vec2 nearestPosToBlock = Vec2(Clamp(nearestPosToChar.x, _bodyRect.left - m_Velocity.x, _bodyRect.right - m_Velocity.x),
        Clamp(nearestPosToChar.y, _bodyRect.top - m_Velocity.y, _bodyRect.bottom - m_Velocity.y));
    Vec2 collDir;
    D3DXVec2Normalize(&collDir, &(nearestPosToChar - nearestPosToBlock));
    if (collDir.x == 0 && collDir.y == 0)
        collDir.y = 1;

    Vec2 bodyBoxOutline = Vec2( collDir.x == 0 ? 0 : (collDir.x < 0 ? _bodyRect.left : _bodyRect.right), collDir.y == 0 ? 0 : (collDir.y < 0 ? _bodyRect.top : _bodyRect.bottom));
    
    m_Owner->SetPos(m_Owner->GetPos() + Vec3((nearestPosToChar.x - bodyBoxOutline.x) * collDir.x, (nearestPosToChar.y - bodyBoxOutline.y) * collDir.y, 0));
    if (_overlapped.bottom - _overlapped.top > 0)
    {
        RemoveFlag(Flag::InAir);
        RemoveFlag(Flag::Dashing);
        m_Velocity = Vec2(0, 0);
        SetAnimation("Land");
        SetState(State::Action);
        m_AirActionLimit = m_Data->GetAirMovementLimit();
    }
}

void cCharacter::OnSpriteChanged(cCharacterSprite* _sprite)
{
    AddVelocity(Vec2(_sprite->GetMomentum().x / 60.f * Sign(m_Owner->GetScale().x), _sprite->GetMomentum().y / 60.f));
}

void cCharacter::HandleAnimationEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacter::HandleSpriteEvent(const std::string& _key, const std::string& _value)
{
    if (_key == "Sound")
    {
        cSound* sound = m_Data->GetSoundSet(_value)->PickSound();
        SOUND->Play(sound->GetSound(), (int)sound->GetVolume());
        return;
    }

    if (_key == "ResetHit")
    {
        m_AttackedCharacters.GetValue()->clear();
        return;
    }

    if (_key == "fx")
    {
        cCharacterEffect* effect = OBJECT->AddObject<cCharacterEffect>("fx", m_Owner->GetPos(), Obj_Effect);
        effect->GetOwner()->SetScale(m_Owner->GetScale());
        cCharacterAnimation* anim = m_Data->GetAnimation(_value);
        effect->SetAnimation(m_Data, anim);
        effect->SetFrom(this);

        if (anim->FindEventKey("Attach"))
            m_AttachedEffects.GetValue()->push_back(effect);
        return;
    }
}

void cCharacter::HandleHurtBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacter::HandleHitBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacter::HandleThrowBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacter::HandleBodyBoxEvent(const std::string& _key, const std::string& _value)
{
}

void cCharacter::CollisionCheck()
{
    RECT overlapped;
    cCharacterSprite* curSprite = GetCurrentSprite();

    for (auto& iter : OBJECT->GetObjects(Obj_Character))
    {
        cCharacter* other = iter->GetComponent<cCharacter>();
        if (m_Team == other->GetTeam())
            continue;
        
        if (IntersectRect(&overlapped, &m_BodyBoxes[0], &other->GetBodyBoxes()[0]))
            OnCollisionWithCharacter(other, m_BodyBoxes[0], overlapped);

        if (!Contains<int>(*m_AttackedCharacters.GetValue(), iter->GetUID()))
        {
            for (int i = 0; i < m_HitBoxes.size(); i++)
            {
                for (int j = 0; j < other->GetHurtBoxes().size(); j++)
                {
                    if (IntersectRect(&overlapped, &m_HitBoxes[i], &other->GetHurtBoxes()[j]))
                    {
                        OnHit(other, other->GetCurrentSprite()->GetHurtBox(j), curSprite->GetHitBox(i), overlapped);
                        other->OnHurt(this, other->GetCurrentSprite()->GetHurtBox(j), curSprite->GetHitBox(i), overlapped);
                        goto out;
                    }
                }
            }
            out:
            continue;
        }
    }

    if (m_HitStop <= 0)
    {
        cBodyBox** bodyBoxes;
        int bodyBoxCount = curSprite->GetBodyBoxes(bodyBoxes);
        RECT bodyRectForGround = m_BodyBoxes[0];
        bodyRectForGround.bottom -= bodyBoxes[0]->GetBottom();

        for (auto& iter : OBJECT->GetObjects(Obj_Map))
        {
            cBlock* block = iter->GetComponent<cBlock>();
            if (IntersectRect(&overlapped, &bodyRectForGround, &block->GetRect()))
                OnCollisionWithMap(block, bodyRectForGround, overlapped);
        }   
    }
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

bool cCharacter::CheckInputs(std::string* _cancelTable)
{
    if (INPUT->GetInputBufferSize(m_PlayerIndex) == 0)
        return false;

    if (_cancelTable == nullptr || _cancelTable->find("SP") != std::string::npos)
    {
        for (auto& command : m_Data->GetCommands())
        {
            if (!m_Data->GetAnimation(command)->FindEventKey(HasFlag(Flag::InAir) ? "Air" : "Ground"))
                continue;
            
            if (INPUT->CheckInputBuffer(command[0] == 'j' ? command.substr(1) : command, this))
            {
                SetState(State::Action);
                RemoveFlag(Flag::Dashing);
                SetAnimation(command);

                cSoundSet* soundSet = m_Data->GetSoundSet(command);
                if (soundSet != nullptr)
                {
                    cSound* sound = soundSet->PickSound();
                    SOUND->Play(sound->GetSound(), sound->GetVolume());
                }

                INPUT->ClearInputBuffer(m_PlayerIndex);
                return true;
            }
        }
    }

    char normalInput[3] = {0, };
    if (HasFlag(Flag::InAir))
    {
        normalInput[0] = 'j';
    }
    else
    {
        normalInput[0] = '5';
        // if (INPUT->CheckGameInput(IngameInput::Left, m_PlayerIndex) || INPUT->CheckGameInput(IngameInput::Right, m_PlayerIndex))
        //     normalInput[0] = '6';
        if (INPUT->CheckGameInput(IngameInput::Down, m_PlayerIndex))
            normalInput[0] = '2';   
    }
    
    IngameInput lastInputBufferChar = INPUT->GetLastBufferedInput(m_PlayerIndex);
    if (lastInputBufferChar == IngameInput::C) normalInput[1] = 'c';
    if (lastInputBufferChar == IngameInput::B) normalInput[1] = 'b';
    if (lastInputBufferChar == IngameInput::A) normalInput[1] = 'a';

    if (normalInput[1] != 0 && (_cancelTable == nullptr || _cancelTable->find(normalInput) != std::string::npos))
    {
        SetState(State::Action);
        SetAnimation(normalInput);
        RemoveFlag(Flag::Dashing);
        if (INPUT->CheckGameInput(IngameInput::Left, m_PlayerIndex))
            SetDirection(-1);
        else if (INPUT->CheckGameInput(IngameInput::Right, m_PlayerIndex))
            SetDirection(1);

        char attackSound[] = "Attack_A";
        attackSound[7] = normalInput[1] + ('A' - 'a');
        cSound* sound = m_Data->GetSoundSet(attackSound)->PickSound();
        SOUND->Play(sound->GetSound(), sound->GetVolume());

        INPUT->ClearInputBuffer(m_PlayerIndex);
        return true;
    }

    if (_cancelTable == nullptr || _cancelTable->find("Dash") != std::string::npos)
    {
        if (!HasFlag(Flag::Dashing) && INPUT->CheckInputBuffer("66", this))
        {
            if (HasFlag(Flag::InAir))
            {
                if (m_AirActionLimit > 0)
                {
                    m_AirActionLimit--;
                    SetAnimation("AirDash");
                    SetState(State::Action);
                    AddVelocity(Vec2(35 * Sign(m_Owner->GetScale().x), 0), true);
                    return false;
                }
            }
            else
            {
                AddFlag(Flag::Dashing);
                if (m_State != State::Idle)
                    SetState(State::Idle);

                return false;
            }
        }
    }

    if (m_AirActionLimit > 0 && (_cancelTable == nullptr || _cancelTable->find("Jump") != std::string::npos))
    {
        if (INPUT->CheckInputBuffer("8", this))
        {
            m_AirActionLimit--;
            m_JumpDir = INPUT->CheckGameInput(IngameInput::Left, m_PlayerIndex) ? -1 : INPUT->CheckGameInput(IngameInput::Right, m_PlayerIndex) ? 1 : 0;
            if (!HasFlag(Flag::InAir))
            {
                SetAnimation("PreJump");
                SetState(State::Action);
            }
            else
            {
                Jump();
            }

            INPUT->RemoveInputBuffer(IngameInput::Up, m_PlayerIndex);
            if (HasFlag(Flag::Dashing) && INPUT->CheckInputBuffer("66", this))
                INPUT->RemoveInputBuffer(0, m_PlayerIndex);
            return true;
        }
    }

    return false;
}

void cCharacter::Jump()
{
    m_Velocity.x = (HasFlag(Flag::Dashing) ? m_Data->GetDashSpeed() : m_Data->GetWalkSpeed()) * m_JumpDir / 60.f;
    m_Velocity.y = -m_Data->GetJumpHeight();
    SetAnimation("Jump");
    RemoveFlag(Flag::Standing);
    RemoveFlag(Flag::Crouching);
    RemoveFlag(Flag::Dashing);
    RemoveFlag(Flag::FastFall);
    AddFlag(Flag::InAir);
    SetState(State::Idle);
}

void cCharacter::SetState(State _state)
{
    State prevState = m_State;
    switch (_state)
    {
    case State::Idle:
        {
            RemoveFlag(Flag::Dashing);
            m_Owner->SetRot(0);

            if (HasFlag(Flag::InAir))
            {
                short fallSpeed = m_Data->GetFallSpeed();
                if (m_Velocity.y >= fallSpeed / 60.f)
                {
                    if (!CheckCurAnimation("Fall"))
                        SetAnimationImmediately("Fall");
                }
                else
                {
                    if (!CheckCurAnimation("Jump"))
                        SetAnimationImmediately("Jump");
                    float jumpAnimLength = m_AnimPlayer->GetCurrentAnimation()->GetLength();
                    float velPerFrame = fallSpeed / 30.f / jumpAnimLength;
                    m_AnimPlayer->SetFrameImmediately(max(0, round(jumpAnimLength * 0.5f + Clamp(m_Velocity.y, -fallSpeed / 60.f, fallSpeed / 60.f) / velPerFrame) - 1));
                }
            }
            else
            {
                if (HasFlag(Flag::Crouching))
                {
                    SetAnimationImmediately("Crouch");
                }
                else
                {
                    SetAnimationImmediately("Idle");
                }
            }
            break;
        }

    case State::Action:
        {
            break;
        }
        
    case State::Hit:
        {
            RemoveFlag(Flag::Dashing);
            if (HasFlag(Flag::InAir))
            {
                SetAnimation("Hit_Air");
                Vec2 velocityDir = m_Velocity;
                velocityDir.y -= 1;
                m_Owner->SetRot(D3DXToDegree(atan2(velocityDir.y, velocityDir.x)) - (m_Owner->GetScale().x > 0 ? 180 : 0));
            }
            else
            {
                SetAnimation("Hit_Ground");
            }

            for (auto& iter : *m_AttachedEffects.GetValue())
            {
                iter->GetOwner()->Destroy();
            }
            m_AttachedEffects.GetValue()->clear();
            break;
        }
    }

    m_AttackedCharacters.GetValue()->clear();
    m_canCancel = false;
    m_State = _state;
}

void cCharacter::OnShieldBreak()
{
    m_Shield = m_Data->GetShieldSize();
    SetState(State::Hit);
    AddVelocity(Vec2(0, -35), true);
    m_HitStun = 100;
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
    SetState(State::Idle);
    m_Damage = 0;
    m_Friction = Vec2(1, 1);
    m_Velocity = Vec2(0, 0);
    m_AirActionLimit = m_Data->GetAirMovementLimit();
    m_Weight = m_Data->GetWeight();
    m_HitStun = 0;
    m_KnockbackDecPerFrame = 0;
    m_HitStop = 0;
    m_canCancel = false;
    m_Shield = m_Data->GetShieldSize();
}

void cCharacter::AddVelocity(const Vec2& _vel, bool _reset)
{
    if (_reset)
        m_Velocity = Vec2(0, 0);
    
    if (m_Velocity.y == 0 && _vel.y != 0)
    {
        RemoveFlag(Flag::Standing);
        RemoveFlag(Flag::Crouching);
        RemoveFlag(Flag::Dashing);
        RemoveFlag(Flag::FastFall);
        AddFlag(Flag::InAir);
    }

    m_Velocity += _vel;
}

void cCharacter::RemoveAttachedEffect(cCharacterEffect* _effect)
{
    for (auto& iter = m_AttachedEffects.GetValue()->begin(); iter != m_AttachedEffects.GetValue()->end();)
    {
        if ((*iter) == _effect)
        {
            m_AttachedEffects.GetValue()->erase(iter);
            return;
        }
    }
}

void cCharacter::SetDirection(int _dir)
{
    int prevDir = Sign(m_Owner->GetScale().x);
    m_Owner->SetScale(Vec2(_dir, 1) * m_Data->GetSpriteScale());
    if (m_State == State::Idle && prevDir != _dir)
    {
        if (HasFlag(Flag::Standing) && (CheckCurAnimation("Idle") || CheckCurAnimation("Walk") || CheckCurAnimation("Dash")))
            SetAnimation("TurnStand");
        else if (HasFlag(Flag::Crouching) && CheckCurAnimation("Crouch"))
            SetAnimation("TurnCrouch");
    }
}

void cCharacter::UpdateRects()
{
    Vec3 pos = m_Owner->GetPos();
    Vec2 scale = m_Owner->GetScale();
    std::function<void(cSpriteBox*, std::vector<RECT>&)> updateFunc = [&pos, &scale](cSpriteBox* _box, std::vector<RECT>& _vec)->void
    {
        RECT rect = scale.x > 0
        ? RECT {
            (LONG)(pos.x + _box->GetLeft() * scale.x),
            (LONG)(pos.y + _box->GetTop() * scale.y),
            (LONG)(pos.x + _box->GetRight() * scale.x),
            (LONG)(pos.y + _box->GetBottom() * scale.y)
        }
        : RECT {
            (LONG)(pos.x + _box->GetRight() * scale.x),
            (LONG)(pos.y + _box->GetTop() * scale.y),
            (LONG)(pos.x + _box->GetLeft() * scale.x),
            (LONG)(pos.y + _box->GetBottom() * scale.y)
        };

        _vec.push_back(rect);
    };

    m_HurtBoxes.clear();
    m_HitBoxes.clear();
    m_ThrowBoxes.clear();
    m_BodyBoxes.clear();
    
    cCharacterSprite* curSprite = m_AnimPlayer->GetCurrentSprite();
    cHurtBox** hurtBoxes;
    for (int i = 0; i < curSprite->GetHurtBoxes(hurtBoxes); i++)
        updateFunc(hurtBoxes[i], m_HurtBoxes);
    cHitBox** hitBoxes;
    for (int i = 0; i < curSprite->GetHitBoxes(hitBoxes); i++)
        updateFunc(hitBoxes[i], m_HitBoxes);
    cThrowBox** throwBoxes;
    for (int i = 0; i < curSprite->GetThrowBoxes(throwBoxes); i++)
        updateFunc(throwBoxes[i], m_ThrowBoxes);
    cBodyBox** bodyBoxes;
    for (int i = 0; i < curSprite->GetBodyBoxes(bodyBoxes); i++)
        updateFunc(bodyBoxes[i], m_BodyBoxes);
}