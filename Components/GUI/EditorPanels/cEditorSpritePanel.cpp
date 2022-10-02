#include "DXUT.h"
#include "cEditorSpritePanel.h"

#include "cEditorCharacterPanel.h"
#include "../cDropDown.h"
#include "../cScrollView.h"
#include "../../../Scenes/Editor/cEditorScene.h"

cEditorSpritePanel::~cEditorSpritePanel()
{
}

void cEditorSpritePanel::Init()
{
    cEditorPanel::Init();

    CreateButton(Vec2(85, 120), Vec2(150, 40), "이미지 불러오기", [&]()->void{OnClickOpenImage();});
    
    CreateButton(Vec2(460, 180), Vec2(100, 40), "추가", [&]()->void{OnClickAddAnimation();});
    CreateButton(Vec2(460, 220), Vec2(100, 40), "삭제", [&]()->void{OnClickRemoveAnimation();});
    m_AnimationsInputField = CreateInputField(Vec2(210, 180), Vec2(400, 40), "Animation Name", 24);
    m_AnimationsInputField->SetOnTextChanged([&](cInputField* _this)->void
    {
        ReloadAnimationList();
        m_AnimationsScrollView->SetSelectedIndex(0); 
    });
    
    m_AnimationsScrollView = CreateScrollView(Vec2(210, 450), Vec2(400, 500), Vec2(400, 25), [&](cObject* _selected)->void
    {
        m_FrameList->SetAnimation((*m_Animations)[_selected->GetComponent<cGUI>()->GetText()]);
    }, true);
    m_AnimationsScrollView->SetElementOffset(Vec2(5, 0));
    m_AnimationsScrollView->SetResetSelectedOnClickOutSide(true);

    CreateButton(Vec2(60, 740), Vec2(100, 40), "<", [&]()->void
    {
        SetPaletteIndex(m_CurPalette == 0 ? m_Data->GetPaletteCount() - 1 : (m_CurPalette - 1) % m_Data->GetPaletteCount());
    })->SetFontSize(32);
    CreateButton(Vec2(360, 740), Vec2(100, 40), ">", [&]()->void
    {
        SetPaletteIndex((m_CurPalette + 1) % m_Data->GetPaletteCount());
    })->SetFontSize(32);
    m_PaletteIndexField = CreateInputField(Vec2(460, 790), Vec2(100, 40), "Palette Index", 24);
    CreateButton(Vec2(460, 830), Vec2(100, 40), "추가", [&]()->void{OnClickAddPalette();});
    CreateButton(Vec2(460, 870), Vec2(100, 40), "삭제", [&]()->void{OnClickRemovePalette();});
    m_PaletteScrollView = CreateScrollView(Vec2(210, 920), Vec2(400, 300), Vec2(25, 25), [&](cObject* _selected)->void
    {
        m_HexColorField->GetOwner()->SetActive(true);
        Vec2 size = _selected->GetComponent<cGUI>()->GetSize() + m_HexColorField->GetSize();
        m_HexColorField->GetOwner()->SetPos(_selected->GetPos() + Vec3(size.x * 0.5f, size.y * 0.5f, -0.2f));
        
        m_SelectedPaletteEntryIndex = m_PaletteScrollView->GetSelectedIndex();
        m_HexColorField->SetText(IntToHex(m_Data->GetPaletteByArrayIndex(m_CurPalette)->GetColor(m_SelectedPaletteEntryIndex)));
    }, true);
    m_PaletteScrollView->SetResetSelectedOnClickOutSide(true);

    m_HexColorField = CreateInputField(Vec2(), Vec2(150, 40), "Hex Color", 24, false);
    m_HexColorField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (m_SelectedPaletteEntryIndex < 0)
            return;

        int color = HexToInt(_this->GetText());
        cPalette* palette = m_Data->GetPaletteByArrayIndex(m_CurPalette);
        palette->SetColor(m_SelectedPaletteEntryIndex, color >> 24, color >> 16, color >> 8, color);
        ReloadPaletteList(false);
        palette->UpdateTexture();
    });
    m_HexColorField->SetInactivateOnClickOutside(true);
    m_HexColorField->GetOwner()->SetActive(false);

    m_SpriteScaleField = CreateInputField(Vec2(1920 - 85, 120), Vec2(100, 40), "Sprite Scale", 24);
    m_SpriteScaleField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_Data->SetSpriteScale(_this->GetFloat());
    });
    
    m_OffsetXField = CreateInputField(Vec2(1920 - 190, 200), Vec2(100, 40), "Offset X", 24);
    m_OffsetXField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetOffset(ShortVec2(_this->GetInt(), _sprite->GetOffset().y)); 
        });
    });
    
    m_OffsetYField = CreateInputField(Vec2(1920 - 85, 200), Vec2(100, 40), "Offset Y", 24);
    m_OffsetYField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetOffset(ShortVec2(_sprite->GetOffset().x, _this->GetInt())); 
        });
    });

    m_MomentumXField = CreateInputField(Vec2(1920 - 190, 280), Vec2(100, 40), "Momentum X", 24);
    m_MomentumXField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetMomentum(ShortVec2(_this->GetInt(), _sprite->GetMomentum().y)); 
        });
    });

    m_MomentumYField = CreateInputField(Vec2(1920 - 85, 280), Vec2(100, 40), "Momentum Y", 24);
    m_MomentumYField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetMomentum(ShortVec2(_sprite->GetMomentum().x, _this->GetInt())); 
        });
    });

    m_FrictionXField = CreateInputField(Vec2(1920 - 190, 360), Vec2(100, 40), "Friction X", 24);
    m_FrictionXField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetFriction(Vec2(_this->GetFloat(), _sprite->GetFriction().y)); 
        });
    });

    m_FrictionYField = CreateInputField(Vec2(1920 - 85, 360), Vec2(100, 40), "Friction Y", 24);
    m_FrictionYField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetFriction(Vec2(_sprite->GetFriction().x, _this->GetFloat()));
        });
    });

    m_FrameLengthField = CreateInputField(Vec2(1920 - 85, 440), Vec2(100, 40), "Frame Length", 24);
    m_FrameLengthField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetFrameLength(_this->GetInt()); 
        });
    });

    m_FixedPaletteField = CreateInputField(Vec2(1920 - 190, 440), Vec2(100, 40), "Fixed Palette", 24);
    m_FixedPaletteField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (!m_FrameList->CanEdit())
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetFixedPaletteIndex(_this->GetInt()); 
        });
    });
    
    m_AnimationEventKeyField = CreateInputField(Vec2(EditorSpriteCenterPos.x, 1005), Vec2(800, 40), "Animation Event Key", 24);
    m_AnimationEventKeyField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (m_FrameList->GetAnimation() == nullptr)
            return;
        m_FrameList->GetAnimation()->SetEventKey(_this->GetText());
    });

    m_SpriteEventKeyField = CreateInputField(Vec2(EditorSpriteCenterPos.x, 1060), Vec2(800, 40), "Event Key", 24);
    m_SpriteEventKeyField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (m_FrameList->GetAnimation() == nullptr)
            return;
        m_FrameList->WithSelectedFrames([&](cCharacterSprite* _sprite)->void
        {
            _sprite->SetEventKey(_this->GetText());
        });
    });

    m_FrameList = OBJECT->AddObject<cFrameList>("FrameList", Vec3(EditorSpriteCenterPos.x, 950, 0.5f), Obj_GUI);
    m_FrameList->SetSize(Vec2(800, 40));
    m_Objects.push_back(m_FrameList->GetOwner());

    m_SpriteBoxArea = OBJECT->AddObject<cSpriteBoxArea>("SpriteBoxArea", Vec3(EditorSpriteCenterPos.x, 460, 0.8f), Obj_GUI);
    m_SpriteBoxArea->SetSize(Vec2(1000, 930));
    m_Objects.push_back(m_SpriteBoxArea->GetOwner());

    m_SpriteBoxEventKeyField = CreateInputField(Vec2(1680, 1060), Vec2(450, 40), "Event Key", 20, false);
    m_SpriteBoxEventKeyField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            _selected.box->SetEventKey(_this->GetText());
        });
    });
    m_SpriteBoxFields.push_back(m_SpriteBoxEventKeyField->GetOwner());

    m_HitBoxDamageField = CreateInputField(Vec2(1920 - 190, 700), Vec2(100, 40), "Damage", 24, false);
    m_HitBoxDamageField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetDamage(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxDamageField->GetOwner());
    
    m_HitBoxGuardDropDown = CreateDropDown(Vec2(1920 - 85, 700), Vec2(100, 40), 100, "Mid", false)
    ->SetDropDownHeight(30)
    ->AddButton(CreateButton(Vec2(0, 0), Vec2(0, 0), GuardToStringMap[0], [&]()->void{SetHitBoxGuard(Guard::Low);}, false))
    ->AddButton(CreateButton(Vec2(0, 0), Vec2(0, 0), GuardToStringMap[1], [&]()->void{SetHitBoxGuard(Guard::Mid);}, false))
    ->AddButton(CreateButton(Vec2(0, 0), Vec2(0, 0), GuardToStringMap[2], [&]()->void{SetHitBoxGuard(Guard::High);}, false))
    ->AddButton(CreateButton(Vec2(0, 0), Vec2(0, 0), GuardToStringMap[3], [&]()->void{SetHitBoxGuard(Guard::All);}, false))
    ->AddButton(CreateButton(Vec2(0, 0), Vec2(0, 0), GuardToStringMap[4], [&]()->void{SetHitBoxGuard(Guard::Unblockable);}, false));
    m_HitBoxFields.push_back(m_HitBoxGuardDropDown->GetOwner());

    m_HitBoxShieldStunMulField = CreateInputField(Vec2(1920 - 190, 780), Vec2(100, 40), "ShieldStun Mul", 24, false);
    m_HitBoxShieldStunMulField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetShieldStunMul(_this->GetFloat());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxShieldStunMulField->GetOwner());

    m_HitBoxShieldDamageMulField = CreateInputField(Vec2(1920 - 85, 780), Vec2(100, 40), "ShieldDamage Mul", 24, false);
    m_HitBoxShieldDamageMulField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetShieldDamageMul(_this->GetFloat());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxShieldDamageMulField->GetOwner());
    
    m_HitBoxHitStunMulField = CreateInputField(Vec2(1920 - 400, 860), Vec2(100, 40), "HitStun Mul", 24, false);
    m_HitBoxHitStunMulField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetHitStunMul(_this->GetFloat());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxHitStunMulField->GetOwner());
    
    m_HitBoxDirectionField = CreateInputField(Vec2(1920 - 295, 860), Vec2(100, 40), "Direction", 24, false);
    m_HitBoxDirectionField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetDirection(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxDirectionField->GetOwner());
    
    m_HitBoxBaseKnockBackField = CreateInputField(Vec2(1920 - 190, 860), Vec2(100, 40), "Base KnockBack", 24, false);
    m_HitBoxBaseKnockBackField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetBaseKnockBack(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxBaseKnockBackField->GetOwner());

    m_HitBoxGrowthKnockBackField = CreateInputField(Vec2(1920 - 85, 860), Vec2(100, 40), "Growth KnockBack", 24, false);
    m_HitBoxGrowthKnockBackField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetGrowthKnockBack(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxGrowthKnockBackField->GetOwner());

    m_HitBoxAirHitStunMulField = CreateInputField(Vec2(1920 - 400, 940), Vec2(100, 40), "Air HitStun Mul", 24, false);
    m_HitBoxAirHitStunMulField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetAirHitStunMul(_this->GetFloat());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxAirHitStunMulField->GetOwner());

    m_HitBoxAirDirectionField = CreateInputField(Vec2(1920 - 295, 940), Vec2(100, 40), "Air Direction", 24, false);
    m_HitBoxAirDirectionField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetAirDirection(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxAirDirectionField->GetOwner());
    
    m_HitBoxAirBaseKnockBackField = CreateInputField(Vec2(1920 - 190, 940), Vec2(100, 40), "Air Base KnockBack", 24, false);
    m_HitBoxAirBaseKnockBackField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetAirBaseKnockBack(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxAirBaseKnockBackField->GetOwner());

    m_HitBoxAirGrowthKnockBackField = CreateInputField(Vec2(1920 - 85, 940), Vec2(100, 40), "Air Growth KnockBack", 24, false);
    m_HitBoxAirGrowthKnockBackField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cHitBox*)_selected.box)->SetAirGrowthKnockBack(_this->GetInt());
        });
    });
    m_HitBoxFields.push_back(m_HitBoxAirGrowthKnockBackField->GetOwner());

    m_ThrowBoxCanThrowMidairField = CreateInputField(Vec2(1920 - 85, 700), Vec2(100, 40), "Can Throw Midair", 24, false);
    m_ThrowBoxCanThrowMidairField->SetOnTextChanged([&](cInputField* _this)->void
    {
        m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
        {
            ((cThrowBox*)_selected.box)->SetCanThrowMidair(_this->GetInt());
        });
    });
    m_ThrowBoxFields.push_back(m_ThrowBoxCanThrowMidairField->GetOwner());

    OnSelectSpriteBox(cSpriteBoxArea::Select, std::vector<cSpriteBoxArea::SelectedBox>());
}

