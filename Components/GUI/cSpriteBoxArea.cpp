#include "DXUT.h"
#include "cSpriteBoxArea.h"

#include "../../Scenes/Editor/cEditorScene.h"
#include "EditorPanels/cEditorSpritePanel.h"

cSpriteBoxArea::~cSpriteBoxArea()
{
}

void cSpriteBoxArea::Init()
{
    cGUI::Init();
}

void cSpriteBoxArea::Update()
{
    cGUI::Update();

    if (INPUT->KeyDown('1'))
        m_SelectedDrawType = Select;
    if (INPUT->KeyDown('2'))
        m_SelectedDrawType = HurtBox;
    if (INPUT->KeyDown('3'))
        m_SelectedDrawType = HitBox;
    if (INPUT->KeyDown('4'))
        m_SelectedDrawType = BodyBox;
    if (INPUT->KeyDown('5'))
        m_SelectedDrawType = ThrowBox;
    if (INPUT->KeyDown('6'))
        m_SelectedDrawType = AttachPoint;

    if (!IsMouseOver())
    {
        m_DrawStartedPos = Vec2(-1 ,-1);
        return;
    }
    
    if (m_CurSprite == nullptr)
        return;

    float spriteScale = SCENE->GetScene<cEditorScene>()->GetCharacterData()->GetSpriteScale();
    int frame = SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->GetFrame();
    std::string animationKey = SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->GetAnimation()->GetKey();
    if (INPUT->KeyDown(VK_LBUTTON))
    {
        if (!INPUT->KeyPress(VK_LCONTROL))
            m_SelectedSpriteBoxes.clear();
        
        if (m_SelectedDrawType == AttachPoint)
        {
            Vec2 pos = Vec2(INPUT->GetMousePos().x - EditorSpriteCenterPos.x, INPUT->GetMousePos().y - EditorSpriteCenterPos.y) / spriteScale;
            INPUT->Do([this, animationKey, frame, pos]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);
                ::AttachPoint** pointer;
                int count = m_CurSprite->GetAttachPoints(pointer) + 1;
                if (count > (int)AttachPointType::End)
                    return;
                
                ::AttachPoint** newPointer = new ::AttachPoint*[count];
                if (pointer != nullptr)
                    memcpy(newPointer, pointer, sizeof(::AttachPoint*) * count - 1);
                            
                ::AttachPoint* attachPoint = new ::AttachPoint();
                attachPoint->x = pos.x;
                attachPoint->y = pos.y;
                for (int type = 0; type < (int)AttachPointType::End; type++)
                {
                    bool duplicated = false;
                    for (int i = 0; i < count - 1; i++)
                    {
                        if (pointer[i]->type != (AttachPointType)type)
                            continue;
                        duplicated = true;
                        break;
                    }
                    
                    if (duplicated)
                        continue;
                    attachPoint->type = (AttachPointType)type;
                    break;
                }
                newPointer[count - 1] = attachPoint;
                m_CurSprite->SetAttachPoints(newPointer, count); 
            },[this, animationKey, frame]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);
                ::AttachPoint** pointer;
                int count = m_CurSprite->GetAttachPoints(pointer) - 1;
                ::AttachPoint** newPointer = new ::AttachPoint*[count];
                if (pointer != nullptr)
                {
                    delete pointer[count];
                    memcpy(newPointer, pointer, sizeof(::AttachPoint*) * count);
                }
                m_CurSprite->SetAttachPoints(newPointer, count); 
            });
        }
        else
        {
            m_DrawStartedPos = INPUT->GetMousePos();
        }
    }
    if (INPUT->KeyUp(VK_LBUTTON) && m_DrawStartedPos.x != -1)
    {
        bool isDrag = abs(m_DrawStartedPos.x - INPUT->GetMousePos().x) > 15 && abs(m_DrawStartedPos.y - INPUT->GetMousePos().y) > 15;
        m_DrawStartedPos -= Vec2(EditorSpriteCenterPos.x, EditorSpriteCenterPos.y);
        Vec2 drawEndedPos = INPUT->GetMousePos() - Vec2(EditorSpriteCenterPos.x, EditorSpriteCenterPos.y);
                
        RECT rect = RECT
        {
            (LONG)(min(m_DrawStartedPos.x, drawEndedPos.x) / spriteScale),
            (LONG)(min(m_DrawStartedPos.y, drawEndedPos.y) / spriteScale),
            (LONG)(max(m_DrawStartedPos.x, drawEndedPos.x) / spriteScale),
            (LONG)(max(m_DrawStartedPos.y, drawEndedPos.y) / spriteScale),
        };
        
        if (m_SelectedDrawType == DrawType::Select && m_DrawStartedPos.x != -1 && m_DrawStartedPos.y != -1)
        {
            rect.right += 1;
            rect.bottom += 1;
            WithBoxes([&](cSpriteBox* _box, DrawType _type)->void
            {
                RECT overlappedRect;
                if (IntersectRect(&overlappedRect, &rect, &_box->GetRect()))
                {
                    for (auto& iter : m_SelectedSpriteBoxes)
                        if (iter.box == _box)
                            return;
                    m_SelectedSpriteBoxes.emplace_back(_box, nullptr, _type);
                }
            });

            ::AttachPoint** attachPoints;
            int count = m_CurSprite->GetAttachPoints(attachPoints);
            for (int i = 0; i < count; i++)
            {
                RECT attachPointRect = RECT
                {
                    (LONG)(attachPoints[i]->x - 7.5f * spriteScale),
                    (LONG)(attachPoints[i]->y - 7.5f * spriteScale),
                    (LONG)(attachPoints[i]->x + 7.5f * spriteScale),
                    (LONG)(attachPoints[i]->y + 7.5f * spriteScale),
                };
                RECT overlappedRect;
                if (IntersectRect(&overlappedRect, &rect, &attachPointRect))
                {
                    for (auto& iter : m_SelectedSpriteBoxes)
                        if (iter.attachPoint == attachPoints[i])
                            return;
                    m_SelectedSpriteBoxes.emplace_back(nullptr, attachPoints[i], AttachPoint);
                }
            }

            if (!m_SelectedSpriteBoxes.empty())
            {
                if (!isDrag && !INPUT->KeyPress(VK_LCONTROL))
                {
                    SelectedBox selected = m_SelectedSpriteBoxes[m_OverlappedBoxSelectIndex % m_SelectedSpriteBoxes.size()];
                    m_SelectedSpriteBoxes.clear();
                    m_SelectedSpriteBoxes.push_back(selected);
                    m_OverlappedBoxSelectIndex++;
                }
                else
                    m_OverlappedBoxSelectIndex = 0;

                int types = 0;
                for (auto& iter : m_SelectedSpriteBoxes)
                    types |= iter.type;

                if (m_SelectedSpriteBoxes[0].type == types)
                    SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnSelectSpriteBox(m_SelectedSpriteBoxes[0].type, m_SelectedSpriteBoxes);
                else
                    SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnSelectSpriteBox(DrawType::Select, m_SelectedSpriteBoxes);
            }
            else
            {
                m_OverlappedBoxSelectIndex = 0;
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnSelectSpriteBox(DrawType::Select, m_SelectedSpriteBoxes);
            }

        }
        else if (m_SelectedDrawType != AttachPoint && m_DrawStartedPos.x != -1 && m_DrawStartedPos.y != -1 && isDrag)
        {
            DrawType type = m_SelectedDrawType;
            INPUT->Do([this, animationKey, frame, rect, type]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);

                switch(type)
                {
                case HurtBox:
                    {
                        cHurtBox* box = new cHurtBox();
                        box->SetRect(rect.left, rect.top, rect.right, rect.bottom);
                        AddBox<cHurtBox>(type, box);
                        break;
                    }
                case HitBox:
                    {
                        cHitBox* box = new cHitBox();
                        box->SetRect(rect.left, rect.top, rect.right, rect.bottom);
                        AddBox<cHitBox>(type, box);
                        break;
                    }
                case ThrowBox:
                    {
                        cThrowBox* box = new cThrowBox();
                        box->SetRect(rect.left, rect.top, rect.right, rect.bottom);
                        AddBox<cThrowBox>(type, box);
                        break;
                    }
                case BodyBox:
                    {
                        cBodyBox* box = new cBodyBox();
                        box->SetRect(rect.left, rect.top, rect.right, rect.bottom);
                        AddBox<cBodyBox>(type, box);
                        break;
                    }
                }
            }, [this, animationKey, frame]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);

                switch(m_SelectedDrawType)
                {
                case HurtBox: UndoAddBox<cHurtBox>(m_SelectedDrawType); break;
                case HitBox: UndoAddBox<cHitBox>(m_SelectedDrawType); break;
                case ThrowBox: UndoAddBox<cThrowBox>(m_SelectedDrawType); break;
                case BodyBox: UndoAddBox<cBodyBox>(m_SelectedDrawType); break;
                }
            });
        }
    }

    if (!m_SelectedSpriteBoxes.empty())
    {
        if (INPUT->KeyDown(VK_DELETE))
        {
            SerializedString serializedStr;
            std::vector<std::pair<DrawType, int>> typeAndIndex(m_SelectedSpriteBoxes.size());
            size_t size = 0;
            for (int i = 0; i < m_SelectedSpriteBoxes.size(); i++)
            {
                SelectedBox box = m_SelectedSpriteBoxes[i];
                if (box.box != nullptr)
                    size += box.box->GetSize();
                if (box.attachPoint != nullptr)
                    size += sizeof(::AttachPoint);
                typeAndIndex[i] = std::pair<DrawType, int>(box.type, GetBoxIndex(box));
            }

            std::sort(m_SelectedSpriteBoxes.begin(), m_SelectedSpriteBoxes.end(), [&](SelectedBox& _a, SelectedBox& _b)->bool{return GetBoxIndex(_a) < GetBoxIndex(_b);});
            serializedStr.SetSize(size);
            UINT pointer = 0;
            for (auto& iter : m_SelectedSpriteBoxes)
            {
                if (iter.box != nullptr)
                    iter.box->Serialize(const_cast<char*>(serializedStr.GetBuffer()), pointer);
                if (iter.attachPoint != nullptr)
                {
                    memcpy(const_cast<char*>(serializedStr.GetBuffer() + pointer), &iter.attachPoint, sizeof(::AttachPoint));
                    pointer += sizeof(::AttachPoint);
                }
            }

            INPUT->Do([this, animationKey, frame, typeAndIndex]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);
                std::vector<std::pair<DrawType, int>> capturedTypeAndIndex = typeAndIndex;
                for (auto& iter : capturedTypeAndIndex)
                {
                    switch(iter.first)
                    {
                    case HurtBox: DeleteBox<cHurtBox>(iter); break;
                    case HitBox: DeleteBox<cHitBox>(iter); break;
                    case ThrowBox: DeleteBox<cThrowBox>(iter); break;
                    case BodyBox: DeleteBox<cBodyBox>(iter); break;
                    case AttachPoint: DeleteBox<::AttachPoint>(iter); break;
                    }

                    for (int i = 0 ; i < capturedTypeAndIndex.size(); i++)
                    {
                        if (capturedTypeAndIndex[i].second > iter.second)
                            capturedTypeAndIndex[i].second--;
                    }
                }
                m_SelectedSpriteBoxes.clear();
            }, [this, animationKey, frame, typeAndIndex, serializedStr]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);
                std::vector<std::pair<DrawType, int>> capturedTypeAndIndex = typeAndIndex;
                std::sort(capturedTypeAndIndex.begin(), capturedTypeAndIndex.end(), [](std::pair<DrawType, int> _a, std::pair<DrawType, int> _b)->bool {return _a.second < _b.second;});
                UINT pointer = 0;
                for (auto& iter : capturedTypeAndIndex)
                {
                    switch(iter.first)
                    {
                    case HurtBox: UndoDeleteBox<cHurtBox>(iter, const_cast<char*>(serializedStr.GetBuffer()), pointer); break;
                    case HitBox: UndoDeleteBox<cHitBox>(iter, const_cast<char*>(serializedStr.GetBuffer()), pointer); break;
                    case ThrowBox: UndoDeleteBox<cThrowBox>(iter, const_cast<char*>(serializedStr.GetBuffer()), pointer); break;
                    case BodyBox: UndoDeleteBox<cBodyBox>(iter, const_cast<char*>(serializedStr.GetBuffer()), pointer); break;
                    case AttachPoint: UndoDeleteBox<::AttachPoint>(iter, const_cast<char*>(serializedStr.GetBuffer()), pointer); break;
                    }
                }
                m_SelectedSpriteBoxes.clear();
            });
        }

        if (INPUT->KeyPress(VK_LCONTROL))
        {
            if (INPUT->KeyDown('C'))
            {
                SerializedString serializedStr;
                SerializedVector<DrawType> types;
                size_t size = 0;
                for (int i = 0; i < m_SelectedSpriteBoxes.size(); i++)
                {
                    SelectedBox box = m_SelectedSpriteBoxes[i];
                    if (box.box != nullptr)
                        size += box.box->GetSize();
                    if (box.attachPoint != nullptr)
                        size += sizeof(::AttachPoint);
                    types.GetValue()->push_back(box.type);
                }
                
                UINT pointer = 0;
                serializedStr.SetSize(size + types.GetSize());
                types.Serialize(const_cast<char*>(serializedStr.GetBuffer()), pointer);
                for (auto& iter : m_SelectedSpriteBoxes)
                {
                    if (iter.box != nullptr)
                        iter.box->Serialize(const_cast<char*>(serializedStr.GetBuffer()), pointer);
                    if (iter.attachPoint != nullptr)
                    {
                        memcpy(const_cast<char*>(serializedStr.GetBuffer() + pointer), iter.attachPoint, sizeof(::AttachPoint));
                        pointer += sizeof(::AttachPoint);
                    }
                }

                INPUT->SetClipboard(CF_SPRITE_BOX, serializedStr);
            }
        }
    }

    if (INPUT->KeyDown('V'))
    {
        SerializedString serializedStr;
        if (INPUT->GetClipboard(CF_SPRITE_BOX, serializedStr))
        {
            UINT pointer = 0;
            SerializedVector<DrawType> types;
            types.Deserialize(const_cast<char*>(serializedStr.GetBuffer()), pointer);
            INPUT->Do([this, animationKey, frame, serializedStr, types, pointer]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);

                UINT capturedPointer = pointer;
                for (auto& iter : *types.GetValue())
                {
                    switch(iter)
                    {
                    case HurtBox:
                        {
                            cHurtBox* box = new cHurtBox();
                            box->Deserialize(const_cast<char*>(serializedStr.GetBuffer()), capturedPointer);
                            AddBox<cHurtBox>(iter, box);
                            break;
                        }
                    case HitBox:
                        {
                            cHitBox* box = new cHitBox();
                            box->Deserialize(const_cast<char*>(serializedStr.GetBuffer()), capturedPointer);
                            AddBox<cHitBox>(iter, box);
                            break;
                        }
                    case ThrowBox:
                        {
                            cThrowBox* box = new cThrowBox();
                            box->Deserialize(const_cast<char*>(serializedStr.GetBuffer()), capturedPointer);
                            AddBox<cThrowBox>(iter, box);
                            break;
                        }
                    case BodyBox:
                        {
                            cBodyBox* box = new cBodyBox();
                            box->Deserialize(const_cast<char*>(serializedStr.GetBuffer()), capturedPointer);
                            AddBox<cBodyBox>(iter, box);
                            break;
                        }
                    case AttachPoint:
                        {
                            ::AttachPoint* attachPoint = new ::AttachPoint();
                            memcpy(attachPoint, serializedStr.GetBuffer() + capturedPointer, sizeof(::AttachPoint));
                            capturedPointer += sizeof(::AttachPoint);
                            AddBox<::AttachPoint>(iter, attachPoint);
                            break;
                        }
                    }
                }
            }, [this, animationKey, frame, types]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetFrameList()->SetFrame(frame);

                for (auto& iter : *types.GetValue())
                {
                    switch(iter)
                    {
                    case HurtBox: UndoAddBox<cHurtBox>(iter); break;
                    case HitBox: UndoAddBox<cHitBox>(iter); break;
                    case ThrowBox: UndoAddBox<cThrowBox>(iter); break;
                    case BodyBox: UndoAddBox<cBodyBox>(iter); break;
                    case AttachPoint: UndoAddBox<::AttachPoint>(iter); break;
                    }
                }
            });
        }
    }
}

