#include "DXUT.h"
#include "cCharacterAnimationPlayer.h"

cCharacterAnimationPlayer::~cCharacterAnimationPlayer()
{
}

void cCharacterAnimationPlayer::Init()
{
    m_Renderer = AddComponent<cRenderer>();
    m_Character = GetComponent<cCharacter>();

    m_Renderer->SetShaderBeginFunc([&]()->void
    {
        IMAGE->SetPaletteSwapShaderParams(m_Palette, 255);
        IMAGE->BeginPaletteSwapShader();
    });

    m_Renderer->SetShaderEndFunc([&]()->void
    {
        IMAGE->EndPaletteSwapShader();
    });
}

void cCharacterAnimationPlayer::Update()
{
    if (m_CurAnim == nullptr)
        return;

    cCharacterSprite* sprite = m_CurAnim->GetSprite(m_CurFrame);
    m_Renderer->SetTexture(sprite->GetTexture());
    m_Renderer->SetOffset(sprite->GetOffset());

    m_FrameTimer += m_Speed;
    if (m_FrameTimer >= sprite->GetFrameLength())
    {
        for (auto& iter : sprite->GetSeparatedEventKeys())
            m_Character->HandleSpriteEvent(iter.first, iter.second);
        m_FrameTimer -= sprite->GetFrameLength();
        m_CurFrame++;

        if (m_CurFrame >= m_CurAnim->GetLength())
        {
            m_CurFrame -= m_CurAnim->GetLength();
            m_Character->OnAnimationEnd(m_CurAnim);
        }
    }
}

void cCharacterAnimationPlayer::Render()
{
}

void cCharacterAnimationPlayer::Release()
{
}

void cCharacterAnimationPlayer::OnCollision(cObject* _other)
{
}

void cCharacterAnimationPlayer::OnAlarm(std::string _key)
{
}

void cCharacterAnimationPlayer::SetAnimation(cCharacterAnimation* _anim, bool _resetFrame)
{
    m_CurAnim = _anim;
    if (_resetFrame)
        SetFrame(0);

    for (auto& iter : _anim->GetSprite(0)->GetSeparatedEventKeys())
        m_Character->HandleSpriteEvent(iter.first, iter.second);
    for (auto& iter : _anim->GetSeparatedEventKeys())
        m_Character->HandleAnimationEvent(iter.first, iter.second);
}

void cCharacterAnimationPlayer::SetFrame(int _frame)
{
    m_CurFrame = _frame;
    m_FrameTimer = 0;
}
