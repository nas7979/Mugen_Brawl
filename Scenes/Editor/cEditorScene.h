#pragma once
#include "../../Components/Characters/Data/cCharacterData.h"
#include "../../Components/Characters/Data/cCharacterSprite.h"
#include "../../Components/GUI/cInputField.h"
#include "../../Components/GUI/cScrollView.h"
#include "../../Components/GUI/EditorPanels/cEditorCharacterPanel.h"
#include "../../Components/GUI/EditorPanels/cEditorPanel.h"
#include "../../Components/GUI/EditorPanels/cEditorSoundPanel.h"

class cEditorSpritePanel;
class cDropDown;
class cButton;

class cEditorScene : public cScene
{
enum Panel
{
    Panel_Character,
    Panel_Sprite,
    Panel_Sound,
    Panel_End
};
    
public:
    ~cEditorScene() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;

private:
    cCharacterData* m_CharacterData = nullptr;
    cEditorPanel* m_Panels[Panel_End];
    Panel m_CurPanel = Panel_End;
    std::wstring m_CurrentFilePath;
    PALETTEENTRY* m_paletteBuffer;

    void InitGUIs();
    void ReloadGUIs();
    void ReleaseGUIs();

    void OnClickNewCharacter();
    void OnClickOpenCharacter();
    void OnClickSave();
    void OnClickSaveAs();
    void OnClickLoadImage();

    void ChangePanel(Panel _panel);
    void SaveCharacterData();

public:
    static bool OpenExplorer(std::vector<std::wstring>* _filePaths, LPCSTR _fileFilter, DWORD _flag);
    static bool OpenFile(std::wstring _path, char** _buffer, UINT& _length);
    static bool SaveFile(std::wstring _path, Serializer* _object);
    
    static cButton* CreateButton(Vec2 _pos, Vec2 _size, std::string _text, std::function<void()> _onClick);
    static cDropDown* CreateDropDown(Vec2 _pos, Vec2 _size, float _dropDownWidth, std::string _text);
    static cInputField* CreateInputField(Vec2 _pos, Vec2 _size, std::string _desc, int _fontSize, D3DCOLOR _textColor = 0xff000000, std::string _font = "Gothic");
    static cScrollView* CreateScrollView(Vec2 _pos, Vec2 _size, Vec2 _elementSize, std::function<void(cObject*)> _onClick, bool _isActive);

    cEditorCharacterPanel* GetCharacterPanel() const {return (cEditorCharacterPanel*)m_Panels[Panel_Character];}
    cEditorSpritePanel* GetSpritePanel() const {return (cEditorSpritePanel*)m_Panels[Panel_Sprite];}
    cEditorSoundPanel* GetSoundPanel() const {return (cEditorSoundPanel*)m_Panels[Panel_Sound];}
    
    cCharacterData* GetCharacterData() const {return m_CharacterData;}
    int GetPaletteBuffer(PALETTEENTRY*& _entries) const {_entries = m_paletteBuffer; return m_CharacterData->GetPaletteLength();}
};