void cEditorSpritePanel::Update()
{
    cEditorPanel::Update();

    if (INPUT->KeyPress(VK_LCONTROL))
    {
        if (INPUT->KeyDown('I'))
        {
            if (m_OnionSkins[OnionSkin::Idle] != nullptr)
                m_OnionSkins[OnionSkin::Idle] = nullptr;
            else
            {
                cCharacterAnimation* anim = m_Data->GetAnimation("Idle");
                if (anim != nullptr && anim->GetLength() > 0)
                    m_OnionSkins[OnionSkin::Idle] = anim->GetSprite(0);
            }
        }
        
        if (INPUT->KeyDown('J'))
        {
            if (m_OnionSkins[OnionSkin::Jump] != nullptr)
                m_OnionSkins[OnionSkin::Jump] = nullptr;
            else
            {
                cCharacterAnimation* anim = m_Data->GetAnimation("Jump");
                if (anim != nullptr && anim->GetLength() > 0)
                    m_OnionSkins[OnionSkin::Jump] = anim->GetSprite(0);
            }
        }

        if (INPUT->KeyDown('U'))
        {
            if (m_OnionSkins[OnionSkin::Custom] != nullptr)
                m_OnionSkins[OnionSkin::Custom] = nullptr;
            else
                m_OnionSkins[OnionSkin::Custom] = GetCurSprite();
        }

        if (INPUT->KeyDown(VK_OEM_COMMA))
            m_UsePrevOnionSkin = !m_UsePrevOnionSkin;

        if (INPUT->KeyDown(VK_OEM_PERIOD))
            m_UseNextOnionSkin = !m_UseNextOnionSkin;
    }
}

