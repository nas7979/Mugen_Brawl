#pragma once
#include "cButton.h"

class cDropDown : public cGUI
{
public:
    cDropDown(cObject* _owner) : cGUI(_owner) {}
    ~cDropDown() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void OnClick() override;
    void OnClickOutSide() override;

private:
    bool m_isOpened = false;
    std::vector<cButton*> m_Buttons;
    float m_DropDownWidth = 300;

public:
    cDropDown* AddButton(cButton* _button);
    cDropDown* SetDropDownWidth(float _width) {m_DropDownWidth = _width; return this;}
};
