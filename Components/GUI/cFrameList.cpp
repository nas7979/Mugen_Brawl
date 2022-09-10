#include "DXUT.h"
#include "cFrameList.h"

#include "../../Scenes/Editor/cEditorScene.h"
#include "EditorPanels/cEditorSpritePanel.h"

cFrameList::~cFrameList()
{
}

void cFrameList::Init()
{
    cGUI::Init();
    SetAnimation(nullptr);
    SetFrame(0);
}

void cFrameList::Update()
{
    cGUI::Update();

    if (!CanEdit() || SCENE->GetScene<cEditorScene>()->GetSpritePanel()->HasFocusedGUI())
        return;

    if (INPUT->KeyDown(VK_SPACE))
    {
        m_isPlaying = !m_isPlaying;
    }

    if (m_isPlaying)
    {
        m_FrameTimer++;
        if (m_FrameTimer >= m_CurAnimation->GetSprite(GetFrame())->GetFrameLength())
        {
            SetFrame((GetFrame() + 1) % m_CurAnimation->GetLength());
            m_FrameTimer = 0;
        }
    }

    int cursorFrame = CursorPosToFrame();
    if (m_IsClicked)
    {
        if (m_CanMoveFrames)
        {
            Vec2 localCursorPos = INPUT->GetMousePos() - Vec2(m_Owner->GetPos().x - m_Size.x * 0.5f + m_ScrollPos, m_Owner->GetPos().y - m_Size.y * 0.5f);
            m_MoveFramesTo = Clamp(floor(localCursorPos.x / m_ElementWidth), 0, m_CurAnimation->GetLength());
        }
        else
        {
            if (cursorFrame != m_DragStartedFrame)
            {
                m_SelectedFrames.clear();
                if (cursorFrame >= m_DragStartedFrame)
                {
                    for (int i = m_DragStartedFrame; i <= cursorFrame; i++)
                        m_SelectedFrames.push_back(i);
                }
                else
                {
                    for (int i = m_DragStartedFrame; i >= cursorFrame; i--)
                        m_SelectedFrames.push_back(i);
                }

                int left = -m_ScrollPos;
                int right = m_Size.x - m_ScrollPos;
                int pos = m_SelectedFrames[m_SelectedFrames.size() - 1] * m_ElementWidth;
                if (left >= pos)
                    SetScrollPos(-pos);
                else if (right <= pos)
                    SetScrollPos(m_Size.x - pos - m_ElementWidth);

                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
            }
        }
    }
    else
    {
        if (m_MoveFramesTo >= 0)
        {
            if (m_MoveFramesTo != m_SelectedFrames[m_SelectedFrames.size() - 1])
            {
                if (m_SelectedFrames[0] > m_MoveFramesTo || m_SelectedFrames[m_SelectedFrames.size() - 1] < m_MoveFramesTo)
                {
                    std::string animationKey = m_CurAnimation->GetKey();
                    int moveTo = m_MoveFramesTo;
                    std::vector<int> selectedFrames = m_SelectedFrames;
                    std::vector<int> unsortedSelectedFrames = m_SelectedFrames;
                    std::sort(selectedFrames.begin(), selectedFrames.end());
                    INPUT->Do([this, animationKey, moveTo, selectedFrames]()->void
                    {
                        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                        std::vector<cCharacterSprite*> sprites(selectedFrames.size());
                        for (int i = 0; i < selectedFrames.size(); i++)
                        {
                            sprites[i] = m_CurAnimation->GetSprite(selectedFrames[i] - i);
                            m_CurAnimation->RemoveSprite(selectedFrames[i] - i, false);
                        }
                        int capturedMoveTo = moveTo;
                        if (capturedMoveTo > selectedFrames[0])
                            capturedMoveTo -= selectedFrames.size();
                        m_CurAnimation->InsertSprites(sprites, capturedMoveTo);
                        m_SelectedFrames.clear();
                        for (int i = 0; i < sprites.size(); i++)
                            m_SelectedFrames.push_back(capturedMoveTo + i);
                        
                        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
                    },[this, animationKey, moveTo, selectedFrames,  unsortedSelectedFrames]()->void
                    {
                        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                        std::vector<cCharacterSprite*> sprites(selectedFrames.size());
                        int capturedMoveTo = moveTo;
                        if (capturedMoveTo > selectedFrames[0])
                            capturedMoveTo -= selectedFrames.size();
                        for (int i = 0; i < selectedFrames.size(); i++)
                        {
                            sprites[i] = m_CurAnimation->GetSprite(capturedMoveTo);
                            m_CurAnimation->RemoveSprite(capturedMoveTo, false);
                        }
                        for (int i = 0; i < selectedFrames.size(); i++)
                            m_CurAnimation->InsertSprite(sprites[i], selectedFrames[i]);

                        m_SelectedFrames = unsortedSelectedFrames;
                        
                        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
                    });
                }
            }
            m_MoveFramesTo = -1;
        }
    }

    if (!m_SelectedFrames.empty())
    {
        if (INPUT->KeyDown(VK_DELETE) && !SCENE->GetScene<cEditorScene>()->GetSpritePanel()->GetSpriteBoxArea()->HasSelectedBox())
        {
            std::string animationKey = m_CurAnimation->GetKey();
            std::vector<int> selectedFrames = m_SelectedFrames;
            std::vector<int> unsortedSelectedFrames = m_SelectedFrames;
            std::vector<std::string> serializedSprites(m_SelectedFrames.size());
            int draggedFramesLeft = m_DraggedFramesLeft;
            int draggedFramesRight = m_DraggedFramesRight;
            std::sort(selectedFrames.begin(), selectedFrames.end());
            for (int i = 0; i < m_SelectedFrames.size(); i++)
            {
                UINT pointer = 0;
                serializedSprites[i].resize(m_CurAnimation->GetSprite(m_SelectedFrames[i])->GetSize());
                m_CurAnimation->GetSprite(m_SelectedFrames[i])->Serialize(const_cast<char*>(serializedSprites[i].data()), pointer);
            }
            
            INPUT->Do([this, animationKey, selectedFrames]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                int prevFrame = selectedFrames[selectedFrames.size() - 1];
                for (int i = 0; i < selectedFrames.size(); i++)
                    m_CurAnimation->RemoveSprite(selectedFrames[i] - i);
                m_SelectedFrames.clear();
                m_SelectedFrames.push_back(min(prevFrame, m_CurAnimation->GetLength() - 1));
                m_DraggedFramesLeft = m_DraggedFramesRight = -1;

                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
            }, [this, animationKey, selectedFrames, unsortedSelectedFrames, serializedSprites, draggedFramesLeft, draggedFramesRight]()->void
            {
                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animationKey);
                for (int i = 0; i < selectedFrames.size(); i++)
                {
                    cCharacterSprite* sprite = new cCharacterSprite();
                    UINT pointer = 0;
                    sprite->Deserialize(const_cast<char*>(serializedSprites[i].c_str()), pointer);
                    m_CurAnimation->InsertSprite(sprite, selectedFrames[i]);
                }
                m_SelectedFrames = unsortedSelectedFrames;
                m_DraggedFramesLeft = draggedFramesLeft;
                m_DraggedFramesRight = draggedFramesRight;

                SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
            });

        }

        if (m_DraggedFramesLeft > 0)
        {
            Vec2 mousePos = INPUT->GetMousePos();
            Vec2 outerMin = Vec2(m_DraggedFramesLeft - 3, m_Owner->GetPos().y - m_Size.y * 0.5f);
            Vec2 outerMax = Vec2(m_DraggedFramesRight + 3, m_Owner->GetPos().y - m_Size.y * 0.5f + m_ElementHeight);
            Vec2 innerMin = outerMin + Vec2(6, 6);
            Vec2 innerMax = outerMax - Vec2(6, 6);
            bool overlapOuter = mousePos.x >= outerMin.x && mousePos.y >= outerMin.y && mousePos.x <= outerMax.x && mousePos.y <= outerMax.y;
            bool overlapInner = mousePos.x >= innerMin.x && mousePos.y >= innerMin.y && mousePos.x <= innerMax.x && mousePos.y <= innerMax.y;
            m_CanMoveFrames = m_MoveFramesTo >= 0 || overlapOuter && !overlapInner;
        }
        else
        {
            m_CanMoveFrames = false;
        }

        if (INPUT->KeyPress(VK_LCONTROL))
        {
            if (INPUT->KeyDown('C'))
                CopyFrames();
            if (INPUT->KeyDown('V'))
                PasteFrames();
            if (INPUT->KeyDown('H'))
            {
                for (int i = 0; i < m_SelectedFrames.size(); i++)
                    m_CurAnimation->GetSprite(m_SelectedFrames[i])->FlipHorizontal();
            }
        }
    }
}