void cEditorSpritePanel::Render()
{
    cEditorPanel::Render();

    Texture* pixel = IMAGE->GetTexture("Pixel");

    float scale = 1;
    if (m_FrameList->CanEdit())
    {
        cCharacterSprite* sprite;
        Vec3 pos;
        scale = m_Data->GetSpriteScale();

        cPalette* palette = m_Data->GetPaletteByArrayIndex(m_CurPalette);
        if (palette != nullptr)
        {
            IMAGE->BeginPaletteSwapShader();
            
            IMAGE->SetPaletteSwapShaderParams(palette, 64);
            for (int i = 0; i < OnionSkin::End; i++)
            {
                sprite = m_OnionSkins[i];
                if (sprite == nullptr || (!m_UsePrevOnionSkin && i == OnionSkin::Prev) || (!m_UseNextOnionSkin && i == OnionSkin::Next))
                    continue;
                pos = EditorSpriteCenterPos;
                pos.x += sprite->GetOffset().x * scale;
                pos.y += sprite->GetOffset().y * scale;
                pos.z += 0.01f;
                IMAGE->RenderSprite(sprite->GetTexture(), pos, 0, Vec2(1,1) * scale, Vec2(0.5f,0.5f), 0xffffffff, true);
            }
            
            IMAGE->EndPaletteSwapShader();
            IMAGE->BeginPaletteSwapShader();

            sprite = GetCurSprite();
            pos = EditorSpriteCenterPos;
            pos.x += sprite->GetOffset().x * scale;
            pos.y += sprite->GetOffset().y * scale;
            IMAGE->SetPaletteSwapShaderParams(palette, 255);
            IMAGE->RenderSprite(sprite->GetTexture(), pos, 0, Vec2(1,1) * scale, Vec2(0.5f,0.5f), 0xffffffff, true);
            
            IMAGE->EndPaletteSwapShader();
        }
        else
        {
            for (int i = 0; i < OnionSkin::End; i++)
            {
                sprite = m_OnionSkins[i];
                if (sprite == nullptr || (!m_UsePrevOnionSkin && i == OnionSkin::Prev) || (!m_UseNextOnionSkin && i == OnionSkin::Next))
                    continue;
                pos = EditorSpriteCenterPos;
                pos.x += sprite->GetOffset().x * scale;
                pos.y += sprite->GetOffset().y * scale;
                pos.z += 0.01f;
                IMAGE->RenderSprite(sprite->GetTexture(), pos, 0, Vec2(1,1) * scale, Vec2(0.5f,0.5f), 0x7fffffff, true);
            }

            IMAGE->RenderSprite(sprite->GetTexture(), pos, 0, Vec2(1,1) * scale, Vec2(0.5f,0.5f), 0xffffffff, true);
        }
    }

    IMAGE->RenderSprite(pixel, EditorSpriteCenterPos, 0, Vec2(500,scale), Vec2(0.5f,0),0x50ffffff, true);
    IMAGE->RenderSprite(pixel, EditorSpriteCenterPos, 0, Vec2(scale,500), Vec2(0.5f,0.5f),0x50ffffff, true);

    IMAGE->RenderText("Gothic", std::to_string(m_CurPalette + 1) + "/" + std::to_string(m_Data->GetPaletteCount()),
        Vec3(210, 740, 0.5f), 0, Vec2(0.4f,0.4f), Vec2(0.5f, 0.5f), 0xff000000, true);
}

