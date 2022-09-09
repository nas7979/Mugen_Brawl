#pragma once
#include "cEditorPanel.h"

class cEditorCharacterPanel : public cEditorPanel
{
public:
    cEditorCharacterPanel(cObject* _owner) : cEditorPanel(_owner) {}
    ~cEditorCharacterPanel() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

private:
    cInputField* m_NameInputField;

public:
    void Save(cCharacterData* _data) override;
    void Load(cCharacterData* _data) override;
    void Reset() override;
    void OnShow() override;
    void OnHide() override;
};
