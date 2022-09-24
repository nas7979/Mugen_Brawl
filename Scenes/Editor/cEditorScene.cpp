#include "DXUT.h"
#include "cEditorScene.h"

#include "../../Components/GUI/cButton.h"
#include "../../Components/GUI/cDropDown.h"
#include "../../Components/GUI/cScrollView.h"
#include "../../Components/GUI/EditorPanels/cEditorCharacterPanel.h"
#include "../../Components/GUI/EditorPanels/cEditorSpritePanel.h"
#include "../../Components/GUI/EditorPanels/cEditorTestPanel.h"

cEditorScene::~cEditorScene()
{
}

void cEditorScene::Init()
{
    m_CharacterData = new cCharacterData();
    m_paletteBuffer = new PALETTEENTRY[m_CharacterData->GetPaletteLength()];
    InitGUIs();
}

void cEditorScene::Update()
{
    if (INPUT->KeyPress(VK_LCONTROL))
    {
        if (INPUT->KeyDown('S'))
            OnClickSave();
    }
}

void cEditorScene::Render()
{
    IMAGE->RenderSprite(IMAGE->GetTexture("Pixel"), Vec3(0, 0, 0.99f), 0, Vec2(1920, 1080), Vec2(0, 0), 0xff808080, true);
}

void cEditorScene::Release()
{
    SaveCharacterData();
    SAFE_DELETE(m_CharacterData)
    SAFE_DELETE_ARRAY(m_paletteBuffer)
}

void cEditorScene::InitGUIs()
{
    m_Panels[Panel_Character] = OBJECT->AddObject<cEditorCharacterPanel>("CharacterPanel", Vec3(0,0,0), Obj_GUI);
    m_Panels[Panel_Sprite] = OBJECT->AddObject<cEditorSpritePanel>("SpritePanel", Vec3(0,0,0), Obj_GUI);
    m_Panels[Panel_Sound] = OBJECT->AddObject<cEditorSoundPanel>("SoundPanel", Vec3(0,0,0), Obj_GUI);
    m_Panels[Panel_Test] = OBJECT->AddObject<cEditorTestPanel>("TestPanel", Vec3(0,0,0), Obj_GUI);
    
    CreateDropDown(Vec2(40, 15), Vec2(80, 30), 200, "파일")
    ->AddButton(CreateButton(Vec2(0,0),Vec2(0,0),"새 캐릭터", [&]()->void{OnClickNewCharacter();}))
    ->AddButton(CreateButton(Vec2(0,0),Vec2(0,0),"열기", [&]()->void{OnClickOpenCharacter();}))
    ->AddButton(CreateButton(Vec2(0,0),Vec2(0,0),"저장", [&]()->void{OnClickSave();}))
    ->AddButton(CreateButton(Vec2(0,0),Vec2(0,0),"다른 이름으로 저장", [&]()->void{OnClickSaveAs();}));

    CreateButton(Vec2(40, 45), Vec2(80, 30), "캐릭터", [&]()->void{ChangePanel(Panel_Character);});
    CreateButton(Vec2(120, 45), Vec2(80, 30), "스프라이트", [&]()->void{ChangePanel(Panel_Sprite);});
    CreateButton(Vec2(200, 45), Vec2(80, 30), "사운드", [&]()->void{ChangePanel(Panel_Sound);});
    CreateButton(Vec2(280, 45), Vec2(80, 30), "테스트", [&]()->void{ChangePanel(Panel_Test);});

    ChangePanel(Panel_Character);
    ReloadGUIs();
}

void cEditorScene::ReloadGUIs()
{
    for (int i = 0; i < Panel_End; i++)
    {
        m_Panels[i]->Reset();
    }
}

void cEditorScene::ReleaseGUIs()
{
    for (int i = 0; i < Panel_End; i++)
    {
        m_Panels[i]->Release();
    }
}

void cEditorScene::OnClickNewCharacter()
{
    ReleaseGUIs();
    SAFE_DELETE(m_CharacterData)
    
    m_CharacterData = new cCharacterData();
    m_CurrentFilePath = L"";
    ReloadGUIs();
}

void cEditorScene::OnClickOpenCharacter()
{
    std::vector<std::wstring> filePaths;
    if (!OpenExplorer(&filePaths, "*.CHARDAT", 0))
        return;
    
    char* buffer;
    UINT length;
    if (!OpenFile(filePaths[0], &buffer, length))
        return;

    ReleaseGUIs();
    SAFE_DELETE(m_CharacterData)
    m_CharacterData = new cCharacterData();

    UINT pointer = 0;
    m_CharacterData->Deserialize(buffer, pointer);

    for (int i = 0; i < Panel_End; i++)
    {
        m_Panels[i]->Load(m_CharacterData);
    }

    delete[] buffer;
    m_CurrentFilePath = filePaths[0];
    ReloadGUIs();
}