void cEditorSpritePanel::Release()
{
    cEditorPanel::Release();
}

void cEditorSpritePanel::OnCollision(cObject* _other)
{
    cEditorPanel::OnCollision(_other);
}

void cEditorSpritePanel::OnAlarm(std::string _key)
{
    cEditorPanel::OnAlarm(_key);
}

void cEditorSpritePanel::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYDOWN)
    {
        if (m_AnimationsInputField->IsFocused())
        {
            if (wParam == VK_RETURN && !m_AnimationsInputField->GetText().empty())
            {
                OnClickAddAnimation();
            }
        }

        if (!HasFocusedGUI())
        {
            if (m_FrameList->GetAnimation() != nullptr)
            {
                int curFrame = m_FrameList->GetFrame();
                int frameLength = m_FrameList->GetAnimation()->GetLength();
                if (wParam == 'A')
                {
                    m_FrameList->SetFrame(curFrame == 0 ? frameLength - 1 : (curFrame - 1) % frameLength);
                }

                if (wParam == 'D')
                {
                    m_FrameList->SetFrame((curFrame + 1) % frameLength);
                }
                
                if (frameLength > 0)
                {
                    if (wParam == VK_LEFT || wParam == VK_RIGHT)
                    {
                        int offsetX = GetCurSprite()->GetOffset().x;
                        offsetX += (!INPUT->KeyPress(VK_LSHIFT) ? 1 : 8) * (wParam == VK_RIGHT ? 1 : -1);
                        m_OffsetXField->SetText(std::to_string(offsetX));
                    }
                
                    if (wParam == VK_UP || wParam == VK_DOWN)
                    {
                        int offsetY = GetCurSprite()->GetOffset().y;
                        offsetY += (!INPUT->KeyPress(VK_LSHIFT) ? 1 : 8) * (wParam == VK_DOWN ? 1 : -1);
                        m_OffsetYField->SetText(std::to_string(offsetY));
                    }
                }
            }
        }
    }
}

