#pragma once
#include "cEditorPanel.h"
#include "../../Characters/Data/cSoundSet.h"

class cEditorSoundPanel : public cEditorPanel
{
public:
    cEditorSoundPanel(cObject* _owner) : cEditorPanel(_owner) {}
    ~cEditorSoundPanel() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

private:
    cScrollView* m_SoundSetsScrollView;
    cInputField* m_SoundSetsInputField;
    cScrollView* m_SoundsScrollView;
    cInputField* m_VolumeInputField;
    cInputField* m_PlayChanceInputField;
    cInputField* m_IsLoopInputField;

    std::map<SerializedString, cSoundSet*>* m_SoundSets;
    cSoundSet* m_CurSoundSet = nullptr;
    int m_SelectedSoundIndex;

    void OnClickAddSoundSet();
    void OnClickRemoveSoundSet();
    void OnClickOpenSound();
    void ReloadSoundSetsList();
    void ReloadSoundsList();

    void SetSoundSetByKey(std::string _key);
    void SetCurrentSoundSet(cSoundSet* _soundSet);
    
public:
    void Save(cCharacterData* _data) override;
    void Load(cCharacterData* _data) override;
    void Reset() override;
    void OnShow() override;
    void OnHide() override;
};
