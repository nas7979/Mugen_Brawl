#pragma once
#include "../../Characters/cCharacter.h"

class cEditorTestPanel : public cEditorPanel
{
public:
    cEditorTestPanel(cObject* _owner) : cEditorPanel(_owner) {}
    ~cEditorTestPanel() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void Save(cCharacterData* _data) override;
    void Load(cCharacterData* _data) override;
    void Reset() override;
    void OnShow() override;
    void OnHide() override;

private:
    cCharacter* m_Character = nullptr;
    cCharacter* m_Dummy = nullptr;
};
