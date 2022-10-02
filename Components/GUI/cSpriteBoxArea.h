#pragma once

#include "../Characters/Data/cCharacterAnimation.h"
#include "../Characters/Data/SpriteBoxes/cSpriteBox.h"

class cSpriteBoxArea : public cGUI, cMsgProcHandler
{
public:
    enum DrawType
    {
        HurtBox = 0x00000001,
        HitBox = 0x00000002,
        BodyBox = 0x00000004,
        ThrowBox = 0x00000008,
        AttachPoint = 0x00000010,
        Select = 0x00000020
    };

    struct SelectedBox
    {
        SelectedBox(cSpriteBox* _box, ::AttachPoint* _attachPoint, DrawType _type)
        {
            box = _box;
            attachPoint = _attachPoint;
            type = _type;
        }
        cSpriteBox* box;
        ::AttachPoint* attachPoint;
        DrawType type;
    };

    cSpriteBoxArea(cObject* _owner) : cGUI(_owner), cMsgProcHandler(_owner) {}
    ~cSpriteBoxArea() override;
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
    cCharacterSprite* m_CurSprite = nullptr;
    DrawType m_SelectedDrawType = Select;
    Vec2 m_DrawStartedPos = Vec2(-1, -1);
    std::vector<SelectedBox> m_SelectedSpriteBoxes;
    int m_OverlappedBoxSelectIndex = 0;
    
    template<typename T>
    void GetBoxGetSetFunc(DrawType _type, std::function<int(T**&)>& _getFunc, std::function<void(T**, int)>& _setFunc);
    template<typename T>
    void DeleteBox(std::pair<DrawType, int> _typeAndIndex);
    template<typename T>
    void UndoDeleteBox(std::pair<DrawType, int> _typeAndIndex, char* _buffer, UINT& _pointer);
    template<typename T>
    void AddBox(DrawType _type, T* _box);
    template<typename T>
    void UndoAddBox(DrawType _type);

public:
    D3DCOLOR static GetBoxColor(DrawType _type, bool _isSelected);
private:
    void WithBoxes(const std::function<void(cSpriteBox*, DrawType)>& _func);
    int GetBoxIndex(SelectedBox _box);

public:
    void SetSprite(cCharacterSprite* _sprite);
    bool HasSelectedBox() {return !m_SelectedSpriteBoxes.empty();}
    void WithSelectedBoxes(std::function<void(SelectedBox)> _func) const;
};

#define BOXCL_HURT 0x7f0050ff
#define BOXCL_HIT 0x7fff0000
#define BOXCL_BODY 0x7fffff00
#define BOXCL_THROW 0x7fff00ff
#define BOXCL_ATTACH 0x7fa0a0a0