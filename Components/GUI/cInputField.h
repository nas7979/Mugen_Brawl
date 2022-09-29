#pragma once
#include "cGUI.h"
#include "../../Managers/Input/cMsgProcHandler.h"

class cInputField : public cGUI, cMsgProcHandler
{
public:
    cInputField(cObject* _owner) : cGUI(_owner), cMsgProcHandler(_owner) {}
    ~cInputField() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void OnClick() override;
    void OnDoubleClick() override;
    void OnClickUp() override;
    void OnClickOutSide() override;
    void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    std::string m_Text;
    int m_CursorPos = -1;
    int m_DragStartedPos = -1;
    LPD3DXFONT m_Font = nullptr;
    std::vector<int> m_LinebreakAt;
    float m_FontSize;
    D3DCOLOR m_TextColor = 0xff000000;
    int m_SpaceWidth = 0;
    std::string m_DescText;
    std::function<void(cInputField*)> m_OnTextChanged = nullptr;

    int GetCursorPos();
    int PosToPixelPos(int _pos);

public:
    cInputField* SetFont(std::string _font);
    cInputField* SetFontSize(int _size) {m_FontSize = _size / 64.f; return this;}
    cInputField* SetTextColor(D3DCOLOR _color) {m_TextColor = _color; return this;}
    cInputField* SetDesc(std::string _desc) {m_DescText = _desc; return this;}
    cInputField* SetOnTextChanged(std::function<void(cInputField*)> _onTextChanged) {m_OnTextChanged = _onTextChanged; return this;}
    bool IsFocused() {return m_CursorPos >= 0;}
    std::string GetText() {return m_Text;}
    int GetInt();
    float GetFloat();
    void SetText(std::string _text, bool _callOnChanged = true);
};
