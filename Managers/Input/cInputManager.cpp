#include "DXUT.h"
#include "cInputManager.h"

void cInputManager::Init()
{
}

void cInputManager::Update()
{
    if (GetFocus() != DXUTGetHWND())
        return;
    
    m_InputBufferClearTimer--;
    if (m_InputBufferClearTimer >= 0)
        m_InputBuffer.clear();
    
    memcpy(m_OldKeys, m_CurKeys, sizeof(m_CurKeys));
    memset(m_CurKeys, 0, sizeof(m_CurKeys));

    for(int i = 0; i < 256; i++)
    {
        if (GetAsyncKeyState(i) & 0x8000)
        {
            m_CurKeys[i] = true;
            m_InputBuffer.push_back(i);
            m_InputBufferClearTimer = INPUT_BUFFER_CLEAR_DELAY;
        }
    }

    m_GameInput = 0;
    for (int i = 0; i < (int)IngameInput::End; i++)
    {
        m_GameInput |= m_CurKeys[m_GameInputBindings[i]] << i; 
    }
    
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(DXUTGetHWND(), &pt);
    m_MousePos = Vec2(pt.x, pt.y);
    m_MousePos.x /= IMAGE->GetScreenScale().x;
    m_MousePos.y /= IMAGE->GetScreenScale().y;

    if (!m_ClickedGUIs.empty())
    {
        std::sort(m_ClickedGUIs.begin(), m_ClickedGUIs.end(), [](cGUI* a, cGUI* b)->bool{return a->GetOwner()->GetPos().z < b->GetOwner()->GetPos().z;});
        m_ClickedGUIs[0]->GetDoubleClickInterval() > 0 ? m_ClickedGUIs[0]->OnDoubleClick() : m_ClickedGUIs[0]->OnClick();
        for (int i = 1; i < m_ClickedGUIs.size(); i++)
        {
            m_ClickedGUIs[i]->OnClickOutSide();
        }

        m_ClickedGUIs.clear();
    }

    if (KeyPress(VK_LCONTROL))
    {
        if (KeyDown('Z') && !m_UndoQueue.empty())
        {
            m_UndoQueue.front().undo();
            m_RedoQueue.push_front(m_UndoQueue.front());
            m_UndoQueue.erase(m_UndoQueue.begin());
        }
        if (KeyDown('Y') && !m_RedoQueue.empty())
        {
            m_RedoQueue.front().redo();
            m_UndoQueue.push_front(m_RedoQueue.front());
            m_RedoQueue.erase(m_RedoQueue.begin());
        }
    }
}

void cInputManager::Render()
{
    m_WindowInputBuffer.clear();
}

void cInputManager::HandleWindowInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_CHAR:
            m_WindowInputBuffer.push_back((char)wParam);
        break;
    }

    for (auto handler : m_MsgProcHandlers)
    {
        handler->MsgProc(uMsg, wParam, lParam);
    }
}

void cInputManager::Release()
{
}

void cInputManager::UnsubscribeMsgProcHandler(cMsgProcHandler* _handler)
{
    for (auto& iter = m_MsgProcHandlers.begin(); iter != m_MsgProcHandlers.end(); iter++)
    {
        if ((*iter) == _handler)
        {
            m_MsgProcHandlers.erase(iter);
            return;
        }
    }
}

bool cInputManager::SetClipboard(UINT _format, Serializer& _serializer)
{
    HANDLE mem = GlobalAlloc(GMEM_FIXED, _serializer.GetSize());
    void* data = GlobalLock(mem);
    if (data == nullptr)
        return false;

    UINT pointer = 0;
    _serializer.Serialize((char*)data, pointer);
    GlobalUnlock(mem);

    if (!OpenClipboard(DXUTGetHWND()))
        return false;

    EmptyClipboard();
    SetClipboardData(_format, mem);
    CloseClipboard();

    return true;
}

bool cInputManager::GetClipboard(UINT _format, Serializer& _serializer)
{
    if (_format != GetPriorityClipboardFormat(&_format, 1))
        return false;

    if (!OpenClipboard(DXUTGetHWND()))
        return false;

    HANDLE mem = GetClipboardData(_format);
    void* data = GlobalLock(mem);
    if (data == nullptr)
        return false;

    UINT pointer = 0;
    _serializer.Deserialize((char*)data, pointer);

    GlobalUnlock(data);
    CloseClipboard();

    return true;
}

void cInputManager::Do(const std::function<void()>& _do, const std::function<void()>& _undo)
{
    _do();
    m_UndoQueue.push_front(Action {_do, _undo});
    if (m_UndoQueue.size() > m_DoQueueMaxCount)
        m_UndoQueue.erase(--m_UndoQueue.end());

    m_RedoQueue.clear();
}

void cInputManager::ClearDoQueue()
{
    m_UndoQueue.clear();
    m_RedoQueue.clear();
}

void cInputManager::ReadAndClearInputBuffer()
{
    for (auto iter : m_InputBuffer)
    {
        m_CurKeys[iter] = true;
    }
    m_InputBuffer.clear();
}
