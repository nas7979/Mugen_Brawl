#include "DXUT.h"
#include "cEditorPanel.h"

#include "../cButton.h"
#include "../cDropDown.h"
#include "../cInputField.h"
#include "../cScrollView.h"
#include "../../../Scenes/Editor/cEditorScene.h"

cEditorPanel::~cEditorPanel()
{
}

void cEditorPanel::Init()
{
}

void cEditorPanel::Update()
{
}

void cEditorPanel::Render()
{
}

void cEditorPanel::Release()
{
}

void cEditorPanel::OnCollision(cObject* _other)
{
}

void cEditorPanel::OnAlarm(std::string _key)
{
}

void cEditorPanel::Show(bool _show)
{
    if (_show != m_Owner->IsActive())
    {
        if (_show)
            OnShow();
        else
            OnHide();
    }
    
    m_Owner->SetActive(_show);
    for (auto object : m_Objects)
    {
        object->SetActive(_show);
    }
}

void cEditorPanel::Reset()
{
    m_Data = SCENE->GetScene<cEditorScene>()->GetCharacterData();
}

cButton* cEditorPanel::CreateButton(Vec2 _pos, Vec2 _size, std::string _text, std::function<void()> _onClick, bool _addToObjects)
{
    cButton* obj = cEditorScene::CreateButton(_pos, _size, _text, _onClick);
    if (_addToObjects)
        m_Objects.push_back(obj->GetOwner());
    return obj;
}

cDropDown* cEditorPanel::CreateDropDown(Vec2 _pos, Vec2 _size, float _dropDownWidth, std::string _text, bool _addToObjects)
{
    cDropDown* obj = cEditorScene::CreateDropDown(_pos, _size, _dropDownWidth, _text);
    if (_addToObjects)
        m_Objects.push_back(obj->GetOwner());
    return obj;
}

cInputField* cEditorPanel::CreateInputField(Vec2 _pos, Vec2 _size, std::string _desc, int _fontSize, bool _addToObjects, D3DCOLOR _textColor, std::string _font)
{
    cInputField* obj = cEditorScene::CreateInputField(_pos, _size, _desc, _fontSize, _textColor, _font);
    if (_addToObjects)
        m_Objects.push_back(obj->GetOwner());
    return obj;
}

cScrollView* cEditorPanel::CreateScrollView(Vec2 _pos, Vec2 _size, Vec2 _elementSize, std::function<void(cObject*)> _onClick, bool _isActive, bool _addToObjects)
{
    cScrollView* obj = cEditorScene::CreateScrollView(_pos, _size, _elementSize, _onClick, _isActive);
    if (_addToObjects)
        m_Objects.push_back(obj->GetOwner());
    return obj;
}
