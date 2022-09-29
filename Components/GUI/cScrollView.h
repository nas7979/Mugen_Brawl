#pragma once
#include "cButton.h"

class cScrollView : public cGUI, cMsgProcHandler
{
public:
    cScrollView(cObject* _owner) : cGUI(_owner), cMsgProcHandler(_owner) {}
    ~cScrollView() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void OnClick() override;
    void OnClickOutSide() override;
    void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    std::vector<cObject*> m_Elements;
    const int m_ScrollbarWidth = 10;
    Vec2 m_ElementSize = Vec2(30, 30);
    Vec3 m_ElementsOffset = Vec3(0,0, 0);
    float m_ScrollPos = 0;
    std::function<void(cObject*)> m_OnClick = nullptr;
    int m_SelectedIndex = -1;
    bool m_ResetSelectedOnClickOutSide = false;

    void ReloadElementsPosition();

public:
    void AddElement(cGUI* _element);
    void RemoveElement(cObject* _element);
    void RemoveAllElement();
    void SetScrollPos(float _pos);
    void SetSelectedIndex(int _index) {m_SelectedIndex = min(_index + 1, m_Elements.size()) - 1;}
    void SetResetSelectedOnClickOutSide(bool _set) {m_ResetSelectedOnClickOutSide = _set;}
    cObject* GetSelectedObject();
    int GetSelectedIndex() {return m_SelectedIndex;}
    cScrollView* SetOnClick(std::function<void(cObject*)> _onClick) {m_OnClick = _onClick; return this;}
    cScrollView* SetElementSize(Vec2 _size) {m_ElementSize = _size; return this;}
    cScrollView* SetElementOffset(Vec2 _offset) {m_ElementsOffset = Vec3(_offset.x, _offset.y, 0); return this;}
};