void cSpriteBoxArea::Render()
{
    cGUI::Render();
    if (m_CurSprite == nullptr)
        return;

    Texture* pixel = IMAGE->GetTexture("Pixel");
    float scale = SCENE->GetScene<cEditorScene>()->GetCharacterData()->GetSpriteScale();

    if (m_SelectedDrawType == AttachPoint && IsMouseOver())
    {
        IMAGE->RenderSprite(pixel, Vec3(INPUT->GetMousePos().x, INPUT->GetMousePos().y, 0.1f), 0, Vec2(15, 2) * scale, Vec2(0.5f, 0.5f), 0xaf000000, true);
        IMAGE->RenderSprite(pixel, Vec3(INPUT->GetMousePos().x, INPUT->GetMousePos().y, 0.1f), 0, Vec2(2, 15) * scale, Vec2(0.5f, 0.5f), 0xaf000000, true);
    }
    else
    {
        if (m_IsClicked && m_DrawStartedPos.x != -1)
        {
            Vec2 drawEndedPos = INPUT->GetMousePos();
            RECT rect = RECT
            {
                (LONG)min(m_DrawStartedPos.x, drawEndedPos.x),
                (LONG)min(m_DrawStartedPos.y, drawEndedPos.y),
                (LONG)max(m_DrawStartedPos.x, drawEndedPos.x),
                (LONG)max(m_DrawStartedPos.y, drawEndedPos.y),
            };

            IMAGE->RenderSprite(pixel, Vec3((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f, 0.49f), 0,
                Vec2(rect.right - rect.left, rect.bottom - rect.top),  Vec2(0.5f, 0.5f), GetBoxColor(m_SelectedDrawType, true), true);
        }
    }
    
    WithBoxes([&](cSpriteBox* _box, DrawType _type)->void
    {
        float left = _box->GetLeft() * scale;
        float top = _box->GetTop() * scale;
        float right = _box->GetRight() * scale;
        float bottom = _box->GetBottom() * scale;
        Vec2 size = Vec2(right - left, bottom - top);
        Vec3 center = EditorSpriteCenterPos + Vec3((right + left) * 0.5f, (bottom + top) * 0.5f, -0.01f);

        bool isSelected = false;
        for (auto& iter : m_SelectedSpriteBoxes)
        {
            if (iter.box != _box)
                continue;
            isSelected = true;
            break;
        }
        IMAGE->RenderSprite(pixel, center, 0, size,  Vec2(0.5f, 0.5f), GetBoxColor(_type, isSelected), true);
        
        IMAGE->RenderSprite(pixel, center + Vec3(0, size.y * 0.5f - scale * 0.5f, 0), 0, Vec2(size.x, scale),  Vec2(0.5f, 0.5f), GetBoxColor(_type, false) | 0x9f000000, true);
        IMAGE->RenderSprite(pixel, center - Vec3(0, size.y * 0.5f - scale * 0.5f, 0), 0, Vec2(size.x, scale),  Vec2(0.5f, 0.5f), GetBoxColor(_type, false) | 0x9f000000, true);
        IMAGE->RenderSprite(pixel, center + Vec3(size.x * 0.5f - scale * 0.5f, 0, 0), 0, Vec2(scale, size.y),  Vec2(0.5f, 0.5f), GetBoxColor(_type, false) | 0x9f000000, true);
        IMAGE->RenderSprite(pixel, center - Vec3(size.x * 0.5f - scale * 0.5f, 0, 0), 0, Vec2(scale, size.y),  Vec2(0.5f, 0.5f), GetBoxColor(_type, false) | 0x9f000000, true);
    });

    ::AttachPoint** attachPoints;
    int count = m_CurSprite->GetAttachPoints(attachPoints);
    for (int i = 0; i < count; i++)
    {
        D3DCOLOR color = 0x9f000000;
        for (auto& iter : m_SelectedSpriteBoxes)
        {
            if (iter.attachPoint == attachPoints[i])
            {
                color = 0x9fff0000;
                break;
            }
        }
        Vec2 pos = Vec2(EditorSpriteCenterPos.x + attachPoints[i]->x * scale, EditorSpriteCenterPos.y + attachPoints[i]->y * scale);
        IMAGE->RenderSprite(pixel, Vec3(pos.x, pos.y, 0.48f), 0, Vec2(15, 2) * scale, Vec2(0.5f, 0.5f), color, true);
        IMAGE->RenderSprite(pixel, Vec3(pos.x, pos.y, 0.48f), 0, Vec2(2, 15) * scale, Vec2(0.5f, 0.5f), color, true);
        IMAGE->RenderText("Gothic", AttachPointTypeToStringMap[(int)attachPoints[i]->type], Vec3(pos.x, pos.y + 7 * scale, 0.48f), 0, Vec2(0.25f, 0.25f) * scale, Vec2(0.5f, 0), color, true);
    }
}

void cSpriteBoxArea::Release()
{
    cGUI::Release();
}

void cSpriteBoxArea::OnCollision(cObject* _other)
{
    cGUI::OnCollision(_other);
}

void cSpriteBoxArea::OnAlarm(std::string _key)
{
    cGUI::OnAlarm(_key);
}

void cSpriteBoxArea::OnClick()
{
    cGUI::OnClick();
}

void cSpriteBoxArea::OnDoubleClick()
{
    cGUI::OnDoubleClick();
}

void cSpriteBoxArea::OnClickUp()
{
    cGUI::OnClickUp();
}

void cSpriteBoxArea::OnClickOutSide()
{
    cGUI::OnClickOutSide();
}

void cSpriteBoxArea::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_SelectedSpriteBoxes.empty())
        return;
    
    if (uMsg == WM_KEYDOWN)
    {
        int dist = 1;
        if (INPUT->KeyPress(VK_LSHIFT))
            dist = 8;

        Vec2 moveDelta = Vec2(0, 0);
        Vec2 lt = Vec2(0, 0);
        Vec2 rb = Vec2(0, 0);
        if (m_SelectedSpriteBoxes.size() > 1)
        {
            switch(wParam)
            {
            case VK_LEFT: moveDelta.x -= dist; break;
            case VK_RIGHT: moveDelta.x += dist; break;
            case VK_UP: moveDelta.y -= dist; break;
            case VK_DOWN: moveDelta.y += dist; break;
            }
        }
        else
        {
            switch(wParam)
            {
            case VK_LEFT: (INPUT->KeyPress(VK_LCONTROL) ? lt : rb).x -= dist; break;
            case VK_RIGHT: (INPUT->KeyPress(VK_LCONTROL) ? lt : rb).x += dist; break;
            case VK_UP: (INPUT->KeyPress(VK_LCONTROL) ? lt : rb).y -= dist; break;
            case VK_DOWN: (INPUT->KeyPress(VK_LCONTROL) ? lt : rb).y += dist; break;
            }
        }

        for (auto& iter : m_SelectedSpriteBoxes)
        {
            if (iter.box != nullptr)
            {
                iter.box->SetLeft(iter.box->GetLeft() + moveDelta.x + lt.x);
                iter.box->SetTop(iter.box->GetTop() + moveDelta.y + lt.y);
                iter.box->SetRight(iter.box->GetRight() + moveDelta.x + rb.x);
                iter.box->SetBottom(iter.box->GetBottom() + moveDelta.y + rb.y);
            }
            if (iter.attachPoint != nullptr)
            {
                iter.attachPoint->x += moveDelta.x + lt.x + rb.x;
                iter.attachPoint->y += moveDelta.y + lt.y + rb.y;
            }
        }
    }
}

