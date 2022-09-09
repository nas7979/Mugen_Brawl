#pragma once

class cGUI : public cComponent
{
public:
    cGUI(cObject* _owner) : cComponent(_owner) {}
    ~cGUI() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    virtual void OnClick();
    virtual void OnDoubleClick();
    virtual void OnClickUp();
    virtual void OnClickOutSide();

protected:
    Vec2 m_Size = Vec2(0,0);
    Vec2 m_TextPivot = Vec2(0.5f,0.5f);
    float m_FontSize = 18;
    std::string m_Text;
    D3DCOLOR m_BackgroundColor = 0xffe0e0e0;
    D3DCOLOR m_OutlineColor = 0xff000000;
    bool m_IsClicked = false;
    int m_DoubleClickInterval = 0;
    bool m_InactivateOnClickOutside = false;

public:
    void SetSize(Vec2 _size) {m_Size = _size;}
    Vec2 GetSize() {return m_Size;}
    void SetTextPivot(Vec2 _pivot) {m_TextPivot = _pivot;}
    Vec2 GetTextPivot() {return m_TextPivot;}
    void SetFontSize(int _size) {m_FontSize = _size;}
    void SetText(std::string _text) {m_Text = _text;}
    std::string GetText() {return m_Text;}
    void SetBackgroundColor(D3DCOLOR _color) {m_BackgroundColor = _color;}
    void SetOutlineColor(D3DCOLOR _color) {m_OutlineColor = _color;}
    void SetInactivateOnClickOutside(bool _set) {m_InactivateOnClickOutside = _set;}
    int GetDoubleClickInterval() {return m_DoubleClickInterval;}

    cGUI* SetDefaults(Vec2 _size, std::string _text, D3DCOLOR _backgroundColor = 0xffe0e0e0, D3DCOLOR _outlineColor = 0xff000000);
    bool IsMouseOver();
};
