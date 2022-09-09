#include "DXUT.h"
#include "cScrollView.h"

cScrollView::~cScrollView()
{
}

void cScrollView::Init()
{
    cGUI::Init();
}

void cScrollView::Update()
{
    cGUI::Update();
    if (m_SelectedIndex >= 0 && m_Elements.size() > 0)
    {
        if (INPUT->KeyDown(VK_RETURN) && m_OnClick != nullptr)
        {
            m_OnClick(m_Elements[m_SelectedIndex]);
            m_SelectedIndex = -1;   
        }
    }
}

void cScrollView::Render()
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
    Vec3 startPos = m_Owner->GetPos() - Vec3(m_Size.x, m_Size.y, 0) * 0.5f + Vec3(4, m_ScrollPos, -0.01f);
    int x = 0;
    int y = 0;
    for(int i = 0; i < m_Elements.size(); i++)
    {
        Vec3 pos = m_Elements[i]->GetPos() - m_ElementsOffset;
        if (i % 2 == 0)
            IMAGE->RenderSprite(pixel, pos, 0, m_ElementSize, Vec2(0.5f,0.5f), 0x20000000, true);

        x += m_ElementSize.x;
        if (x > m_Size.x)
        {
            x = 0;
            y += m_ElementSize.y;
        }
    }

    for (auto& iter : m_Elements)
    {
        iter->Render();
    }
    if (m_SelectedIndex >= 0 && m_SelectedIndex < m_Elements.size())
        IMAGE->RenderSprite(pixel, m_Elements[m_SelectedIndex]->GetPos() - m_ElementsOffset, 0, m_ElementSize, Vec2(0.5f,0.5f), 0x900000ff, true);

    int elementPerLine = floor(m_Size.x / m_ElementSize.x);
    int lineCount = floor(m_Elements.size() / elementPerLine);
    int totalHeight = lineCount * m_ElementSize.y;
    int scrollbarHeight = m_Size.y / (totalHeight / m_Size.y);

    IMAGE->RenderSprite(pixel, m_Owner->GetPos() + Vec3(m_Size.x * 0.5f, -m_Size.y * 0.5f - m_ScrollPos / totalHeight * m_Size.y, -0.1f),
        0, Vec2(m_ScrollbarWidth, scrollbarHeight), Vec2(1, 0), 0x90000000, true);

    IMAGE->End();
    IMAGE->Begin();
    CAMERA->Render();
    g_Device->SetRenderState(D3DRS_STENCILENABLE, false);
}

void cScrollView::Release()
{
    cGUI::Release();
}

void cScrollView::OnCollision(cObject* _other)
{
    cGUI::OnCollision(_other);
}

void cScrollView::OnAlarm(std::string _key)
{
    cGUI::OnAlarm(_key);
}

void cScrollView::OnClick()
{
    cGUI::OnClick();
    if (m_OnClick == nullptr)
        return;

    Vec2 localClickedPos = INPUT->GetMousePos() - Vec2(m_Owner->GetPos().x - m_Size.x * 0.5f, (m_Owner->GetPos().y - m_Size.y * 0.5f) + m_ScrollPos);
    int index = floor(localClickedPos.y / m_ElementSize.y) * floor(m_Size.x / m_ElementSize.x) + floor(localClickedPos.x / m_ElementSize.x);
    if (m_Elements.size() <= index)
    {
        m_SelectedIndex = -1;
        return;
    }

    m_SelectedIndex = index;
    m_OnClick(m_Elements[index]);
}

void cScrollView::OnClickOutSide()
{
    cGUI::OnClickOutSide();
    // m_SelectedIndex = -1;
}

