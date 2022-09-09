#include "DXUT.h"
#include "cGUI.h"

cGUI::~cGUI()
{
}

void cGUI::Init()
{
}

void cGUI::Update()
{
    if (INPUT->KeyDown(VK_LBUTTON))
    {
        IsMouseOver() ? INPUT->AddToClickedGUIs(this) : OnClickOutSide();
    }

    if (m_IsClicked && INPUT->KeyUp(VK_LBUTTON))
    {
        OnClickUp();
    }
    
    m_DoubleClickInterval--;
}

void cGUI::Render()
{
    Texture* pixel = IMAGE->GetTexture("Pixel");
    if (m_BackgroundColor != 0)
    {
        IMAGE->RenderSprite(pixel, m_Owner->GetPos(), 0, m_Size, Vec2(0.5,0.5), m_BackgroundColor, true);
    }
    if (m_OutlineColor != 0)
    {
        IMAGE->RenderSprite(pixel, m_Owner->GetPos() - Vec3(0, m_Size.y * 0.5f - 1, 0), 0, Vec2(m_Size.x, 2), Vec2(0.5,0.5), m_OutlineColor, true);
        IMAGE->RenderSprite(pixel, m_Owner->GetPos() + Vec3(0, m_Size.y * 0.5f - 1, 0), 0, Vec2(m_Size.x, 2), Vec2(0.5,0.5), m_OutlineColor, true);
        IMAGE->RenderSprite(pixel, m_Owner->GetPos() - Vec3(m_Size.x * 0.5f - 1, 0, 0), 0, Vec2(2, m_Size.y), Vec2(0.5,0.5), m_OutlineColor, true);
        IMAGE->RenderSprite(pixel, m_Owner->GetPos() + Vec3(m_Size.x * 0.5f - 1, 0, 0), 0, Vec2(2, m_Size.y), Vec2(0.5,0.5), m_OutlineColor, true);
    }
    if (!m_Text.empty())
    {
        IMAGE->RenderText("Gothic", m_Text, m_Owner->GetPos() + Vec3(m_Size.x * (m_TextPivot.x - 0.5f), m_Size.y * (m_TextPivot.y - 0.5f), 0),
            0, Vec2(1, 1) * (m_FontSize / 64.f), m_TextPivot, 0xff000000, true);
    }
}

void cGUI::Release()
{
}

void cGUI::OnCollision(cObject* _other)
{
}

void cGUI::OnAlarm(std::string _key)
{
}

void cGUI::OnClick()
{
    m_IsClicked = true;
    m_DoubleClickInterval = 20;
}

void cGUI::OnDoubleClick()
{
    m_IsClicked = false;
}

void cGUI::OnClickUp()
{
    m_IsClicked = false;
}

void cGUI::OnClickOutSide()
{
    if (m_InactivateOnClickOutside)
        GetOwner()->SetActive(false);
}

cGUI* cGUI::SetDefaults(Vec2 _size, std::string _text, D3DCOLOR _backgroundColor, D3DCOLOR _outlineColor)
{
    m_Size = _size;
    m_Text = _text;
    m_BackgroundColor = _backgroundColor;
    m_OutlineColor = _outlineColor;
    return this;
}

bool cGUI::IsMouseOver()
{
    Vec2 mousePos = INPUT->GetMousePos();
    Vec2 min = (Vec2)m_Owner->GetPos() - m_Size * 0.5f;
    Vec2 max = (Vec2)m_Owner->GetPos() + m_Size * 0.5f;
    return mousePos.x >= min.x && mousePos.y >= min.y && mousePos.x <= max.x && mousePos.y <= max.y;
}
