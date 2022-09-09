#include "DXUT.h"
#include "cEditorCharacterPanel.h"

#include "../../../Scenes/Editor/cEditorScene.h"

cEditorCharacterPanel::~cEditorCharacterPanel()
{
}

void cEditorCharacterPanel::Init()
{
    cEditorPanel::Init();

    m_NameInputField = CreateInputField(Vec2(110, 120), Vec2(200, 40), "이름", 24);
}

void cEditorCharacterPanel::Update()
{
    cEditorPanel::Update();
}

void cEditorCharacterPanel::Render()
{
    cEditorPanel::Render();
}

void cEditorCharacterPanel::Release()
{
    cEditorPanel::Release();
}

void cEditorCharacterPanel::OnCollision(cObject* _other)
{
    cEditorPanel::OnCollision(_other);
}

void cEditorCharacterPanel::OnAlarm(std::string _key)
{
    cEditorPanel::OnAlarm(_key);
}

void cEditorCharacterPanel::Save(cCharacterData* _data)
{
    _data->SetName(m_NameInputField->GetText());
}

void cEditorCharacterPanel::Load(cCharacterData* _data)
{
    m_NameInputField->SetText(_data->GetName());
}

void cEditorCharacterPanel::Reset()
{
    cEditorPanel::Reset();
}

void cEditorCharacterPanel::OnShow()
{
}

void cEditorCharacterPanel::OnHide()
{
}