void cFrameList::Render()
{
    IMAGE->End();
    IMAGE->Begin();
    CAMERA->Render();
    g_Device->SetRenderState(D3DRS_STENCILENABLE, true);
    g_Device->SetRenderState(D3DRS_STENCILREF, m_Owner->GetUID() % 255);
    g_Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    g_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    g_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    g_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
    g_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    g_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    cGUI::Render();

    IMAGE->End();
    IMAGE->Begin();
    CAMERA->Render();
    g_Device->SetRenderState(D3DRS_STENCILENABLE, true);
    g_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
    g_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    
    Texture* pixel = IMAGE->GetTexture("Pixel");
    Vec3 basePos = m_Owner->GetPos() - Vec3(m_Size.x, m_Size.y - m_ElementHeight, 0) * 0.5f + Vec3(m_ScrollPos, 0, 0);
    int min = 100000;
    int max = 0;
    if (m_CurAnimation != nullptr)
    {
        Vec3 pos = basePos;
        for(int i = 0; i < m_CurAnimation->GetLength(); i++)
        {
            pos.x += m_ElementWidth * 0.5f;
            D3DXCOLOR color = 0xffffffff;
            for (int frame = 0; frame < m_SelectedFrames.size(); frame++)
            {
                if (m_SelectedFrames[frame] == i)
                {
                    color = frame == m_SelectedFrames.size() - 1 ? 0xff909090 : 0xffc0c0c0;
                    break;
                }
            }
            IMAGE->RenderSprite(pixel, pos, 0, Vec2(m_ElementWidth, m_ElementHeight), Vec2(0.5f,0.5f), color, true);
            IMAGE->RenderText("Gothic", std::to_string(i + 1), pos, 0, Vec2(0.3f, 0.3f), Vec2(0.5f,0.5f), 0xff000000, true);

            IMAGE->RenderSprite(pixel, pos - Vec3(0, m_ElementHeight * 0.5f, 0), 0, Vec2(m_ElementWidth, 1), Vec2(0.5,0.5), m_OutlineColor, true);
            IMAGE->RenderSprite(pixel, pos + Vec3(0, m_ElementHeight * 0.5f, 0), 0, Vec2(m_ElementWidth, 1), Vec2(0.5,0.5), m_OutlineColor, true);
            IMAGE->RenderSprite(pixel, pos - Vec3(m_ElementWidth * 0.5f, 0, 0), 0, Vec2(1, m_ElementHeight), Vec2(0.5,0.5), m_OutlineColor, true);
            IMAGE->RenderSprite(pixel, pos + Vec3(m_ElementWidth * 0.5f, 0, 0), 0, Vec2(1, m_ElementHeight), Vec2(0.5,0.5), m_OutlineColor, true);
            
            pos.x += m_ElementWidth * 0.5f;
        }
        
        for (auto& iter : m_SelectedFrames)
        {
            if (iter < min)
                min = iter;
            if (iter > max)
                max = iter;
        }

        int totalWidth = pos.x - basePos.x;
        int scrollbarWidth = m_Size.x / (totalWidth / m_Size.x);

        IMAGE->RenderSprite(pixel, m_Owner->GetPos() + Vec3(-m_Size.x * 0.5f - m_ScrollPos / totalWidth * m_Size.x, m_Size.y * 0.5f, -0.1f),
            0, Vec2(scrollbarWidth, m_ScrollbarHeight), Vec2(0, 1), 0x90000000, true);

        if (m_CurAnimation->GetLength() > 0)
        {
            int selectedLeft = basePos.x + min * m_ElementWidth;
            int length = (max - min + 1) * m_ElementWidth;
            D3DCOLOR color = m_CanMoveFrames ? 0xffff0000 : 0xffffff00;
            IMAGE->RenderSprite(pixel, Vec3(selectedLeft, basePos.y - round(m_ElementWidth * 0.5f) - 1, 0), 0, Vec2(length, 2), Vec2(0,0.5), color, true);
            IMAGE->RenderSprite(pixel, Vec3(selectedLeft, basePos.y + round(m_ElementWidth * 0.5f) + 1, 0), 0, Vec2(length, 2), Vec2(0,0.5), color, true);
            IMAGE->RenderSprite(pixel, Vec3(selectedLeft - 1, basePos.y, 0), 0, Vec2(2, m_ElementHeight), Vec2(0,0.5), color, true);
            IMAGE->RenderSprite(pixel, Vec3(selectedLeft + length - 1, basePos.y, 0), 0, Vec2(2, m_ElementHeight), Vec2(0,0.5), color, true);

            if (!INPUT->KeyPress(VK_LBUTTON))
            {
                m_DraggedFramesLeft = selectedLeft;
                m_DraggedFramesRight = basePos.x + (max + 1) * m_ElementWidth;
            }

            if (m_MoveFramesTo >= 0)
            {
                IMAGE->RenderSprite(pixel, Vec3(basePos.x + m_MoveFramesTo * m_ElementWidth, basePos.y, 0), 0, Vec2(4, 100), Vec2(0.5,0.5), 0xffff0000, true);
            }
        }
    }

    IMAGE->End();
    IMAGE->Begin();
    CAMERA->Render();
    g_Device->SetRenderState(D3DRS_STENCILENABLE, false);

    IMAGE->RenderSprite(pixel, m_Owner->GetPos() - Vec3(0, m_Size.y * 0.5f - 1, 0), 0, Vec2(m_Size.x, 2), Vec2(0.5,0.5), m_OutlineColor, true);
    IMAGE->RenderSprite(pixel, m_Owner->GetPos() + Vec3(0, m_Size.y * 0.5f - 1, 0), 0, Vec2(m_Size.x, 2), Vec2(0.5,0.5), m_OutlineColor, true);
    IMAGE->RenderSprite(pixel, m_Owner->GetPos() - Vec3(m_Size.x * 0.5f - 1, 0, 0), 0, Vec2(2, m_Size.y), Vec2(0.5,0.5), m_OutlineColor, true);
    IMAGE->RenderSprite(pixel, m_Owner->GetPos() + Vec3(m_Size.x * 0.5f - 1, 0, 0), 0, Vec2(2, m_Size.y), Vec2(0.5,0.5), m_OutlineColor, true);
}

