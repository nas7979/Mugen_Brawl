#include "DXUT.h"
#include "cInputField.h"

cInputField::~cInputField()
{
}

void cInputField::Init()
{
    cGUI::Init();
}

void cInputField::Update()
{
    if (m_IsClicked)
    {
        m_CursorPos = GetCursorPos();
    }
    
    cGUI::Update();
}

void cInputField::Render()
{
    cGUI::Render();
    Vec3 textStartPos = m_Owner->GetPos() - Vec3(m_Size.x * 0.5f - 5, 0, 0);
    IMAGE->RenderText(m_Font, m_Text, textStartPos, 0, Vec2(1, 1) * m_FontSize, Vec2(0, 0.5), m_TextColor, true);

    if (IsFocused())
    {
        int width = PosToPixelPos(m_CursorPos);
        
        IMAGE->RenderSprite(IMAGE->GetTexture("Pixel"), textStartPos + Vec3(width, 0, 0), 0, Vec2(2, m_FontSize * 64), Vec2(0.5, 0.5), 0xff000000, true);

        if (m_DragStartedPos != m_CursorPos)
        {
            int dragStartedPixelPos = PosToPixelPos(m_DragStartedPos);
            IMAGE->RenderSprite(IMAGE->GetTexture("Pixel"), textStartPos + Vec3(dragStartedPixelPos, 0, 0), 0, Vec2(width - dragStartedPixelPos, m_FontSize * 64), Vec2(0, 0.5), 0x900000ff, true);
        }
    }

    if (!m_DescText.empty())
    {
        IMAGE->RenderText(m_Font, m_DescText, m_Owner->GetPos() - Vec3(m_Size.x * 0.5f - 5, m_Size.y * 0.5f + 10, 0), 0, Vec2(0.25f, 0.25f), Vec2(0, 0.5f), 0xff000000, true);
    }
}

void cInputField::Release()
{
    cGUI::Release();
}

void cInputField::OnCollision(cObject* _other)
{
    cGUI::OnCollision(_other);
}

void cInputField::OnAlarm(std::string _key)
{
    cGUI::OnAlarm(_key);
}

void cInputField::OnClick()
{
    cGUI::OnClick();
    m_CursorPos = m_DragStartedPos = GetCursorPos();
}

void cInputField::OnDoubleClick()
{
    cGUI::OnDoubleClick();

    if (m_Text.empty())
        return;
    
    char startChar = m_Text[max(0, m_CursorPos - 1)];
    for (int i = m_CursorPos - 1; i >= 0; i--)
    {
        if (i == 0 || m_Text[i] == ' ' && m_Text[i - 1] != startChar)
        {
            m_CursorPos = m_Text[i] == ' ' && startChar != m_Text[i] ? i + 1 : i;
            break;
        }
    }
}

void cInputField::OnClickUp()
{
    cGUI::OnClickUp();
}

void cInputField::OnClickOutSide()
{
    cGUI::OnClickOutSide();
    m_CursorPos = -1;
}