void cEditorSpritePanel::OnClickOpenImage()
{
    if (m_FrameList->GetAnimation() == nullptr)
        return;

    if (m_Data->GetPaletteByArrayIndex(m_CurPalette) == nullptr)
    {
        MessageBoxA(DXUTGetHWND(), "Please Select Palette!", "ERROR!", 0);
        return;
    }
   
    std::vector<std::wstring> filePaths;
    if (!cEditorScene::OpenExplorer(&filePaths, "*.png", FOS_ALLOWMULTISELECT))
        return;

    int paletteIndex = m_CurPalette;
    int prevFrame = m_FrameList->GetFrame();
    int lastFrame = m_FrameList->GetAnimation()->GetLength() - 1;
    int filePathsCount = filePaths.size();
    std::string curAnimKey = m_FrameList->GetAnimation()->GetKey();
    INPUT->Do([this, paletteIndex, filePaths, curAnimKey]()->void
    {
        SetAnimationByKey(curAnimKey);
        PALETTEENTRY* entries;
        m_Data->GetPaletteByArrayIndex(paletteIndex)->GetEntries(entries);
        
        for(int i = 0; i < filePaths.size(); i++)
        {
            cCharacterSprite* sprite = new cCharacterSprite();
            sprite->SetTexture(filePaths[i], entries);
            m_FrameList->GetAnimation()->AddSprite(sprite);
        }

        m_FrameList->SetFrame(m_FrameList->GetAnimation()->GetLength() - 1);
    }, [this, filePathsCount, prevFrame, lastFrame, curAnimKey]()->void
    {
        SetAnimationByKey(curAnimKey);
        for(int i = 0; i < filePathsCount; i++)
            m_FrameList->GetAnimation()->RemoveSprite(lastFrame + 1);
        m_FrameList->SetFrame(prevFrame);
    });
}

