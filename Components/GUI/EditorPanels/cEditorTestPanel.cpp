#include "DXUT.h"
#include "cEditorTestPanel.h"

#include "../../Maps/cBlock.h"

cEditorTestPanel::~cEditorTestPanel()
{
}

void cEditorTestPanel::Init()
{
    cEditorPanel::Init();

    cBlock* ground = OBJECT->AddObject<cBlock>("Block", Vec3(0,400, 0.9f), Obj_Map);
    ground->GetOwner()->SetScale(Vec2(10000, 32));
    m_Objects.push_back(ground->GetOwner());

    ground = OBJECT->AddObject<cBlock>("Block", Vec3(960,0, 0.9f), Obj_Map);
    ground->GetOwner()->SetScale(Vec2(32, 10000));
    m_Objects.push_back(ground->GetOwner());

    ground = OBJECT->AddObject<cBlock>("Block", Vec3(-960,0, 0.9f), Obj_Map);
    ground->GetOwner()->SetScale(Vec2(32, 10000));
    m_Objects.push_back(ground->GetOwner());

    ground = OBJECT->AddObject<cBlock>("Block", Vec3(0,100, 0.9f), Obj_Map);
    ground->GetOwner()->SetScale(Vec2(300, 32));
    m_Objects.push_back(ground->GetOwner());
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
    m_Character->SetDirection(1);
    m_Character->SetPlayerIndex(0);
    m_Character->SetTeam(0);

    m_Dummy = OBJECT->AddObject<cCharacter>(m_Data->GetName(), Vec3(100,0, 0.5f), Obj_Character);
    m_Dummy->SetData(m_Data);
    m_Dummy->SetPalette(1);
    m_Dummy->SetDirection(1);
    m_Dummy->SetPlayerIndex(1);
    m_Dummy->SetTeam(1);
}

void cEditorTestPanel::OnHide()
{
    if (m_Character != nullptr)
        m_Character->GetOwner()->Destroy();
    if (m_Dummy != nullptr)
        m_Dummy->GetOwner()->Destroy();
}
