#pragma once
#include "cEditorPanel.h"
#include "../cFrameList.h"
#include "../cSpriteBoxArea.h"

class cCharacterSprite;
static const Vec3 EditorSpriteCenterPos = Vec3(1020, 700, 0.5f);

class cEditorSpritePanel : public cEditorPanel, cMsgProcHandler
{
enum OnionSkin
{
    Idle,
    Jump,
    Prev,
    Next,
    Custom,
    End
};
    
public:
    cEditorSpritePanel(cObject* _owner) : cEditorPanel(_owner) {}
    ~cEditorSpritePanel() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    cScrollView* m_AnimationsScrollView;
    cScrollView* m_PaletteScrollView;
    cInputField* m_AnimationsInputField;
    cInputField* m_SpriteScaleField;
    cInputField* m_FrameLengthField;
    cInputField* m_OffsetXField;
    cInputField* m_OffsetYField;
    cInputField* m_MomentumXField;
    cInputField* m_MomentumYField;
    cInputField* m_FrictionXField;
    cInputField* m_FrictionYField;
    cInputField* m_AnimationEventKeyField;
    cInputField* m_SpriteEventKeyField;
    cInputField* m_PaletteIndexField;
    cInputField* m_FixedPaletteField;
    cInputField* m_HexColorField;
    cFrameList* m_FrameList;
    cSpriteBoxArea* m_SpriteBoxArea;

    cInputField* m_SpriteBoxEventKeyField;
    
    cInputField* m_HitBoxDamageField;
    cInputField* m_HitBoxDirectionField;
    cInputField* m_HitBoxHitStunMulField;
    cInputField* m_HitBoxShieldStunMulField;
    cInputField* m_HitBoxShieldDamageMulField;
    cInputField* m_HitBoxBaseKnockBackField;
    cInputField* m_HitBoxGrowthKnockBackField;
    cDropDown* m_HitBoxGuardDropDown;
    
    cInputField* m_ThrowBoxCanThrowMidairField;
    
    std::map<SerializedString, cCharacterAnimation*>* m_Animations;
    int m_CurPalette = 0;
    int m_SelectedPaletteEntryIndex = -1;

    std::vector<cObject*> m_SpriteBoxFields;
    std::vector<cObject*> m_HitBoxFields;
    std::vector<cObject*> m_ThrowBoxFields;

    cCharacterSprite* m_OnionSkins[OnionSkin::End];
    bool m_UsePrevOnionSkin = true;
    bool m_UseNextOnionSkin = false;
    
    void OnClickOpenImage();
    void OnClickAddAnimation();
    void OnClickRemoveAnimation();
    void ReloadAnimationList();
    void ReloadPaletteList(bool _resetScrollPos);
    void SetPaletteIndex(int _index);
    void OnClickAddPalette();
    void OnClickRemovePalette();
    
    cCharacterSprite* GetCurSprite() {return m_FrameList->GetAnimation()->GetSprite(m_FrameList->GetFrame());}
    void SetHitBoxGuard(Guard _guard);

public:
    void Save(cCharacterData* _data) override;
    void Load(cCharacterData* _data) override;
    void Reset() override;
    void OnShow() override;
    void OnHide() override;

    bool HasFocusedGUI();
    cFrameList* GetFrameList() {return m_FrameList;}
    cSpriteBoxArea* GetSpriteBoxArea() {return m_SpriteBoxArea;}
    void OnFrameChanged();
    void SetAnimationByKey(std::string _key);
    void OnSelectSpriteBox(cSpriteBoxArea::DrawType _type, std::vector<cSpriteBoxArea::SelectedBox> _boxes);
    void UnlinkRemovedOnionSkinSprites(cCharacterSprite* _spriteToRemove);
};