template <typename T>
void cSpriteBoxArea::GetBoxGetSetFunc(DrawType _type, std::function<int(T**&)>& _getFunc, std::function<void(T**, int)>& _setFunc)
{
    switch (_type)
    {
    case HurtBox:
        _getFunc = [&](T**& _boxes)->int{return m_CurSprite->GetHurtBoxes((cHurtBox**&)_boxes);};
        _setFunc = [&](T** _boxes, int _length)->void{m_CurSprite->SetHurtBoxes((cHurtBox**)_boxes, _length);};
        return;
    case HitBox:
        _getFunc = [&](T**& _boxes)->int{return m_CurSprite->GetHitBoxes((cHitBox**&)_boxes);};
        _setFunc = [&](T** _boxes, int _length)->void{m_CurSprite->SetHitBoxes((cHitBox**)_boxes, _length);};
        return;
    case ThrowBox:
        _getFunc = [&](T**& _boxes)->int{return m_CurSprite->GetThrowBoxes((cThrowBox**&)_boxes);};
        _setFunc = [&](T** _boxes, int _length)->void{m_CurSprite->SetThrowBoxes((cThrowBox**)_boxes, _length);};
        return;
    case BodyBox:
        _getFunc = [&](T**& _boxes)->int{return m_CurSprite->GetBodyBoxes((cBodyBox**&)_boxes);};
        _setFunc = [&](T** _boxes, int _length)->void{m_CurSprite->SetBodyBoxes((cBodyBox**)_boxes, _length);};
        return;
    case AttachPoint:
        _getFunc = [&](T**& _boxes)->int{return m_CurSprite->GetAttachPoints((::AttachPoint**&)_boxes);};
        _setFunc = [&](T** _boxes, int _length)->void{m_CurSprite->SetAttachPoints((::AttachPoint**)_boxes, _length);};
        return;
    }
}