void cEditorSpritePanel::OnClickAddAnimation()
{
    std::string key = m_AnimationsInputField->GetText();
    if (key.empty())
        return;

    if (m_Animations->find(key) != m_Animations->end())
        return;

    INPUT->Do([this, key]()->void
    {
        cCharacterAnimation* anim = new cCharacterAnimation();
        (*m_Animations)[key] = anim;
        
        m_FrameList->SetAnimation(anim);
        m_AnimationsInputField->SetText("");
        ReloadAnimationList();
    },[this, key]()->void
    {
        m_Data->RemoveAnimation(key);
        
        m_FrameList->SetAnimation(nullptr);
        ReloadAnimationList();
    });
}

void cEditorSpritePanel::OnClickRemoveAnimation()
{
    cCharacterAnimation* anim = m_FrameList->GetAnimation();
    if (anim == nullptr)
        return;

    std::string key;
    for (auto& iter : *m_Animations)
    {
        if (iter.second == anim)
        {
            key = iter.first;
            break;
        }
    }
    
    std::string serializedAnim;
    serializedAnim.resize(anim->GetSize());
    UINT pointer = 0;
    anim->Serialize(const_cast<char*>(serializedAnim.c_str()), pointer);
    
    INPUT->Do([this, key]()->void
    {
        m_Data->RemoveAnimation(key);
        m_FrameList->SetAnimation(nullptr);
        ReloadAnimationList();
    }, [this, serializedAnim, key]()->void
    {
        UINT pointer = 0;
        cCharacterAnimation* anim = m_Data->AddAnimation(key);
        anim->Deserialize(const_cast<char*>(serializedAnim.c_str()), pointer);
        m_FrameList->SetAnimation(anim);
        ReloadAnimationList();
    });
}

void cEditorSpritePanel::ReloadAnimationList()
{
    m_AnimationsScrollView->RemoveAllElement();
    m_AnimationsScrollView->SetScrollPos(0);
    
    std::string find = m_AnimationsInputField->GetText();
    if (find.empty())
    {
        for (auto& iter : *m_Animations)
        {
            cGUI* text = OBJECT->AddObject<cGUI>("Text", Vec3(), Obj_GUI)
                ->SetDefaults(Vec2(0,0), iter.first, 0, 0);
            m_AnimationsScrollView->AddElement(text);
        }
    }
    else
    {
        for (auto& iter : *m_Animations)
        {
            if (((std::string)iter.first).find(find) != std::string::npos)
            {
                cGUI* text = OBJECT->AddObject<cGUI>("Text", Vec3(), Obj_GUI)
                    ->SetDefaults(Vec2(0,0), iter.first, 0, 0);
                m_AnimationsScrollView->AddElement(text);
            }
        }
    }
}

void cEditorSpritePanel::ReloadPaletteList(bool _resetScrollPos)
{
    m_PaletteScrollView->RemoveAllElement();
    if (_resetScrollPos)
        m_PaletteScrollView->SetScrollPos(0);

    cPalette* palette = m_Data->GetPaletteByArrayIndex(m_CurPalette);
    if (palette == nullptr)
        return;
    
    int count = m_Data->GetPaletteLength();
    for (int i = 0; i < count; i++)
    {
        cGUI* color = OBJECT->AddObject<cGUI>("PaletteEntry", Vec3(), Obj_GUI)
            ->SetDefaults(Vec2(), "", palette->GetColor(i));
        m_PaletteScrollView->AddElement(color);
    }
}

void cEditorSpritePanel::SetPaletteIndex(int _index)
{
    m_CurPalette = _index;
    cPalette* palette = m_Data->GetPaletteByArrayIndex(_index);
    if (palette == nullptr)
        return;
    
    m_PaletteIndexField->SetText(std::to_string(palette->GetIndex()));

    ReloadPaletteList(true);
}

