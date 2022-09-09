#pragma once

class cMainGame
{
public:
    void Init();
    void Update();
    void Render();
    void MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Release();
    void OnLostDevice();
    void OnResetDevice();
};
