#include "DXUT.h"
#include "cEditorSoundPanel.h"

#include "../cScrollView.h"
#include "../../../Scenes/Editor/cEditorScene.h"
#include "../../Characters/Data/cSound.h"
#include "../../Characters/Data/cSoundSet.h"

class cSound;

cEditorSoundPanel::~cEditorSoundPanel()
{
}

void cEditorSoundPanel::Init()
{
    cEditorPanel::Init();

    CreateButton(Vec2(85, 120), Vec2(150, 40), "사운드 불러오기", [&]()->void{OnClickOpenSound();});
    
    CreateButton(Vec2(460, 180), Vec2(100, 40), "추가", [&]()->void{OnClickAddSoundSet();});
    CreateButton(Vec2(460, 220), Vec2(100, 40), "삭제", [&]()->void{OnClickRemoveSoundSet();});
    m_SoundSetsInputField = CreateInputField(Vec2(210, 180), Vec2(400, 40), "Animation Name", 24);
    m_SoundSetsInputField->SetOnTextChanged([&](cInputField* _this)->void
    {
        ReloadSoundSetsList();
        m_SoundSetsScrollView->SetSelectedIndex(0);
        m_SelectedSoundIndex = m_SoundSetsScrollView->GetSelectedIndex();
    });
    
    m_SoundSetsScrollView = CreateScrollView(Vec2(210, 450), Vec2(400, 500), Vec2(400, 25), [&](cObject* _selected)->void
    {
        m_CurSoundSet = (*m_SoundSets)[_selected->GetComponent<cGUI>()->GetText()];
        m_SoundsScrollView->SetSelectedIndex(0);
        m_SelectedSoundIndex = m_SoundSetsScrollView->GetSelectedIndex();
        ReloadSoundsList();
    }, true);
    m_SoundSetsScrollView->SetElementOffset(Vec2(5, 0));

    m_SoundsScrollView = CreateScrollView(Vec2(800, 450), Vec2(400, 500), Vec2(400, 25), [&](cObject* _selected)->void
    {
        m_SelectedSoundIndex = m_SoundsScrollView->GetSelectedIndex();
        cSound* sound = m_CurSoundSet->GetSound(m_SelectedSoundIndex);
        m_VolumeInputField->SetText(std::to_string(sound->GetVolume()));
        m_PlayChanceInputField->SetText(std::to_string(sound->GetPlayChance()));
        m_IsLoopInputField->SetText(std::to_string(sound->IsLoop()));
    }, true);
    m_SoundsScrollView->SetElementOffset(Vec2(5, 0));

    m_VolumeInputField = CreateInputField(Vec2(1920 - 85, 120), Vec2(100, 40), "Volume", 24);
    m_VolumeInputField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (m_CurSoundSet == nullptr)
            return;
        m_CurSoundSet->GetSound(m_SelectedSoundIndex)->SetVolume(_this->GetInt());
    });

    m_PlayChanceInputField = CreateInputField(Vec2(1920 - 190, 120), Vec2(100, 40), "Play Chance", 24);
    m_PlayChanceInputField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (m_CurSoundSet == nullptr)
            return;
        m_CurSoundSet->GetSound(m_SelectedSoundIndex)->SetPlayChance(_this->GetInt());
    });

    m_IsLoopInputField = CreateInputField(Vec2(1920 - 85, 200), Vec2(100, 40), "IsLoop", 24);
    m_IsLoopInputField->SetOnTextChanged([&](cInputField* _this)->void
    {
        if (m_CurSoundSet == nullptr)
            return;
        m_CurSoundSet->GetSound(m_SelectedSoundIndex)->SetIsLoop(_this->GetInt());
    });

}

