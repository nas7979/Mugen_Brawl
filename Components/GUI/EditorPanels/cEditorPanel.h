#pragma once
#include "../../Characters/Data/cCharacterData.h"

class cButton;
class cDropDown;
class cInputField;
class cScrollView;

class cEditorPanel : public cComponent
{
public:
    cEditorPanel(cObject* _owner) : cComponent(_owner) {}
    ~cEditorPanel() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

protected:
    std::vector<cObject*> m_Objects;
    cCharacterData* m_Data = nullptr;

public:
    void Show(bool _show);
    void virtual Save(cCharacterData* _data) = 0;
    void virtual Load(cCharacterData* _data) = 0;
    void virtual Reset();
    void virtual OnShow() = 0;
    void virtual OnHide() = 0;

    cButton* CreateButton(Vec2 _pos, Vec2 _size, std::string _text, std::function<void()> _onClick, bool _addToObjects = true);
    cDropDown* CreateDropDown(Vec2 _pos, Vec2 _size, float _dropDownWidth, std::string _text, bool _addToObjects = true);
    cInputField* CreateInputField(Vec2 _pos, Vec2 _size, std::string _desc, int _fontSize, bool _addToObjects = true, D3DCOLOR _textColor = 0xff000000, std::string _font = "Gothic");
    cScrollView* CreateScrollView(Vec2 _pos, Vec2 _size, Vec2 _elementSize, std::function<void(cObject*)> _onClick, bool _isActive, bool _addToObjects = true);
};
