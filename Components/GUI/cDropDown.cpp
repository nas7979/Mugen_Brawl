#include "DXUT.h"
#include "cDropDown.h"

cDropDown::~cDropDown()
{
}

void cDropDown::Init()
{
    cGUI::Init();
}

void cDropDown::Update()
{
    cGUI::Update();
}

void cDropDown::Render()
{
    cGUI::Render();
}

void cDropDown::Release()
{
    cGUI::Release();
}

void cDropDown::OnCollision(cObject* _other)
{
    cGUI::OnCollision(_other);
}

void cDropDown::OnAlarm(std::string _key)
{
    cGUI::OnAlarm(_key);
}

void cDropDown::OnClick()
{
    cGUI::OnClick();
    if (m_isOpened)
        return;
    m_isOpened = true;
    for (auto& iter : m_Buttons)
        iter->GetOwner()->SetActive(true);
}

void cDropDown::OnClickOutSide()
{
    cGUI::OnClickOutSide();
    if (!m_isOpened)
        return;
    m_isOpened = false;
    for (auto& iter : m_Buttons)
        iter->GetOwner()->SetActive(false);
}

cDropDown* cDropDown::AddButton(cButton* _button)
{
    _button->SetSize(Vec2(m_DropDownWidth, 40));
    _button->GetOwner()->SetActive(false);
    
    float yPos = m_Owner->GetPos().y + m_Size.y * 0.5f + _button->GetSize().y * 0.5f;
    for (auto& iter : m_Buttons)
        yPos += iter->GetSize().y;

    _button->GetOwner()->SetPos(Vec3(m_Owner->GetPos().x - m_Size.x * 0.5f + m_DropDownWidth * 0.5f, yPos, 0.2));

    m_Buttons.push_back(_button);
    return this;
}