template <typename T>
void cSpriteBoxArea::DeleteBox(std::pair<DrawType, int> _typeAndIndex)
{
    std::function<int(T**&)> getFunc;
    std::function<void(T**, int)> setFunc;
    GetBoxGetSetFunc(_typeAndIndex.first, getFunc, setFunc);

    T** prevBoxes;
    int count = getFunc(prevBoxes);
    std::vector<T*> vec(count);
    for (int i = 0; i < count; i++)
        vec[i] = prevBoxes[i];
    delete vec[_typeAndIndex.second];
    vec.erase(vec._Make_iterator_offset(_typeAndIndex.second));
    T** newBoxes = new T*[count - 1];
    memcpy(newBoxes, vec.data(), sizeof(T*) * (count - 1));
    setFunc(newBoxes, count - 1);
}

template <typename T>
void cSpriteBoxArea::UndoDeleteBox(std::pair<DrawType, int> _typeAndIndex, char* _buffer, UINT& _pointer)
{
    std::function<int(T**&)> getFunc;
    std::function<void(T**, int)> setFunc;
    GetBoxGetSetFunc(_typeAndIndex.first, getFunc, setFunc);

    T** prevBoxes;
    int count = getFunc(prevBoxes);
    std::vector<T*> vec(count);
    for (int i = 0; i < count; i++)
        vec[i] = prevBoxes[i];
    T* box = new T();
    if (_typeAndIndex.first != AttachPoint)
        box->Deserialize(_buffer, _pointer);
    else
    {
        memcpy(box, _buffer, sizeof(::AttachPoint));
        _pointer += sizeof(::AttachPoint);
    }
    vec.insert(vec._Make_iterator_offset(_typeAndIndex.second), box);
    T** newBoxes = new T*[count + 1];
    memcpy(newBoxes, vec.data(), sizeof(T*) * (count + 1));
    setFunc(newBoxes, count + 1);
}

