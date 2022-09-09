#pragma once
#include "cCharacterSprite.h"

class cCharacterAnimation : public Serializer
{
public:
    cCharacterAnimation();
    ~cCharacterAnimation() override;

    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    DynamicSerializedVector<cCharacterSprite*>* m_Sprites;
    SerializedString m_Tags;
    SerializedString m_EventKey;

    std::string m_Key;

public:
    void AddSprite(cCharacterSprite* _sprite);
    cCharacterSprite* GetSprite(int _frame);
    void RemoveSprite(int _frame, bool _delete = true);
    void InsertSprite(cCharacterSprite* _sprite, int _offset);
    void InsertSprites(std::vector<cCharacterSprite*>& _sprites, int _offset);
    void SwapSprite(int _from, int _to);
    int GetLength() {return m_Sprites->GetLength();}
    void SetTags(std::string _tags) {m_Tags = _tags;}
    std::string GetTags() {return m_Tags;}
    void SetEventKey(std::string _eventKey) {m_EventKey = _eventKey;}
    std::string GetEventKey() {return m_EventKey;}

    void SetKey(std::string _key) {m_Key = _key;}
    std::string GetKey() {return m_Key;}
};