void cFrameList::Release()
{
    cGUI::Release();
}

void cFrameList::OnCollision(cObject* _other)
{
    cGUI::OnCollision(_other);
}

void cFrameList::OnAlarm(std::string _key)
{
    cGUI::OnAlarm(_key);
}

void cFrameList::OnClick()
{
    cGUI::OnClick();
    if (m_CanMoveFrames)
    {
        return;
    }
    
    m_DragStartedFrame = CursorPosToFrame();
    
    if (!INPUT->KeyPress(VK_LCONTROL))
        m_SelectedFrames.clear();
    
    m_SelectedFrames.push_back(m_DragStartedFrame);
    m_DraggedFramesLeft = m_DraggedFramesRight = -1;
    SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
}

void cFrameList::OnDoubleClick()
{
    cGUI::OnDoubleClick();
}

void cFrameList::OnClickUp()
{
    cGUI::OnClickUp();
}

void cFrameList::OnClickOutSide()
{
    cGUI::OnClickOutSide();
}

void cFrameList::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

int cFrameList::CursorPosToFrame()
{
    if (m_CurAnimation == nullptr)
        return -1;
    Vec2 localCursorPos = INPUT->GetMousePos() - Vec2(m_Owner->GetPos().x - m_Size.x * 0.5f + m_ScrollPos, m_Owner->GetPos().y - m_Size.y * 0.5f);
    return Clamp(floor(localCursorPos.x / m_ElementWidth), 0, m_CurAnimation->GetLength() - 1);
}

