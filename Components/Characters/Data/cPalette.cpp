#include "DXUT.h"
#include "cPalette.h"

cPalette::~cPalette()
{
    m_Texture->Release();
}

void cPalette::Serialize(char* _buffer, UINT& _pointer) const
{
    CopyTo(_buffer, _pointer, (void*)&m_Index, sizeof(short));
    m_Entries.Serialize(_buffer, _pointer);
}

void cPalette::Deserialize(char* _buffer, UINT& _pointer)
{
    CopyFrom(_buffer, _pointer, &m_Index, sizeof(short));
    m_Entries.Deserialize(_buffer, _pointer);
    CreateTexture();
}

size_t cPalette::GetSize() const
{
    return sizeof(short) + m_Entries.GetSize();
}

void cPalette::CreateTexture()
{
    g_Device->CreateTexture(32, 32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_Texture, nullptr);
    UpdateTexture();
}

void cPalette::UpdateTexture()
{
    D3DLOCKED_RECT rect;
    m_Texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);

    DWORD* colors = (DWORD*)rect.pBits;
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            int index = y * 32 + x;
            if (m_Entries.GetLength() <= index)
                goto out;
            PALETTEENTRY color = m_Entries[index];
            colors[index] = D3DXCOLOR(color.peRed / 255.f, color.peGreen / 255.f, color.peBlue / 255.f, color.peFlags / 255.f);
        }
    }

    out:
    m_Texture->UnlockRect(0);
}

D3DCOLOR cPalette::GetColor(int _index)
{
    PALETTEENTRY color = m_Entries[_index];
    return D3DCOLOR_ARGB(color.peFlags, color.peRed, color.peGreen, color.peBlue);
}

void cPalette::SetColor(int _index, BYTE _a, BYTE _r, BYTE _g, BYTE _b)
{
    m_Entries[_index] = PALETTEENTRY {_r, _g, _b, _a};
}
