#include "DXUT.h"
#include "cCameraManager.h"

void cCameraManager::Init()
{
    D3DXMatrixOrthoLH(&m_ProjMat, 1920, 1080, 0.01f, 10000);
    g_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMat);
}

void cCameraManager::Update()
{
    if(m_ShakeForce > 0)
    {
        m_ShakeForce -= m_ShakeForceDecPerFrame;
        if (m_ShakeForce < 0)
            m_ShakeForce = 0;
    }

    Vec2 screenScale = IMAGE->GetScreenScale();
    Matrix S, R, P;
    D3DXMatrixScaling(&S, m_Scale, m_Scale, 1);
    D3DXMatrixRotationZ(&R, -m_Rot);
    Vec3 pos = m_Pos - Vec3(Random(-m_ShakeForce, m_ShakeForce), Random(-m_ShakeForce, m_ShakeForce), 0);
    D3DXMatrixTranslation(&P, -pos.x + SCREEN_WIDTH * screenScale.x * 0.5f, -pos.y + SCREEN_HEIGHT * screenScale.y * 0.5f, 0);

    m_ViewMat = P * R * S;
    D3DXMatrixInverse(&m_InversedViewMat, 0, &m_ViewMat);
}

void cCameraManager::Render()
{
    g_Device->SetTransform(D3DTS_VIEW, &m_ViewMat);
}

void cCameraManager::Release()
{
}

void cCameraManager::Shake(float _force, int _time)
{
    m_ShakeForce = _force;
    m_ShakeForceDecPerFrame = _force / _time;
}
