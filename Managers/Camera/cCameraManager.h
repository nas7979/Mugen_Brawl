#pragma once

class cCameraManager : public cSingleton<cCameraManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    Vec3 m_Pos = Vec3(0,0,0);
    float m_Rot = 0;
    float m_Scale = 1;
    float m_ShakeForce = 0;
    float m_ShakeForceDecPerFrame = 0;
    Matrix m_ViewMat;
    Matrix m_ProjMat;
    Matrix m_InversedViewMat;

public:
    Vec3 GetPos() {return m_Pos;}
    void SetPos(Vec3 _pos) {m_Pos = _pos;}
    float GetRot() {return m_Rot;}
    void SetRot(float _rot) {m_Rot = _rot;}
    float GetScale() {return m_Scale;}
    void SetScale(float _scale) {m_Scale = _scale;}
    const Matrix& GetInversedMatrix() {return m_InversedViewMat;}

    void Shake(float _force, int _time);
};

#define CAMERA cCameraManager::GetInstance()