void cScrollView::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!GetOwner()->IsActive())
        return;
    
    if (uMsg == WM_MOUSEWHEEL && IsMouseOver())
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam) / (120.f / m_ElementSize.y);
        SetScrollPos(m_ScrollPos + delta);
    }

    if (uMsg == WM_KEYDOWN && m_SelectedIndex >= 0)
    {
        int elementPerLine = floor(m_Size.x / m_ElementSize.x);
        if (wParam == VK_UP && m_SelectedIndex - elementPerLine >= 0)
        {
            m_SelectedIndex -= elementPerLine;
            if (floor(m_SelectedIndex / elementPerLine) * m_ElementSize.y < -m_ScrollPos)
                SetScrollPos(m_ScrollPos + m_ElementSize.y);
            if (m_OnClick != nullptr)
                m_OnClick(m_Elements[m_SelectedIndex]);
        }
        if (wParam == VK_DOWN && m_SelectedIndex + elementPerLine < m_Elements.size())
        {
            m_SelectedIndex = min(m_SelectedIndex + elementPerLine, m_Elements.size() - 1);
            if (floor(m_SelectedIndex / elementPerLine) * m_ElementSize.y >= m_Size.y - m_ScrollPos)
                SetScrollPos(m_ScrollPos - m_ElementSize.y);
            if (m_OnClick != nullptr)
                m_OnClick(m_Elements[m_SelectedIndex]);
        }
        if (wParam == VK_LEFT)
        {
            m_SelectedIndex = max(0, m_SelectedIndex - 1);
            if (floor(m_SelectedIndex / elementPerLine) * m_ElementSize.y < -m_ScrollPos)
                SetScrollPos(m_ScrollPos + m_ElementSize.y);
            if (m_OnClick != nullptr)
                m_OnClick(m_Elements[m_SelectedIndex]);
        }
        if (wParam == VK_RIGHT)
        {
            m_SelectedIndex = min(m_Elements.size() - 1, m_SelectedIndex + 1);
            if (floor(m_SelectedIndex / elementPerLine) * m_ElementSize.y >= m_Size.y - m_ScrollPos)
                SetScrollPos(m_ScrollPos - m_ElementSize.y);
            if (m_OnClick != nullptr)
                m_OnClick(m_Elements[m_SelectedIndex]);
        }
    }
}

void cScrollView::ReloadElementsPosition()
{
    int elementPerLine = floor(m_Size.x / m_ElementSize.x);
    Vec3 basePos = m_Owner->GetPos() - (Vec3(m_Size.x, m_Size.y, 0.1f) * 0.5f) + (Vec3(m_ElementSize.x, m_ElementSize.y, 0) * 0.5f) + m_ElementsOffset;
    basePos.y += m_ScrollPos;
    Vec3 pos = Vec3(0,0,0);
    for (int i = 0; i < m_Elements.size(); i++)
    {
        m_Elements[i]->SetPos(basePos + pos);
        pos.x += m_ElementSize.x;
        if ((i + 1) % elementPerLine == 0)
        {
            pos.x = 0;
            pos.y += m_ElementSize.y;
        }
    }
}

void cScrollView::AddElement(cGUI* _element)
{
    _element->GetOwner()->SetActive(false);
    _element->SetSize(m_ElementSize);
    _element->SetTextPivot(Vec2(0, 0.5f));
    m_Elements.push_back(_element->GetOwner());
    ReloadElementsPosition();
}

void cScrollView::RemoveElement(cObject* _element)
{
    for (auto iter = m_Elements.begin(); iter != m_Elements.end(); iter++)
    {
        if ((*iter) == _element)
        {
            (*iter)->Destroy();
            m_Elements.erase(iter);
            return;
        }
    }
    ReloadElementsPosition();
}

void cScrollView::RemoveAllElement()
{
    for(auto& iter : m_Elements)
        iter->Destroy();
    m_Elements.clear();
}

void cScrollView::SetScrollPos(float _pos)
{
    int lineCount = ceil(m_Elements.size() / floor(m_Size.x / m_ElementSize.x));
    m_ScrollPos = Clamp(_pos, -(lineCount * m_ElementSize.y - min(m_Size.y, lineCount * m_ElementSize.y)), 0);
    ReloadElementsPosition();
}

cObject* cScrollView::GetSelectedObject()
{
    if (m_SelectedIndex < 0)
        return nullptr;

    return m_Elements[m_SelectedIndex];
}
