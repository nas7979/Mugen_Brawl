#pragma once

class cPalette;

struct Texture
{
    LPDIRECT3DTEXTURE9 texture;
    UINT width = 0;
    UINT height = 0;
};

class cImageManager : public cSingleton<cImageManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();
    void Begin();
    void End();
    void OnLostDevice();
    void OnResetDevice();

private:
    LPD3DXSPRITE m_Sprite;
    std::map<std::string, Texture*> m_Textures;
    std::map<std::string, std::vector<Texture*>*> m_Animations;
    std::map<std::string, LPD3DXFONT> m_Fonts;
    Vec2 m_ScreenScale;

    LPD3DXEFFECT m_PaletteSwapShader = nullptr;

public:
    void AddImage(std::string _key, std::string _path);
    void AddAnimation(std::string _key);

    Texture* GetTexture(std::string _key);
    std::vector<Texture*>* GetAnimation(std::string _key);
    LPD3DXFONT GetFont(std::string _key);
    Vec2 GetScreenScale() {return m_ScreenScale;}

    void RenderSprite(Texture* _texture, Vec3 _pos, float _rot, Vec2 _scale, Vec2 _pivot = Vec2(0.5, 0.5), D3DCOLOR _color = 0xffffffff, bool _isView = false);
    void RenderText(std::string _font, std::string _text, Vec3 _pos, float _rot, Vec2 _scale, Vec2 _pivot = Vec2(0.5, 0.5), D3DCOLOR _color = 0xffffffff, bool _isView = false);
    void RenderText(LPD3DXFONT _font, std::string _text, Vec3 _pos, float _rot, Vec2 _scale, Vec2 _pivot = Vec2(0.5, 0.5), D3DCOLOR _color = 0xffffffff, bool _isView = false);
    int GetTextWidth(LPD3DXFONT _font, LPCSTR _text, int _offset, int _count);

    void BeginPaletteSwapShader();
    void SetPaletteSwapShaderParams(cPalette* _palette, int _alpha);
    void EndPaletteSwapShader();
};

#define IMAGE cImageManager::GetInstance()