void cEditorScene::OnClickSave()
{
    SaveCharacterData();
    if (!m_CurrentFilePath.empty())
        SaveFile(m_CurrentFilePath, (Serializer*)m_CharacterData);
    else
    {
        std::vector<std::wstring> filePaths;
        if(!OpenExplorer(&filePaths, "", FOS_PICKFOLDERS))
            return;

        SaveFile(filePaths[0] + L"\\" + AToW(m_CharacterData->GetName()) + L".CHARDAT", (Serializer*)m_CharacterData);
    }
}

void cEditorScene::OnClickSaveAs()
{
}

void cEditorScene::OnClickLoadImage()
{
}

void cEditorScene::ChangePanel(Panel _panel)
{
    if (m_CurPanel == _panel)
        return;

    m_CurPanel = _panel;
    for (int i = 0; i < Panel_End; i++)
        m_Panels[i]->Show(i == _panel);
}

void cEditorScene::SaveCharacterData()
{
    for (int i = 0; i < Panel_End; i++)
    {
        m_Panels[i]->Save(m_CharacterData);
    }
}

bool cEditorScene::OpenExplorer(std::vector<std::wstring>* _filePaths, LPCSTR _fileFilter, DWORD _flag)
{
    IFileOpenDialog *pFileOpen;

    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
    pFileOpen->SetOptions(_flag);
    if (SUCCEEDED(hr))
    {
        hr = pFileOpen->Show(NULL);
        if (SUCCEEDED(hr))
        {
            IShellItemArray *pItems;
            hr = pFileOpen->GetResults(&pItems);
            if (SUCCEEDED(hr))
            {
                DWORD count;
                pItems->GetCount(&count);
                _filePaths->resize(count);
                for (int i = 0; i < count; i++)
                {
                    IShellItem* pItem;
                    LPWSTR pPath;
                    pItems->GetItemAt(i, &pItem);
                    pItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);
                    (*_filePaths)[i] = pPath;
                    pItem->Release();
                    CoTaskMemFree(pPath);
                }
                
                pItems->Release();
            }
        }
        pFileOpen->Release();
    }

    return SUCCEEDED(hr);
}

bool cEditorScene::OpenFile(std::wstring _path, char** _buffer, UINT& _length)
{
    std::ifstream ifs(_path, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
        return false;
    
    ifs.seekg(0, ifs.end);
    _length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    *_buffer = new char[_length];
    ifs.read(*_buffer, _length);

    return true;
}

bool cEditorScene::SaveFile(std::wstring _path, Serializer* _object)
{
    std::ofstream ofs(_path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!ofs.is_open())
        return false;

    size_t size = _object->GetSize();
    char* buffer = new char[size];
    UINT pointer = 0;
    _object->Serialize(buffer, pointer);
    ofs.write(buffer, size);

    delete[] buffer;
    return true;
}

cButton* cEditorScene::CreateButton(Vec2 _pos, Vec2 _size, std::string _text, std::function<void()> _onClick)
{
    return (cButton*)OBJECT->AddObject<cButton>("Button", Vec3(_pos.x, _pos.y, 0.5), Obj_GUI)
    ->SetOnClick(_onClick)
    ->SetDefaults(_size, _text);
}

cDropDown* cEditorScene::CreateDropDown(Vec2 _pos, Vec2 _size, float _dropDownWidth, std::string _text)
{
    return (cDropDown*)OBJECT->AddObject<cDropDown>("DropDown", Vec3(_pos.x, _pos.y, 0.5), Obj_GUI)
    ->SetDropDownWidth(_dropDownWidth)
    ->SetDefaults(_size, _text);
}

cInputField* cEditorScene::CreateInputField(Vec2 _pos, Vec2 _size, std::string _desc, int _fontSize, D3DCOLOR _textColor, std::string _font)
{
    return (cInputField*)OBJECT->AddObject<cInputField>("InputField", Vec3(_pos.x, _pos.y, 0.5), Obj_GUI)
    ->SetDesc(_desc)
    ->SetFont(_font)
    ->SetFontSize(_fontSize)
    ->SetTextColor(_textColor)
    ->SetDefaults(_size, "");
}

cScrollView* cEditorScene::CreateScrollView(Vec2 _pos, Vec2 _size, Vec2 _elementSize, std::function<void(cObject*)> _onClick, bool _isActive)
{
    cScrollView* scrollView = (cScrollView*)OBJECT->AddObject<cScrollView>("ScrollView", Vec3(_pos.x, _pos.y, 0.5), Obj_GUI)
    ->SetElementSize(_elementSize)
    ->SetOnClick(_onClick)
    ->SetDefaults(_size, "");
    scrollView->GetOwner()->SetActive(_isActive);
    return scrollView;
}
