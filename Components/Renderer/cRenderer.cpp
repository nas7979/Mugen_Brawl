#include "DXUT.h"
#include "cRenderer.h"

cRenderer::~cRenderer()
{
}

void cRenderer::Init()
{
}

void cRenderer::Update()
{
}

void cRenderer::Render()
{
    if (m_Texture == nullptr)
        return;
    Vec3 pos = m_Owner->GetPos();
    pos.x += m_Offset.x;
    pos.y += m_Offset.y;
    IMAGE->RenderSprite(m_Texture, pos, m_Owner->GetRot(), m_Owner->GetScale(), m_Pivot, m_Color);
}

void cRenderer::Release()
{
}

void cRenderer::OnCollision(cObject* _other)
{
}

void cRenderer::OnAlarm(std::string _key)
{
}
