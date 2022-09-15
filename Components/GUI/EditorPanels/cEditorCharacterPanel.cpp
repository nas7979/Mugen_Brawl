#include "DXUT.h"
#include "cEditorCharacterPanel.h"

#include "../../../Scenes/Editor/cEditorScene.h"

cEditorCharacterPanel::~cEditorCharacterPanel()
{
}

void cEditorCharacterPanel::Init()
{
    cEditorPanel::Init();

    m_NameInputField = CreateInputField(Vec2(110, 120), Vec2(200, 40), "Name", 24);
    
    m_WeightInputField = CreateInputField(Vec2(1920 - 85, 120), Vec2(100, 40), "Weight", 24);
    m_JumpHeightInputField = CreateInputField(Vec2(1920 - 190, 120), Vec2(100, 40), "Jump Height", 24);
    m_AirMovementLimitInputField = CreateInputField(Vec2(1920 - 295, 120), Vec2(100, 40), "Air Movement Limit", 24);
    
    m_WalkSpeedInputField = CreateInputField(Vec2(1920 - 85, 200), Vec2(100, 40), "Walk Speed", 24);
    m_DashSpeedInputField = CreateInputField(Vec2(1920 - 190, 200), Vec2(100, 40), "Dash Speed", 24);
    m_AirSpeedInputField = CreateInputField(Vec2(1920 - 295, 200), Vec2(100, 40), "Air Speed", 24);
    
    m_FallSpeedInputField = CreateInputField(Vec2(1920 - 85, 280), Vec2(100, 40), "Fall Speed", 24);
    m_FastFallSpeedInputField = CreateInputField(Vec2(1920 - 190, 280), Vec2(100, 40), "Fast Fall Speed", 24);
    
    m_ShieldSizeInputField = CreateInputField(Vec2(1920 - 85, 360), Vec2(100, 40), "Shield Size", 24);
    m_ShieldDecSpeedInputField = CreateInputField(Vec2(1920 - 190, 360), Vec2(100, 40), "Shield Dec Speed", 24);
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
    _data->SetWeight(m_WeightInputField->GetInt());
    _data->SetJumpHeight(m_JumpHeightInputField->GetInt());
    _data->SetAirMovementLimit(m_AirMovementLimitInputField->GetInt());
    _data->SetWalkSpeed(m_WalkSpeedInputField->GetInt());
    _data->SetDashSpeed(m_DashSpeedInputField->GetInt());
    _data->SetAirSpeed(m_AirSpeedInputField->GetInt());
    _data->SetFallSpeed(m_FallSpeedInputField->GetInt());
    _data->SetFastFallSpeed(m_FastFallSpeedInputField->GetInt());
    _data->SetShieldSize(m_ShieldSizeInputField->GetInt());
    _data->SetShieldDecSpeed(m_ShieldDecSpeedInputField->GetInt());
}

void cEditorCharacterPanel::Load(cCharacterData* _data)
{
    m_NameInputField->SetText(_data->GetName());
    m_WeightInputField->SetText(std::to_string(_data->GetWeight()));
    m_JumpHeightInputField->SetText(std::to_string(_data->GetJumpHeight()));
    m_AirMovementLimitInputField->SetText(std::to_string(_data->GetAirMovementLimit()));
    m_WalkSpeedInputField->SetText(std::to_string(_data->GetWalkSpeed()));
    m_DashSpeedInputField->SetText(std::to_string(_data->GetDashSpeed()));
    m_AirSpeedInputField->SetText(std::to_string(_data->GetAirSpeed()));
    m_FallSpeedInputField->SetText(std::to_string(_data->GetFallSpeed()));
    m_FastFallSpeedInputField->SetText(std::to_string(_data->GetFastFallSpeed()));
    m_ShieldSizeInputField->SetText(std::to_string(_data->GetShieldSize()));
    m_ShieldDecSpeedInputField->SetText(std::to_string(_data->GetShieldDecSpeed()));
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
