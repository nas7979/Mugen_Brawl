#include "DXUT.h"
#include "cCharacterAnimationPlayer.h"

cCharacterAnimationPlayer::~cCharacterAnimationPlayer()
{
}

void cCharacterAnimationPlayer::Init()
{
    m_Renderer = AddComponent<cRenderer>();

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
    if (m_NextFrame != -1)
    {
        if (m_NextAnim != nullptr)
        {
            m_CurAnim = m_NextAnim;
            m_NextAnim = nullptr;
            
            for (auto& iter : m_CurAnim->GetSeparatedEventKeys())
                m_CharacterEventHandler->HandleAnimationEvent(iter.first, iter.second);
        }
        
        m_CurFrame = m_NextFrame;
        m_NextFrame = -1;
        m_FrameTimer = 0;

        if (m_CurFrame >= m_CurAnim->GetLength())
        {
            m_CurFrame -= m_CurAnim->GetLength();
            m_CharacterEventHandler->OnAnimationEnd(m_CurAnim);
        }
        
        cCharacterSprite* sprite = GetCurrentSprite();
        m_CharacterEventHandler->OnSpriteChanged(sprite);
        for (auto& iter : sprite->GetSeparatedEventKeys())
            m_CharacterEventHandler->HandleSpriteEvent(iter.first, iter.second);
    }

    if (m_CurAnim == nullptr)
        return;

    cCharacterSprite* sprite = m_CurAnim->GetSprite(m_CurFrame);

    if (m_CharacterEventHandler->GetHitStop() > 0)
        return;

    m_FrameTimer += m_Speed;
    if (m_FrameTimer >= sprite->GetFrameLength())
    {
        m_FrameTimer -= sprite->GetFrameLength();
        m_CurFrame++;

        if (m_CurFrame >= m_CurAnim->GetLength())
        {
            m_CurFrame -= m_CurAnim->GetLength();
            m_CharacterEventHandler->OnAnimationEnd(m_CurAnim);
        }

        sprite = GetCurrentSprite();
        m_CharacterEventHandler->OnSpriteChanged(sprite);
        for (auto& iter : sprite->GetSeparatedEventKeys())
            m_CharacterEventHandler->HandleSpriteEvent(iter.first, iter.second);

        m_Renderer->SetTexture(sprite->GetTexture());
        m_Renderer->SetOffset(sprite->GetOffset());
    }
}

void cCharacterAnimationPlayer::Render()
{
    float xScale = m_Owner->GetScale().x;
    float scale = m_Owner->GetScale().y;
    Texture* pixel = IMAGE->GetTexture("Pixel");
    WithBoxes([&](cSpriteBox* _box, cSpriteBoxArea::DrawType _type)->void
    {
        float left = _box->GetLeft() * xScale;
        float top = _box->GetTop() * scale;
        float right = _box->GetRight() * xScale;
        float bottom = _box->GetBottom() * scale;
        Vec2 size = Vec2(right - left, bottom - top);
        Vec3 center = m_Owner->GetPos() + Vec3((left + right) * 0.5f, (top + bottom) * 0.5f, -0.1f);
        
        IMAGE->RenderSprite(pixel, center, 0, size, Vec2(0.5f, 0.5f), cSpriteBoxArea::GetBoxColor(_type, true));
            
        IMAGE->RenderSprite(pixel, center + Vec3(0, size.y * 0.5f - scale * 0.5f, 0), 0, Vec2(size.x, scale), Vec2(0.5f, 0.5f), cSpriteBoxArea::GetBoxColor(_type, false) | 0x9f000000);
        IMAGE->RenderSprite(pixel, center - Vec3(0, size.y * 0.5f - scale * 0.5f, 0), 0, Vec2(size.x, scale), Vec2(0.5f, 0.5f), cSpriteBoxArea::GetBoxColor(_type, false) | 0x9f000000);
        IMAGE->RenderSprite(pixel, center + Vec3(size.x * 0.5f - scale * 0.5f, 0, 0), 0, Vec2(scale, size.y), Vec2(0.5f, 0.5f), cSpriteBoxArea::GetBoxColor(_type, false) | 0x9f000000);
        IMAGE->RenderSprite(pixel, center - Vec3(size.x * 0.5f - scale * 0.5f, 0, 0), 0, Vec2(scale, size.y), Vec2(0.5f, 0.5f), cSpriteBoxArea::GetBoxColor(_type, false) | 0x9f000000);
    });
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
    m_NextAnim = _anim;
    if (_resetFrame)
        SetFrame(0);
    else
        SetFrame(m_CurFrame);
}

void cCharacterAnimationPlayer::SetAnimationImmediately(cCharacterAnimation* _anim, bool _resetFrame)
{
    m_CurAnim = _anim;
    m_NextAnim = nullptr;
    
    for (auto& iter : m_CurAnim->GetSeparatedEventKeys())
        m_CharacterEventHandler->HandleAnimationEvent(iter.first, iter.second);

    if (_resetFrame)
        SetFrameImmediately(0);
    else
        SetFrameImmediately(m_CurFrame);
}

void cCharacterAnimationPlayer::SetFrame(int _frame)
{
    m_NextFrame = _frame;
}

void cCharacterAnimationPlayer::SetFrameImmediately(int _frame)
{
    m_CurFrame = _frame;
    m_NextFrame = -1;
    m_FrameTimer = 0;

    if (m_CurFrame >= m_CurAnim->GetLength())
    {
        m_CurFrame -= m_CurAnim->GetLength();
        m_CharacterEventHandler->OnAnimationEnd(m_CurAnim);
    }
        
    cCharacterSprite* sprite = GetCurrentSprite();
    for (auto& iter : sprite->GetSeparatedEventKeys())
        m_CharacterEventHandler->HandleSpriteEvent(iter.first, iter.second);

    m_CharacterEventHandler->OnSpriteChanged(sprite);

    m_Renderer->SetTexture(sprite->GetTexture());
    m_Renderer->SetOffset(sprite->GetOffset());
}

void cCharacterAnimationPlayer::UpdateRendererOnly()
{
    cCharacterAnimation* anim = m_NextAnim == nullptr ? m_CurAnim : m_NextAnim;
    cCharacterSprite* sprite = anim->GetSprite(m_NextFrame == -1 ? m_CurFrame : m_NextFrame);
    m_Renderer->SetTexture(sprite->GetTexture());
    m_Renderer->SetOffset(sprite->GetOffset());
}

void cCharacterAnimationPlayer::WithBoxes(const std::function<void(cSpriteBox*, cSpriteBoxArea::DrawType)>& _func)
{
    cCharacterSprite* sprite = GetCurrentSprite();
    cHurtBox** hurtBoxes;
    for (int i = 0; i < sprite->GetHurtBoxes(hurtBoxes); i++)
        _func(hurtBoxes[i], cSpriteBoxArea::DrawType::HurtBox);
    cHitBox** hitBoxes;
    for (int i = 0; i < sprite->GetHitBoxes(hitBoxes); i++)
        _func(hitBoxes[i], cSpriteBoxArea::DrawType::HitBox);
    cThrowBox** throwBoxes;
    for (int i = 0; i < sprite->GetThrowBoxes(throwBoxes); i++)
        _func(throwBoxes[i], cSpriteBoxArea::DrawType::ThrowBox);
    cBodyBox** bodyBoxes;
    for (int i = 0; i < sprite->GetBodyBoxes(bodyBoxes); i++)
        _func(bodyBoxes[i], cSpriteBoxArea::DrawType::BodyBox);
}