void cEditorSpritePanel::OnClickAddPalette()
{
    std::vector<std::wstring> filePaths;
    if (!cEditorScene::OpenExplorer(&filePaths, ".png", 0))
        return;
    
    Texture* tex = new Texture;
    D3DXIMAGE_INFO info;
    PALETTEENTRY* entries = new PALETTEENTRY[SCENE->GetScene<cEditorScene>()->GetCharacterData()->GetPaletteLength()];
    if (D3DXCreateTextureFromFileEx(g_Device, filePaths[0].c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, &info, entries, &tex->texture) != S_OK)
    {
        MessageBoxA(DXUTGetHWND(), "An error occurred while loading image.", "ERROR!", 0);
        return;
    }

    if (entries[1].peRed == 255 && entries[1].peGreen == 255 && entries[1].peBlue == 255)
    {
        std::vector<D3DXCOLOR> colors;
        D3DLOCKED_RECT rect;
        tex->texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
        DWORD* pixels = (DWORD*)rect.pBits;

        for (int y = 0; y < info.Height; y++)
        {
            for (int x = 0; x < info.Width; x++)
            {
                D3DXCOLOR pixel = (D3DXCOLOR)pixels[y * info.Width + x];
                for (auto& iter : colors)
                {
                    if (iter == pixel)
                        goto next;
                }
                colors.push_back(pixel);
                
                next:
                continue;
            }
        }

        for (int i = 0; i < colors.size(); i++)
            entries[i] = PALETTEENTRY {(BYTE)(colors[i].r * 255), (BYTE)(colors[i].g * 255), (BYTE)(colors[i].b * 255), (BYTE)(colors[i].a * 255)};

        tex->texture->UnlockRect(0);
    }

    tex->texture->Release();
    delete tex;

    int index = m_PaletteIndexField->GetInt();
    INPUT->Do([this, entries, index]()->void
    {
        cPalette* palette = new cPalette(index, entries, m_Data->GetPaletteLength());
        m_Data->SetPalette(palette);

        ReloadPaletteList(true);
    }, [this, index]()->void
    {
        m_Data->RemovePalette(index);

        ReloadPaletteList(true);
    });

}

void cEditorSpritePanel::OnClickRemovePalette()
{
}

bool cEditorSpritePanel::HasFocusedGUI()
{
    return m_AnimationsInputField->IsFocused() || m_OffsetXField->IsFocused() || m_OffsetYField->IsFocused() || m_FrameLengthField->IsFocused()
    || m_SpriteScaleField->IsFocused() || m_MomentumXField->IsFocused() || m_MomentumYField->IsFocused() || m_FrictionXField->IsFocused() || m_FrictionYField->IsFocused()
    || m_SpriteEventKeyField->IsFocused() || m_PaletteIndexField->IsFocused() || m_HexColorField->IsFocused() || m_FixedPaletteField->IsFocused() || m_SpriteBoxArea->HasSelectedBox()
    || m_SpriteBoxEventKeyField->IsFocused() || m_HitBoxDamageField->IsFocused() || m_HitBoxDirectionField->IsFocused() || m_HitBoxHitStunMulField->IsFocused()
    || m_HitBoxShieldStunMulField->IsFocused() || m_HitBoxShieldDamageMulField->IsFocused() || m_HitBoxBaseKnockBackField->IsFocused() || m_HitBoxGrowthKnockBackField->IsFocused()
    || m_ThrowBoxCanThrowMidairField->IsFocused() || m_AnimationsScrollView->GetSelectedIndex() >= 0 || m_AnimationEventKeyField->IsFocused()
    ;
}

void cEditorSpritePanel::SetHitBoxGuard(Guard _guard)
{
    m_SpriteBoxArea->WithSelectedBoxes([&](cSpriteBoxArea::SelectedBox _selected)->void
    {
        ((cHitBox*)_selected.box)->SetGuard(_guard);
    });
    m_HitBoxGuardDropDown->SetText(GuardToStringMap[(int)_guard]);
}

void cEditorSpritePanel::Save(cCharacterData* _data)
{
}

void cEditorSpritePanel::Load(cCharacterData* _data)
{
    Reset();
}

void cEditorSpritePanel::Reset()
{
    cEditorPanel::Reset();
    m_Animations = m_Data->GetAnimations();
    m_FrameList->SetAnimation(nullptr);
    
    m_OffsetXField->SetText("0");
    m_OffsetYField->SetText("0");
    m_MomentumXField->SetText("0");
    m_MomentumYField->SetText("0");
    m_FrictionXField->SetText("1");
    m_FrictionYField->SetText("1");
    m_FrameLengthField->SetText("1");
    m_PaletteIndexField->SetText("0");
    m_FixedPaletteField->SetText("0");
    m_SpriteEventKeyField->SetText("");
    m_AnimationEventKeyField->SetText("");

    m_SpriteScaleField->SetText(std::to_string(SCENE->GetScene<cEditorScene>()->GetCharacterData()->GetSpriteScale()));

    m_FrameList->SetFrame(0);
    SetPaletteIndex(0);
    
    ReloadAnimationList();
    ReloadPaletteList(true);

    for (int i = 0; i < OnionSkin::End; i++)
        m_OnionSkins[i] = nullptr;
}

void cEditorSpritePanel::OnShow()
{
}

void cEditorSpritePanel::OnHide()
{
}