template <typename T>
void cSpriteBoxArea::AddBox(DrawType _type, T* _box)
{
    std::function<int(T**&)> getFunc;
    std::function<void(T**, int)> setFunc;
    GetBoxGetSetFunc(_type, getFunc, setFunc);

    T** pointer;
    int count = getFunc(pointer) + 1;
    T** newPointer = new T*[count];
    if (pointer != nullptr)
        memcpy(newPointer, pointer, sizeof(T*) * count - 1);
    
    newPointer[count - 1] = _box;
    setFunc(newPointer, count); 
}

template <typename T>
void cSpriteBoxArea::UndoAddBox(DrawType _type)
{
    std::function<int(T**&)> getFunc;
    std::function<void(T**, int)> setFunc;
    GetBoxGetSetFunc(_type, getFunc, setFunc);

    T** pointer;
    int count = getFunc(pointer) - 1;
    T** newPointer = new T*[count];
    if (pointer != nullptr)
    {
        delete pointer[count];
        memcpy(newPointer, pointer, sizeof(T*) * count);
    }
    setFunc(newPointer, count); 
}


D3DCOLOR cSpriteBoxArea::GetBoxColor(DrawType _type, bool _isSelected)
{
    switch(_type)
    {
    case Select: return 0x80ffffff;
    case HurtBox: return _isSelected ? 0x6f0050ff : 0x2f0050ff;
    case HitBox: return _isSelected ? 0x4fff0000 : 0x1fff0000;
    case ThrowBox: return _isSelected ? 0x4fff00ff : 0x1fff00ff;
    case BodyBox: return _isSelected ? 0x6fffff00 : 0x2fffff00;
    case AttachPoint: return _isSelected ? 0x6fa0a0a0 : 0x2fa0a0a0;
    }
}

