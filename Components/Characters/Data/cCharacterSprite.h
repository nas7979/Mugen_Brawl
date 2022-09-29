#pragma once
#include "SpriteBoxes/cHitBox.h"
#include "SpriteBoxes/cHurtBox.h"
#include "SpriteBoxes/cThrowBox.h"
#include "SpriteBoxes/cBodyBox.h"

enum class AttachPointType : byte
{
    Body,
    Head,
    Neck,
    Chest,
    AttachTo,
    End
};

static const char* AttachPointTypeToStringMap[6] = 
{
    "Body",
    "Head",
    "Neck",
    "Chest",
    "AttachTo",
    "End"
};

struct AttachPoint
{
    short x = 0;
    short y = 0;
    AttachPointType type = AttachPointType::Body;

    Vec2 GetPos() const {return Vec2(x, y);}
    void Deserialize(char* _buffer, UINT& _pointer) {}
};

class cCharacterSprite : public Serializer, public cEventKey
{
public:
    cCharacterSprite() = default;
    ~cCharacterSprite() override;

private:
    short m_OffsetX = 0;
    short m_OffsetY = 0;
    short m_FrameLength = 1;
    short m_FixedPaletteIndex = 0;
    ShortVec2 m_Momentum = ShortVec2(0, 0);
    Vec2 m_Friction = Vec2(1, 1);
    SerializedArray<AttachPoint*> m_AttachPoints;
    DynamicSerializedArray<cHitBox*> m_HitBoxes;
    DynamicSerializedArray<cThrowBox*> m_ThrowBoxes;
    DynamicSerializedArray<cHurtBox*> m_HurtBoxes;
    DynamicSerializedArray<cBodyBox*> m_BodyBoxes;
    SerializedString m_ImageBinary;

    Texture* m_Texture = nullptr;

public:
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

    Texture* GetTexture() {return m_Texture;}
    bool SetTexture(std::wstring _path, PALETTEENTRY* _paletteEntries);
    void FlipHorizontal();

    ShortVec2 GetOffset() {return ShortVec2(m_OffsetX, m_OffsetY);}
    void SetOffset(ShortVec2 _offset) {m_OffsetX = _offset.x; m_OffsetY = _offset.y;}
    short GetFrameLength() {return m_FrameLength;}
    void SetFrameLength(short _length) {m_FrameLength = _length;}
    void SetFixedPaletteIndex(short _index) {m_FixedPaletteIndex = _index;}
    bool GetFixedPaletteIndex() {return m_FixedPaletteIndex;}
    ShortVec2 GetMomentum() {return m_Momentum;}
    void SetMomentum(ShortVec2 _momentum) {m_Momentum = _momentum;}
    Vec2 GetFriction() {return m_Friction;}
    void SetFriction(Vec2 _friction) {m_Friction = _friction;}

    int GetAttachPoints(AttachPoint**& _attachPoints) {_attachPoints = m_AttachPoints.GetValue(); return m_AttachPoints.GetLength();}
    void SetAttachPoints(AttachPoint** _attachPoints, int _length) {m_AttachPoints.SetValue(_attachPoints, _length);}

    int GetHitBoxes(cHitBox**& _pointer) {_pointer = m_HitBoxes.GetValue(); return m_HitBoxes.GetLength();}
    cHitBox* GetHitBox(int _index) {return m_HitBoxes[_index];}
    void SetHitBoxes(cHitBox** _pointer, int _length) {m_HitBoxes.SetValue(_pointer, _length);}
    int GetThrowBoxes(cThrowBox**& _pointer) {_pointer = m_ThrowBoxes.GetValue(); return m_ThrowBoxes.GetLength();}
    cThrowBox* GetThrowBox(int _index) {return m_ThrowBoxes[_index];}
    void SetThrowBoxes(cThrowBox** _pointer, int _length) {m_ThrowBoxes.SetValue(_pointer, _length);}
    int GetHurtBoxes(cHurtBox**& _pointer) {_pointer = m_HurtBoxes.GetValue(); return m_HurtBoxes.GetLength();}
    cHurtBox* GetHurtBox(int _index) {return m_HurtBoxes[_index];}
    void SetHurtBoxes(cHurtBox** _pointer, int _length) {m_HurtBoxes.SetValue(_pointer, _length);}
    int GetBodyBoxes(cBodyBox**& _pointer) {_pointer = m_BodyBoxes.GetValue(); return m_BodyBoxes.GetLength();}
    cBodyBox* GetBodyBox(int _index) {return m_BodyBoxes[_index];}
    void SetBodyBoxes(cBodyBox** _pointer, int _length) {m_BodyBoxes.SetValue(_pointer, _length);}
};
