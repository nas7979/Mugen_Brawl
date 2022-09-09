#pragma once
#include "cGUI.h"

class cButton : public cGUI
{
public:
    cButton(cObject* _owner) : cGUI(_owner) {}
    ~cButton() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void OnClick() override;

private:
    std::function<void()> m_OnClick = nullptr;

public:
    cButton* SetOnClick(std::function<void()> _onClick) {m_OnClick = _onClick; return this;}
};
