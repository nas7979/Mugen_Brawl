#include "DXUT.h"
#include "cCharacterAnimation.h"

#include "../../../Scenes/Editor/cEditorScene.h"
#include "../../Components/GUI/EditorPanels/cEditorSpritePanel.h"

cCharacterAnimation::cCharacterAnimation()
{
    m_Sprites = new DynamicSerializedVector<cCharacterSprite*>();
}

cCharacterAnimation::~cCharacterAnimation()
{
    for(auto& iter : *m_Sprites->GetValue())
    {
        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->UnlinkRemovedOnionSkinSprites(iter);
        delete iter;
    }
    delete m_Sprites;
}

void cCharacterAnimation::Serialize(char* _buffer, UINT& _pointer) const
{
    m_Sprites->Serialize(_buffer, _pointer);
    m_Tags.Serialize(_buffer, _pointer);
    m_EventKey.Serialize(_buffer, _pointer);
}

void cCharacterAnimation::Deserialize(char* _buffer, UINT& _pointer)
{
    m_Sprites->Deserialize(_buffer, _pointer);
    m_Tags.Deserialize(_buffer, _pointer);
    m_EventKey.Deserialize(_buffer, _pointer);
    SeparateEventKey();
}

size_t cCharacterAnimation::GetSize() const
{
    return m_Sprites->GetSize() + m_Tags.GetSize() + m_EventKey.GetSize();
}

void cCharacterAnimation::AddSprite(cCharacterSprite* _sprite)
{
    m_Sprites->GetValue()->push_back(_sprite);
}

cCharacterSprite* cCharacterAnimation::GetSprite(int _frame)
{
    if (_frame >= m_Sprites->GetLength())
        return nullptr;
    
    return (*m_Sprites)[_frame];
}

void cCharacterAnimation::RemoveSprite(int _frame, bool _delete)
{
    if (_delete)
    {
        SCENE->GetScene<cEditorScene>()->GetSpritePanel()->UnlinkRemovedOnionSkinSprites((*m_Sprites)[_frame]);
        delete (*m_Sprites)[_frame];
    }
    m_Sprites->GetValue()->erase(m_Sprites->GetValue()->_Make_iterator_offset(_frame));
}

void cCharacterAnimation::InsertSprite(cCharacterSprite* _sprite, int _offset)
{
    m_Sprites->GetValue()->insert(m_Sprites->GetValue()->_Make_iterator_offset(_offset), _sprite);
}

void cCharacterAnimation::InsertSprites(std::vector<cCharacterSprite*>& _sprites, int _offset)
{
    m_Sprites->GetValue()->insert(m_Sprites->GetValue()->_Make_iterator_offset(_offset), _sprites.begin(), _sprites.end());
}

void cCharacterAnimation::SwapSprite(int _from, int _to)
{
    std::swap(m_Sprites[_from], m_Sprites[_to]);
}