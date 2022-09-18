#pragma once
#include "../Data/cCharacterAnimation.h"

class cCharacterAnimation;

class cCharacterAnimationPlayer : public cComponent
{
public:
    cCharacterAnimationPlayer(cObject* _owner) : cComponent(_owner) {}
    ~cCharacterAnimationPlayer() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

private:
    cCharacterAnimation* m_CurAnim = nullptr;
    cPalette* m_Palette = nullptr;
    float m_FrameTimer = 0;
    float m_Speed = 1;
    short m_CurFrame = 0;

    cRenderer* m_Renderer;
    cCharacter* m_Character;

public:
    void SetAnimation(cCharacterAnimation* _anim, bool _resetFrame = true);
    void SetPalette(cPalette* _palette) {m_Palette = _palette;}
    cCharacterAnimation* GetCurrentAnimation() const {return m_CurAnim;}
    void SetFrame(int _frame);
    int GetCurrentFrame() const {return m_CurFrame;}
    cCharacterSprite* GetCurrentSprite() const {return m_CurAnim->GetSprite(m_CurFrame);}
    
};
