#include "DXUT.h"
#include "cBlock.h"

cBlock::~cBlock()
{
}

void cBlock::Init()
{
}

void cBlock::Update()
{
    m_Rect = RECT {
        (LONG)(m_Owner->GetPos().x - m_Owner->GetScale().x * 0.5f),
        (LONG)(m_Owner->GetPos().y - m_Owner->GetScale().y * 0.5f),
        (LONG)(m_Owner->GetPos().x + m_Owner->GetScale().x * 0.5f),
        (LONG)(m_Owner->GetPos().y + m_Owner->GetScale().y * 0.5f),
    };
}

void cBlock::Render()
{
    IMAGE->RenderSprite(IMAGE->GetTexture("Pixel"), m_Owner->GetPos(), 0, m_Owner->GetScale());
}

void cBlock::Release()
{
}

void cBlock::OnCollision(cObject* _other)
{
}

void cBlock::OnAlarm(std::string _key)
{
}
