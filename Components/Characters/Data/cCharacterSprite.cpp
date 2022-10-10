#include "DXUT.h"
#include "cCharacterSprite.h"

#include "../../../Scenes/Editor/cEditorScene.h"

cCharacterSprite::~cCharacterSprite()
{
    m_Texture->texture->Release();
    delete m_Texture;

    for (int i = 0 ; i < m_HurtBoxes.GetLength(); i++)
        delete m_HurtBoxes[i];
    for (int i = 0 ; i < m_HitBoxes.GetLength(); i++)
        delete m_HitBoxes[i];
    for (int i = 0 ; i < m_ThrowBoxes.GetLength(); i++)
        delete m_ThrowBoxes[i];
    for (int i = 0 ; i < m_BodyBoxes.GetLength(); i++)
        delete m_BodyBoxes[i];
}

void cCharacterSprite::Serialize(char* _buffer, UINT& _pointer) const
{
    CopyTo(_buffer, _pointer, &m_OffsetX, sizeof(short) * 6 + sizeof(Vec2));

    m_AttachPoints.Serialize(_buffer, _pointer);
    m_HitBoxes.Serialize(_buffer, _pointer);
    m_ThrowBoxes.Serialize(_buffer, _pointer);
    m_HurtBoxes.Serialize(_buffer, _pointer);
    m_BodyBoxes.Serialize(_buffer, _pointer);
    
    m_ImageBinary.Serialize(_buffer, _pointer);
    m_EventKey.Serialize(_buffer, _pointer);
}

void cCharacterSprite::Deserialize(char* _buffer, UINT& _pointer)
{
    CopyFrom(_buffer, _pointer, &m_OffsetX, sizeof(short) * 6 + sizeof(Vec2));

    m_AttachPoints.Deserialize(_buffer, _pointer);
    m_HitBoxes.Deserialize(_buffer, _pointer);
    m_ThrowBoxes.Deserialize(_buffer, _pointer);
    m_HurtBoxes.Deserialize(_buffer, _pointer);
    m_BodyBoxes.Deserialize(_buffer, _pointer);
    
    m_ImageBinary.Deserialize(_buffer, _pointer);
    m_Texture = new Texture;
    D3DXIMAGE_INFO info;
    if (D3DXCreateTextureFromFileInMemoryEx(g_Device, m_ImageBinary.GetBuffer(), m_ImageBinary.GetLength(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, &info, nullptr, &m_Texture->texture) != S_OK)
    {
        MessageBoxA(DXUTGetHWND(), "An error occurred while loading image.", "ERROR!", 0);
        return;
    }
    m_Texture->width = info.Width;
    m_Texture->height = info.Height;

    m_EventKey.Deserialize(_buffer, _pointer);
    SeparateEventKey();
}

size_t cCharacterSprite::GetSize() const
{
    return sizeof(short) * 6 + sizeof(Vec2) + m_AttachPoints.GetSize() +
        m_HitBoxes.GetSize() + m_ThrowBoxes.GetSize() + m_HurtBoxes.GetSize() + m_BodyBoxes.GetSize() +
            m_ImageBinary.GetSize() + m_EventKey.GetSize();
}

bool cCharacterSprite::SetTexture(std::wstring _path, PALETTEENTRY* _paletteEntries)
{
    if (m_Texture != nullptr)
    {
        m_Texture->texture->Release();
        delete m_Texture;
    }

    m_Texture = new Texture;
    D3DXIMAGE_INFO info;
    PALETTEENTRY entries[256];
    if (D3DXCreateTextureFromFileEx(g_Device, _path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, &info, entries, &m_Texture->texture) == S_OK)
    {
        m_Texture->width = info.Width;
        m_Texture->height = info.Height;
    }
    else
        return false;

    int usedPaletteLength = 255;
    UINT white = 0xff000000;
    // if (memcmp(&entries[1], &white, sizeof(int)) != 0)
    //     _paletteEntries = entries;
    
    for (usedPaletteLength = 255; usedPaletteLength >= 0; usedPaletteLength--)
    {
        if (memcmp(&_paletteEntries[usedPaletteLength], &white, sizeof(int)) != 0)
            break;
    }
    
    D3DLOCKED_RECT lockedRect;
    m_Texture->texture->LockRect(0, &lockedRect, 0, D3DLOCK_DISCARD);

    DWORD* colors = (DWORD*)lockedRect.pBits;
    for (int x = 0; x < m_Texture->width; x++)
    {
        for (int y = 0; y < m_Texture->height; y++)
        {
            int index = y * m_Texture->width + x;
            D3DXCOLOR color = colors[index];
            color.r = (int)(color.r * 255.f);
            color.g = (int)(color.g * 255.f);
            color.b = (int)(color.b * 255.f);
            color.a = (int)(color.a * 255.f);
            
            for (int i = 0; i <= usedPaletteLength; i++)
            {
                PALETTEENTRY entry = _paletteEntries[i];
                if ((BYTE)color.r == entry.peRed && (BYTE)color.g == entry.peGreen && (BYTE)color.b == entry.peBlue && (BYTE)color.a == entry.peFlags)
                {
                    color.r = i / 255.f;
                    color.g = i / 255.f;
                    color.b = i / 255.f;
                    colors[index] = color;
                    break;
                }
            }
        }
    }

    m_Texture->texture->UnlockRect(0);
    
    LPD3DXBUFFER buffer;
    if (D3DXSaveTextureToFileInMemory(&buffer, D3DXIFF_PNG, m_Texture->texture, _paletteEntries) != S_OK)
    {
        MessageBoxA(DXUTGetHWND(), "An error occurred while saving image.", "ERROR!", 0);
    }

    m_ImageBinary = std::string((LPCSTR)buffer->GetBufferPointer(), buffer->GetBufferSize());
    buffer->Release();
    
    return true;
}

void cCharacterSprite::FlipHorizontal()
{
    D3DLOCKED_RECT lockedRect;
    m_Texture->texture->LockRect(0, &lockedRect, 0, D3DLOCK_DISCARD);

    DWORD* original = new DWORD[m_Texture->width * m_Texture->height];
    memcpy(original, lockedRect.pBits, m_Texture->width * m_Texture->height * sizeof(DWORD));
    DWORD* colors = (DWORD*)lockedRect.pBits;
    
    for (int x = 0; x < m_Texture->width; x++)
    {
        for (int y = 0; y < m_Texture->height; y++)
        {
            int index = y * m_Texture->width + x;
            colors[index] = original[y * m_Texture->width + (m_Texture->width - 1 - x)];
        }
    }

    m_Texture->texture->UnlockRect(0);
    delete[] original;

    LPD3DXBUFFER buffer;
    if (D3DXSaveTextureToFileInMemory(&buffer, D3DXIFF_PNG, m_Texture->texture, nullptr) != S_OK)
    {
        MessageBoxA(DXUTGetHWND(), "An error occurred while saving image.", "ERROR!", 0);
    }

    m_ImageBinary = std::string((LPCSTR)buffer->GetBufferPointer(), buffer->GetBufferSize());
    buffer->Release();
}

AttachPoint* cCharacterSprite::GetAttachPoint(AttachPointType _type) const
{
    for (int i = 0; i < m_AttachPoints.GetLength(); i++)
    {
        AttachPoint* point = m_AttachPoints[i];
        if (point->type == _type)
            return point;
    }

    return nullptr;
}