void cInputField::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!GetOwner()->IsActive() || !IsFocused())
        return;

    switch (uMsg)
    {
    case WM_CHAR:
        if (INPUT->KeyPress(VK_LCONTROL))
        {
            return;
        }
        
        if (m_CursorPos != m_DragStartedPos)
        {
            int min = min(m_CursorPos, m_DragStartedPos);
            int max = max(m_CursorPos, m_DragStartedPos);
            m_Text.erase(min, max - min);
            m_CursorPos = m_DragStartedPos = min;
        }
        
        if (wParam == VK_BACK)
        {
            if (m_CursorPos > 0)
            {
                m_Text.erase(m_CursorPos - 1, 1);
                m_CursorPos--;
            }
        }
        else
        {
            m_Text.insert(m_CursorPos, 1, wParam);
            m_CursorPos++;
        }
        m_DragStartedPos = m_CursorPos;
        if (m_OnTextChanged != nullptr)
            m_OnTextChanged(this);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_RETURN)
        {
            m_CursorPos = m_DragStartedPos = -1;
            return;
        }
        
        if (INPUT->KeyPress(VK_LCONTROL))
        {
            if (wParam == 'A')
            {
                m_DragStartedPos = 0;
                m_CursorPos = m_Text.size();
            }

            if (wParam == 'C')
            {
                int min = min(m_CursorPos, m_DragStartedPos);
                int max = max(m_CursorPos, m_DragStartedPos);
                
                SerializedString str = m_CursorPos != m_DragStartedPos ? m_Text.substr(min, max - min) : m_Text;
                INPUT->SetClipboard(CF_STRING, str);
            }
            else if (wParam == 'V')
            {
                SerializedString str;
                if (INPUT->GetClipboard(CF_STRING, str))
                {
                    int min = min(m_CursorPos, m_DragStartedPos);
                    int max = max(m_CursorPos, m_DragStartedPos);
                    m_Text.erase(min, max - min);

                    m_Text.insert(min, str);
                    m_CursorPos = m_DragStartedPos = min + str.GetLength();
                
                    if (m_OnTextChanged != nullptr)
                        m_OnTextChanged(this);
                }
            }
            
            if (wParam == VK_LEFT && m_CursorPos > 0)
            {
                if (m_CursorPos != m_DragStartedPos)
                    m_CursorPos = min(m_CursorPos, m_DragStartedPos);
                
                char startChar = m_Text[m_CursorPos - 1];
                for (int i = m_CursorPos - 1; i >= 0; i--)
                {
                    if (i == 0 || m_Text[i] == ' ' && m_Text[i - 1] != startChar)
                    {
                        m_CursorPos = m_Text[i] == ' ' && startChar != m_Text[i] ? i + 1 : i;
                        break;
                    }
                }

                if (!INPUT->KeyPress(VK_LSHIFT))
                    m_DragStartedPos = m_CursorPos;
            }

            if (wParam == VK_RIGHT && m_CursorPos < m_Text.size())
            {
                if (m_CursorPos != m_DragStartedPos)
                    m_CursorPos = max(m_CursorPos, m_DragStartedPos);
                
                char startChar = m_Text[m_CursorPos];
                for (int i = m_CursorPos; i < m_Text.size(); i++)
                {
                    if (i == m_Text.size() - 1 || m_Text[i] == ' ' && m_Text[i + 1] != startChar)
                    {
                        m_CursorPos = m_Text[i] == ' ' && startChar != m_Text[i] ? i : i + 1;
                        break;
                    }
                }

                if (!INPUT->KeyPress(VK_LSHIFT))
                    m_DragStartedPos = m_CursorPos;
            }
        }
        else if (INPUT->KeyPress(VK_LSHIFT))
        {
            if (wParam == VK_LEFT && m_CursorPos > 0)
            {
                m_CursorPos--;
            }

            if (wParam == VK_RIGHT && m_CursorPos < m_Text.size())
            {
                m_CursorPos++;
            }
        }
        else
        {
            if (wParam == VK_LEFT)
            {
                if (m_CursorPos > 0)
                {
                    if (m_CursorPos != m_DragStartedPos)
                        m_CursorPos = min(m_CursorPos, m_DragStartedPos);
                    else
                        m_CursorPos--;
                }
                else
                {
                    m_CursorPos = 0;
                }
                m_DragStartedPos = m_CursorPos;
            }

            if (wParam == VK_RIGHT)
            {
                if (m_CursorPos < m_Text.size())
                {
                    if (m_CursorPos != m_DragStartedPos)
                        m_CursorPos = max(m_CursorPos, m_DragStartedPos);
                    else
                        m_CursorPos++;
                }
                else
                {
                    m_CursorPos = m_Text.size();
                }
                m_DragStartedPos = m_CursorPos;
            }
        }
        
        break;
    }
}

int cInputField::GetCursorPos()
{
    Vec2 localMousePos = (INPUT->GetMousePos() - ((Vec2)m_Owner->GetPos() - m_Size * 0.5f )) / m_FontSize;
    localMousePos.x -= 5;
    int pos = min(m_Text.size(), localMousePos.x / 64);
    int width = IMAGE->GetTextWidth(m_Font, m_Text.c_str(), 0, pos);

    int lastCharWidth = 0;
    for(; pos < m_Text.size() && localMousePos.x > width; pos++)
    {
        if (m_Text[pos] == ' ')
            lastCharWidth = m_SpaceWidth;
        else
            lastCharWidth = IMAGE->GetTextWidth(m_Font, m_Text.c_str(), pos, 1);
        width += lastCharWidth;
    }

    if (width - lastCharWidth * 0.5f > localMousePos.x)
    {
        pos--;
        width -= lastCharWidth;
    }

    return max(0, pos);
}

int cInputField::PosToPixelPos(int _pos)
{
    int pixelPos = 0;
    for(int i = 0; i < _pos; i++)
    {
        if (m_Text[i] == ' ')
            pixelPos += m_SpaceWidth;
        else
            pixelPos += IMAGE->GetTextWidth(m_Font, m_Text.c_str(), i, 1);
    }
    
    return pixelPos * m_FontSize;
}

cInputField* cInputField::SetFont(std::string _font)
{
    m_Font = IMAGE->GetFont(_font);

    m_SpaceWidth = IMAGE->GetTextWidth(m_Font, " 1", 0, 2);
    m_SpaceWidth -= IMAGE->GetTextWidth(m_Font, "1", 0, 1);
    return this;
}

int cInputField::GetInt()
{
    return atoi(m_Text.c_str());
}

float cInputField::GetFloat()
{
    return atof(m_Text.c_str());
}

void cInputField::SetText(std::string _text, bool _callOnChanged)
{
    m_Text = _text;
    if (_callOnChanged && m_OnTextChanged != nullptr)
        m_OnTextChanged(this);

    if (m_CursorPos > 0)
        m_CursorPos = 0;

    m_DragStartedPos = m_CursorPos;
}