void cSpriteBoxArea::WithBoxes(const std::function<void(cSpriteBox*, DrawType)>& _func)
{
    cHurtBox** hurtBoxes;
    for (int i = 0; i < m_CurSprite->GetHurtBoxes(hurtBoxes); i++)
        _func(hurtBoxes[i], HurtBox);
    cHitBox** hitBoxes;
    for (int i = 0; i < m_CurSprite->GetHitBoxes(hitBoxes); i++)
        _func(hitBoxes[i], HitBox);
    cThrowBox** throwBoxes;
    for (int i = 0; i < m_CurSprite->GetThrowBoxes(throwBoxes); i++)
        _func(throwBoxes[i], ThrowBox);
    cBodyBox** bodyBoxes;
    for (int i = 0; i < m_CurSprite->GetBodyBoxes(bodyBoxes); i++)
        _func(bodyBoxes[i], BodyBox);
}

int cSpriteBoxArea::GetBoxIndex(SelectedBox _box)
{
    switch(_box.type)
    {
    case HitBox:
        {
            cHitBox** pointer;
            int count = m_CurSprite->GetHitBoxes(pointer);
            for (int i = 0; i < count;  i++)
                if (pointer[i] == _box.box)
                    return i;
        }

    case HurtBox:
        {
            cHurtBox** pointer;
            int count = m_CurSprite->GetHurtBoxes(pointer);
            for (int i = 0; i < count;  i++)
                if (pointer[i] == _box.box)
                    return i;
        }

    case ThrowBox:
        {
            cThrowBox** pointer;
            int count = m_CurSprite->GetThrowBoxes(pointer);
            for (int i = 0; i < count;  i++)
                if (pointer[i] == _box.box)
                    return i;
        }

    case BodyBox:
        {
            cBodyBox** pointer;
            int count = m_CurSprite->GetBodyBoxes(pointer);
            for (int i = 0; i < count;  i++)
                if (pointer[i] == _box.box)
                    return i;
        }

    case AttachPoint:
        {
            ::AttachPoint** pointer;
            int count = m_CurSprite->GetAttachPoints(pointer);
            for (int i = 0; i < count;  i++)
                if (pointer[i] == _box.attachPoint)
                    return i;
        }
    }

    return -1;
}

void cSpriteBoxArea::SetSprite(cCharacterSprite* _sprite)
{
    m_CurSprite = _sprite;
    m_SelectedSpriteBoxes.clear();
    m_OverlappedBoxSelectIndex = 0;
}

void cSpriteBoxArea::WithSelectedBoxes(std::function<void(SelectedBox)> _func) const
{
    for (auto& iter : m_SelectedSpriteBoxes)
        _func(iter);
}
