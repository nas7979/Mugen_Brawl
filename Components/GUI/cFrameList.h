#pragma once
#include "../Characters/Data/cCharacterAnimation.h"

class cFrameList : public cGUI, cMsgProcHandler
{
public:
    cFrameList(cObject* _owner) : cGUI(_owner), cMsgProcHandler(_owner) {}
    ~cFrameList() override;
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
    cCharacterAnimation* m_CurAnimation = nullptr;
    std::vector<int> m_SelectedFrames;
    int m_DragStartedFrame = -1;
    float m_ScrollPos = 0;
    int m_DraggedFramesLeft = -1;
    int m_DraggedFramesRight = -1;
    bool m_CanMoveFrames = false;
    int m_MoveFramesTo = -1;
    int m_FrameTimer = 0;
    bool m_isPlaying = false;
    const int m_ElementWidth = 25;
    const int m_ElementHeight = 30;
    const int m_ScrollbarHeight = 10;

    int CursorPosToFrame();
public:
    void SetAnimation(cCharacterAnimation* _anim);
    cCharacterAnimation* GetAnimation() {return m_CurAnimation;}
    void SetFrame(int _frame);
    int GetFrame();
    void CopyFrames();
    void PasteFrames();
    bool CanEdit() {return m_CurAnimation != nullptr && m_CurAnimation->GetLength() > 0;}
    void SetScrollPos(float _pos);
    void WithSelectedFrames(std::function<void(cCharacterSprite*)> _func);
};
