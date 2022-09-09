#pragma once

class cSpriteBox : public Serializer
{
public:
    cSpriteBox() = default;
    cSpriteBox(short _left, short _top, short _right, short _bottom) : m_Left(_left), m_Top(_top), m_Right(_right), m_Bottom(_bottom) {}
    ~cSpriteBox() override = default;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    short m_Left, m_Top, m_Right, m_Bottom;
    SerializedString m_EventKey;

public:
    void SetRect(short _left, short _top, short _right, short _bottom)
    {
        m_Left = _left;
        m_Top = _top;
        m_Right = _right;
        m_Bottom = _bottom;
    }

    RECT GetRect() {return RECT{m_Left, m_Top, m_Right, m_Bottom};}

    short GetLeft() const {return  m_Left;}
    void SetLeft(short _left) {m_Left = _left;}
    short GetTop() const {return  m_Top;}
    void SetTop(short _top) {m_Top = _top;}
    short GetRight() const {return  m_Right;}
    void SetRight(short _right) {m_Right = _right;}
    short GetBottom() const {return  m_Bottom;}
    void SetBottom(short _bottom) {m_Bottom = _bottom;}

    std::string GetEventKey() {return m_EventKey;}
    void SetEventKey(const std::string& _key) {m_EventKey = _key;}
};