void cEditorSoundPanel::Update()
{
    cEditorPanel::Update();

    if (m_CurSoundSet == nullptr || m_SelectedSoundIndex < 0)
        return;
    
    if (INPUT->KeyDown(VK_SPACE))
    {
        SOUND->StopAll();
        SOUND->Play(m_CurSoundSet->GetSound(m_SelectedSoundIndex)->GetSound(), m_CurSoundSet->GetSound(m_SelectedSoundIndex)->GetVolume());
    }

    if (INPUT->KeyDown(VK_DELETE))
    {
        std::string curSoundSetKey;
        for (auto& iter : *m_SoundSets)
        {
            if (iter.second == m_CurSoundSet)
            {
                curSoundSetKey = iter.first;
                break;
            }
        }

        std::string serializedSound;
        int index = m_SelectedSoundIndex;
        serializedSound.resize(m_CurSoundSet->GetSound(index)->GetSize());
        UINT pointer = 0;
        m_CurSoundSet->GetSound(index)->Serialize(const_cast<char*>(serializedSound.c_str()), pointer);

        int soundCount = m_CurSoundSet->GetSoundsCount();
        INPUT->Do([this, curSoundSetKey, index]()->void
        {
            SetSoundSetByKey(curSoundSetKey);
            m_CurSoundSet->RemoveSound(index, true);
            ReloadSoundsList();
        }, [this, serializedSound, curSoundSetKey, soundCount]()->void
        {
            SetSoundSetByKey(curSoundSetKey);
            cSound* sound = new cSound();
            UINT Pointer = 0;
            sound->Deserialize(const_cast<char*>(serializedSound.c_str()), Pointer);
            m_CurSoundSet->AddSound(sound);
            ReloadSoundsList();
        });
    }
}

void cEditorSoundPanel::Render()
{
    cEditorPanel::Render();
}

void cEditorSoundPanel::Release()
{
    cEditorPanel::Release();
}

void cEditorSoundPanel::OnCollision(cObject* _other)
{
    cEditorPanel::OnCollision(_other);
}

void cEditorSoundPanel::OnAlarm(std::string _key)
{
    cEditorPanel::OnAlarm(_key);
}

void cEditorSoundPanel::OnClickAddSoundSet()
{
    std::string key = m_SoundSetsInputField->GetText();
    if (key.empty())
        return;

    if (m_SoundSets->find(key) != m_SoundSets->end())
        return;

    INPUT->Do([this, key]()->void
    {
        SetCurrentSoundSet(m_Data->AddSoundSet(key));
        
        m_SoundSetsInputField->SetText("");
        ReloadSoundSetsList();
    },[this, key]()->void
    {
        m_Data->RemoveSoundSet(key);

        SetCurrentSoundSet(nullptr);
        ReloadSoundSetsList();
    });
}

void cEditorSoundPanel::OnClickRemoveSoundSet()
{
    if (m_CurSoundSet == nullptr)
        return;

    std::string key;
    for (auto& iter : *m_SoundSets)
    {
        if (iter.second == m_CurSoundSet)
        {
            key = iter.first;
            break;
        }
    }
    
    std::string serializedSoundSet;
    serializedSoundSet.resize(m_CurSoundSet->GetSize());
    UINT pointer = 0;
    m_CurSoundSet->Serialize(const_cast<char*>(serializedSoundSet.c_str()), pointer);
    
    INPUT->Do([this, key]()->void
    {
        m_Data->RemoveSoundSet(key);
        SetCurrentSoundSet(nullptr);
        ReloadSoundSetsList();
    }, [this, serializedSoundSet, key]()->void
    {
        UINT pointer = 0;
        cSoundSet* soundSet = m_Data->AddSoundSet(key);
        soundSet->Deserialize(const_cast<char*>(serializedSoundSet.c_str()), pointer);
        SetCurrentSoundSet(soundSet);
        ReloadSoundSetsList();
    });
}

