#include "DXUT.h"
#include "cButton.h"

cButton::~cButton()
{
}

void cButton::Init()
{
    cGUI::Init();
}

void cButton::Update()
{
    cGUI::Update();
}

void cButton::Render()
{
    cGUI::Render();
}

void cButton::Release()
{
    cGUI::Release();
}

void cButton::OnCollision(cObject* _other)
{
    cGUI::OnCollision(_other);
}

void cButton::OnAlarm(std::string _key)
{
    cGUI::OnAlarm(_key);
}

void cButton::OnClick()
{
    cGUI::OnClick();
    if(m_OnClick != nullptr)
        m_OnClick();
}
