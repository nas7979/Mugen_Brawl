#pragma once
#include "../Data/cCharacterAnimation.h"
#include "../../Components/GUI/cSpriteBoxArea.h"
#include "../Characters/Data/SpriteBoxes/cSpriteBox.h"

class cCharacterAnimation;
class cSpriteBoxArea;

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
    cCharacterAnimation* m_NextAnim = nullptr;
    cPalette* m_Palette = nullptr;
    float m_FrameTimer = 0;
    float m_Speed = 1;
    short m_CurFrame = 0;
    short m_NextFrame = -1;

    cRenderer* m_Renderer;
    cCharacterEventHandler* m_CharacterEventHandler;

    void WithBoxes(const std::function<void(cSpriteBox*, cSpriteBoxArea::DrawType)>& _func);

public:
    void SetCharacterEventHandler(cCharacterEventHandler* _handler) {m_CharacterEventHandler = _handler;}
    void SetAnimation(cCharacterAnimation* _anim, bool _resetFrame = true);
    void SetAnimationImmediately(cCharacterAnimation* _anim, bool _resetFrame = true);
    void SetPalette(cPalette* _palette) {m_Palette = _palette;}
    cCharacterAnimation* GetCurrentAnimation() const {return m_CurAnim;}
    void SetFrame(int _frame);
    void SetFrameImmediately(int _frame);
    int GetCurrentFrame() const {return m_CurFrame;}
    cCharacterSprite* GetCurrentSprite() const {return m_CurAnim->GetSprite(m_CurFrame);}
    void UpdateRendererOnly();
    
};
