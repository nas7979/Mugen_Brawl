#include "DXUT.h"
#include "cEditorTestPanel.h"

cEditorTestPanel::~cEditorTestPanel()
{
}

void cEditorTestPanel::Init()
{
    cEditorPanel::Init();
}

void cEditorTestPanel::Update()
{
    cEditorPanel::Update();
}

void cEditorTestPanel::Render()
{
    cEditorPanel::Render();
}

void cEditorTestPanel::Release()
{
    cEditorPanel::Release();
}

void cEditorTestPanel::OnCollision(cObject* _other)
{
    cEditorPanel::OnCollision(_other);
}

void cEditorTestPanel::OnAlarm(std::string _key)
{
    cEditorPanel::OnAlarm(_key);
}

void cEditorTestPanel::Save(cCharacterData* _data)
{
}

void cEditorTestPanel::Load(cCharacterData* _data)
{
}

void cEditorTestPanel::Reset()
{
    cEditorPanel::Reset();
}

void cEditorTestPanel::OnShow()
{
    if (m_Data == nullptr)
        return;
    
    m_Character = OBJECT->AddObject<cCharacter>(m_Data->GetName(), Vec3(0,0, 0.5f), Obj_Character);
    m_Character->SetData(m_Data);
    m_Character->SetPalette(0);
}

void cEditorTestPanel::OnHide()
{
    if (m_Character != nullptr)
        m_Character->GetOwner()->Destroy();
    if (m_Dummy != nullptr)
        m_Dummy->GetOwner()->Destroy();
}