void cFrameList::SetAnimation(cCharacterAnimation* _anim)
{
    m_CurAnimation = _anim;
    m_SelectedFrames.clear();
    SetFrame(0);
}

void cFrameList::SetFrame(int _frame)
{
    m_SelectedFrames.clear();
    m_DragStartedFrame = -1;
    if (m_CurAnimation == nullptr)
        return;
    
    m_SelectedFrames.push_back(_frame);

    int left = -m_ScrollPos;
    int right = m_Size.x - m_ScrollPos;
    int pos = _frame * m_ElementWidth;
    if (left >= pos)
        SetScrollPos(-pos);
    else if (right <= pos)
        SetScrollPos(m_Size.x - pos - m_ElementWidth);

    SCENE->GetScene<cEditorScene>()->GetSpritePanel()->OnFrameChanged();
}

int cFrameList::GetFrame()
{
    if (m_CurAnimation == nullptr || m_SelectedFrames.empty())
        return 0;
    
    return m_SelectedFrames[m_SelectedFrames.size() - 1];
}

void cFrameList::CopyFrames()
{
    std::vector<int> sortedFrames = m_SelectedFrames;
    std::sort(sortedFrames.begin(), sortedFrames.end());
    
    DynamicSerializedVector<cCharacterSprite*> serializedSprites;
    for (int i = 0; i < sortedFrames.size(); i++)
        serializedSprites.GetValue()->push_back(m_CurAnimation->GetSprite(sortedFrames[i]));
    
    INPUT->SetClipboard(CF_SPRITE, serializedSprites);
}

void cFrameList::PasteFrames()
{
    DynamicSerializedVector<cCharacterSprite*> sprites;
    if (!INPUT->GetClipboard(CF_SPRITE, sprites))
        return;

    int insertTo = GetFrame();
    std::string animKey = m_CurAnimation->GetKey();
    INPUT->Do([this, sprites, insertTo, animKey]()->void
    {
        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animKey);
        m_CurAnimation->InsertSprites(*sprites.GetValue(), insertTo);
        SetFrame(insertTo);
    }, [this, sprites, insertTo, animKey]()->void
    {
        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->SetAnimationByKey(animKey);
        for (int i = 0; i < sprites.GetLength(); i++)
            m_CurAnimation->RemoveSprite(insertTo);
        SetFrame(insertTo);
    });
}

void cFrameList::SetScrollPos(float _pos)
{
    m_ScrollPos = Clamp(_pos, -(m_CurAnimation->GetLength() * m_ElementWidth - min(m_Size.x, m_CurAnimation->GetLength() * m_ElementWidth)), 0);
}

void cFrameList::WithSelectedFrames(std::function<void(cCharacterSprite* _sprite)> _func)
{
    for (auto& iter : m_SelectedFrames)
        _func(m_CurAnimation->GetSprite(iter));
}
