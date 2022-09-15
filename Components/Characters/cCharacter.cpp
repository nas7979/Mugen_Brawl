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
        if (!HasFlag(Flag::InAir))
        {
            if (INPUT->CheckGameInput(IngameInput::Down))
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
        
        if (CheckInputs())
            return;
        
        bool isRightPressed = INPUT->CheckGameInput(IngameInput::Right);
        bool isLeftPressed = INPUT->CheckGameInput(IngameInput::Left);
        int prevDir = Sign(m_Owner->GetScale().x);

        if (isRightPressed && isLeftPressed)
        {
            if (INPUT->GetGameInputPressTimer(IngameInput::Right) > INPUT->GetGameInputPressTimer(IngameInput::Left))
                isRightPressed = false;
            else
                isLeftPressed = false;
        }
        else if (!isRightPressed && !isLeftPressed)
        {
            RemoveFlag(Flag::Dashing);
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
            if (isLeftPressed)
            {
                SetDirection(-1);
            }

            if (isRightPressed)
            {
                SetDirection(1);
            }
        }

        if ((!isLeftPressed && !isRightPressed) || HasFlag(Flag::Crouching))
        {
            if (HasFlag(Flag::Standing) && !CheckCurAnimation("Idle") && !CheckCurAnimation("CrouchEnd") && !CheckCurAnimation("TurnStand"))
            {
                SetAnimation("Idle");
            }
            else if (HasFlag(Flag::Crouching) && !CheckCurAnimation("Crouch") && !CheckCurAnimation("CrouchStart") && !CheckCurAnimation("TurnCrouch"))
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

    m_State = State::Idle;
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

bool cCharacter::CheckInputs()
{
    if (INPUT->GetInputBuffer().empty())
        return false;
    
    if (!HasFlag(Flag::Dashing) && INPUT->CheckInputBuffer("66", this))
    {
        AddFlag(Flag::Dashing);
        return false;
    }

    for (auto& command : m_Data->GetCommands())
    {
        if (INPUT->CheckInputBuffer(command, this))
        {
            m_State = State::Action;
            SetAnimation(command);
            return true;
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
        if (INPUT->CheckGameInput(IngameInput::Left) || INPUT->CheckGameInput(IngameInput::Right))
            normalInput[0] = '6';
        if (HasFlag(Flag::Crouching))
            normalInput[0] = '2';   
    }
    
    char lastInputBufferChar = INPUT->GetInputBuffer()[INPUT->GetInputBuffer().size() - 1];
    if (lastInputBufferChar == 'c') normalInput[1] = 'c';
    if (lastInputBufferChar == 'b') normalInput[1] = 'b';
    if (lastInputBufferChar == 'a') normalInput[1] = 'a';

    if (normalInput[1] != 0)
    {
        m_State = State::Action;
        SetAnimation(normalInput);
        if (INPUT->CheckGameInput(IngameInput::Left))
            SetDirection(-1);
        else if (INPUT->CheckGameInput(IngameInput::Right))
            SetDirection(1);
        return true;
    }
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