void cEditorSpritePanel::OnFrameChanged()
{
    m_SpriteBoxArea->SetSprite(nullptr);

    if (!m_FrameList->CanEdit())
        return;

    cCharacterSprite* sprite = GetCurSprite();
    m_SpriteBoxArea->SetSprite(sprite);
    m_OffsetXField->SetText(std::to_string(sprite->GetOffset().x), false);
    m_OffsetYField->SetText(std::to_string(sprite->GetOffset().y), false);
    m_MomentumXField->SetText(std::to_string(sprite->GetMomentum().x), false);
    m_MomentumYField->SetText(std::to_string(sprite->GetMomentum().y), false);
    m_FrictionXField->SetText(std::to_string(sprite->GetFriction().x), false);
    m_FrictionYField->SetText(std::to_string(sprite->GetFriction().y), false);
    m_FrameLengthField->SetText(std::to_string(sprite->GetFrameLength()), false);
    m_FixedPaletteField->SetText(std::to_string(sprite->GetFixedPaletteIndex()), false);
    m_SpriteEventKeyField->SetText(sprite->GetEventKey(), false);
    m_AnimationEventKeyField->SetText(m_FrameList->GetAnimation()->GetEventKey());

    int curFrame = m_FrameList->GetFrame();
    int frameLength = m_FrameList->GetAnimation()->GetLength();
    m_OnionSkins[OnionSkin::Prev] = m_FrameList->GetAnimation()->GetSprite(curFrame == 0 ? frameLength - 1 : (curFrame - 1) % frameLength);
    m_OnionSkins[OnionSkin::Next] = m_FrameList->GetAnimation()->GetSprite((curFrame + 1) % frameLength);
}

void cEditorSpritePanel::SetAnimationByKey(std::string _key)
{
    cCharacterAnimation* anim = m_Data->GetAnimation(_key);
    if (anim == nullptr)
        return;
    m_FrameList->SetAnimation(anim);
}

void cEditorSpritePanel::OnSelectSpriteBox(cSpriteBoxArea::DrawType _type, std::vector<cSpriteBoxArea::SelectedBox> _boxes)
{
    for (auto& iter : m_SpriteBoxFields)
        iter->SetActive(_type != cSpriteBoxArea::Select && _type != cSpriteBoxArea::AttachPoint);
    for (auto& iter : m_HitBoxFields)
        iter->SetActive(_type == cSpriteBoxArea::HitBox);
    for (auto& iter : m_ThrowBoxFields)
        iter->SetActive(_type == cSpriteBoxArea::ThrowBox);

    if (_boxes.empty())
        return;
    
    if (_type == cSpriteBoxArea::HitBox)
    {
        cHitBox* box = (cHitBox*)_boxes[0].box;
        m_HitBoxDamageField->SetText(std::to_string(box->GetDamage()), false);
        m_HitBoxGuardDropDown->SetText(GuardToStringMap[(int)box->GetGuard()]);
        m_HitBoxHitStunMulField->SetText(std::to_string(box->GetHitStunMul()), false);
        m_HitBoxShieldStunMulField->SetText(std::to_string(box->GetShieldStunMul()), false);
        m_HitBoxShieldDamageMulField->SetText(std::to_string(box->GetShieldDamageMul()), false);
        m_HitBoxDirectionField->SetText(std::to_string(box->GetDirection()), false);
        m_HitBoxBaseKnockBackField->SetText(std::to_string(box->GetBaseKnockBack()), false);
        m_HitBoxGrowthKnockBackField->SetText(std::to_string(box->GetGrowthKnockBack()), false);
        m_HitBoxAirDirectionField->SetText(std::to_string(box->GetAirDirection()), false);
        m_HitBoxAirBaseKnockBackField->SetText(std::to_string(box->GetAirBaseKnockBack()), false);
        m_HitBoxAirGrowthKnockBackField->SetText(std::to_string(box->GetAirGrowthKnockBack()), false);
        m_HitBoxAirHitStunMulField->SetText(std::to_string(box->GetAirHitStunMul()), false);
    }
    else if (_type == cSpriteBoxArea::ThrowBox)
    {
        cThrowBox* box = (cThrowBox*)_boxes[0].box;
        m_HitBoxDamageField->SetText(box->GetCanThrowMidair() ? "1" : "0", false);
    }

    m_SpriteBoxEventKeyField->SetText(_boxes[0].box->GetEventKey(), false);
}

void cEditorSpritePanel::UnlinkRemovedOnionSkinSprites(cCharacterSprite* _spriteToRemove)
{
    for (int i = 0; i < OnionSkin::End; i++)
    {
        if (m_OnionSkins[i] == _spriteToRemove)
            m_OnionSkins[i] = nullptr;
    }
}