void cEditorSoundPanel::OnClickOpenSound()
{
    if (m_CurSoundSet == nullptr)
        return;
   
    std::vector<std::wstring> filePaths;
    if (!cEditorScene::OpenExplorer(&filePaths, "*.wav", FOS_ALLOWMULTISELECT))
        return;

    int filePathsCount = filePaths.size();
    std::string curSoundSetKey;
    for (auto& iter : *m_SoundSets)
    {
        if (iter.second == m_CurSoundSet)
        {
            curSoundSetKey = iter.first;
            break;
        }
    }

    int soundCount = m_CurSoundSet->GetSoundsCount();
    INPUT->Do([this, filePaths, curSoundSetKey]()->void
    {
        SetSoundSetByKey(curSoundSetKey);
        
        for(int i = 0; i < filePaths.size(); i++)
        {
            cSound* sound = new cSound();
            sound->SetSound(filePaths[i]);
            m_CurSoundSet->AddSound(sound);
        }
        ReloadSoundsList();
    }, [this, filePathsCount, curSoundSetKey, soundCount]()->void
    {
        SetSoundSetByKey(curSoundSetKey);
        for(int i = 0; i < filePathsCount; i++)
            m_CurSoundSet->RemoveSound(soundCount, true);
        ReloadSoundsList();
    });
}

void cEditorSoundPanel::ReloadSoundSetsList()
{
    m_SoundSetsScrollView->RemoveAllElement();
    m_SoundSetsScrollView->SetScrollPos(0);
    ReloadSoundsList();
    
    std::string find = m_SoundSetsInputField->GetText();
    if (find.empty())
    {
        for (auto& iter : *m_SoundSets)
        {
            cGUI* text = OBJECT->AddObject<cGUI>("Text", Vec3(), Obj_GUI)
                ->SetDefaults(Vec2(0,0), iter.first, 0, 0);
            m_SoundSetsScrollView->AddElement(text);
        }
    }
    else
    {
        for (auto& iter : *m_SoundSets)
        {
            if (((std::string)iter.first).find(find) != std::string::npos)
            {
                cGUI* text = OBJECT->AddObject<cGUI>("Text", Vec3(), Obj_GUI)
                    ->SetDefaults(Vec2(0,0), iter.first, 0, 0);
                m_SoundSetsScrollView->AddElement(text);
            }
        }
    }
}

void cEditorSoundPanel::ReloadSoundsList()
{
    m_SoundsScrollView->RemoveAllElement();
    if (m_CurSoundSet == nullptr)
        return;

    for (int i = 0; i < m_CurSoundSet->GetSoundsCount(); i++)
    {
        cGUI* text = OBJECT->AddObject<cGUI>("Text", Vec3(), Obj_GUI)
            ->SetDefaults(Vec2(0,0), std::to_string(i), 0, 0);
        m_SoundsScrollView->AddElement(text);
    }
}

void cEditorSoundPanel::SetSoundSetByKey(std::string _key)
{
    cSoundSet* soundSet = m_Data->GetSoundSet(_key);
    if (soundSet == nullptr)
        return;
    SetCurrentSoundSet(soundSet);
}

void cEditorSoundPanel::SetCurrentSoundSet(cSoundSet* _soundSet)
{
    m_CurSoundSet = _soundSet;
}

void cEditorSoundPanel::Save(cCharacterData* _data)
{
}

void cEditorSoundPanel::Load(cCharacterData* _data)
{
    Reset();
}

void cEditorSoundPanel::Reset()
{
    cEditorPanel::Reset();
    m_SelectedSoundIndex = -1;
    m_SoundSets = m_Data->GetSoundSets();
    SetCurrentSoundSet(nullptr);

    m_SoundSetsInputField->SetText("");
    m_VolumeInputField->SetText("255");
    m_PlayChanceInputField->SetText("0");
    m_IsLoopInputField->SetText("0");

    ReloadSoundSetsList();
}

void cEditorSoundPanel::OnShow()
{
}

void cEditorSoundPanel::OnHide()
{
}
