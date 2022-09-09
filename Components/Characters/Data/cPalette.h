#pragma once

class cPalette : public Serializer
{
public:
    cPalette() {}
    cPalette(short _index, PALETTEENTRY* _entries, int _length) : m_Index(_index), m_Entries(SerializedArray<PALETTEENTRY>(_entries, _length)) {CreateTexture();}
    ~cPalette() override;
    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    short m_Index = -1;
    SerializedArray<PALETTEENTRY> m_Entries;
    LPDIRECT3DTEXTURE9 m_Texture;

    void CreateTexture();

public:
    void SetIndex(short _index) {m_Index = _index;}
    short GetIndex() {return m_Index;}
    void SetEntries(PALETTEENTRY* _entries, int _count) {m_Entries = SerializedArray<PALETTEENTRY>(_entries, _count);}
    int GetEntries(PALETTEENTRY*& _entries) {_entries = m_Entries.GetValue(); return m_Entries.GetLength();}
    LPDIRECT3DTEXTURE9 GetTexture() {return m_Texture;}

    void UpdateTexture();
    
    D3DCOLOR GetColor(int _index);
    void SetColor(int _index, BYTE _a, BYTE _r, BYTE _g, BYTE _b);
};
