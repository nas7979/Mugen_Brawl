#include "DXUT.h"
#include "cImageManager.h"

#include "../../Components/Characters/Data/cPalette.h"

void cImageManager::Init()
{
    D3DXCreateSprite(g_Device, &m_Sprite);

    LPD3DXFONT gothic;
    D3DXCreateFontA(g_Device, 64, 0, 0, 1, 0, DEFAULT_CHARSET, 0, 0, 0, "맑은 고딕", &gothic);
    m_Fonts["Gothic"] = gothic;

    LPD3DXBUFFER buffer;
    if (D3DXCreateEffectFromFile(g_Device, L"./Shaders/PaletteSwap.fx", nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_PaletteSwapShader, &buffer) != S_OK)
    {
        MessageBoxA(DXUTGetHWND(), (LPCSTR)buffer->GetBufferPointer(), "ERROR!", 0);
        buffer->Release();
    }
}

void cImageManager::Update()
{
}

void cImageManager::Render()
{
}

void cImageManager::Release()
{
    m_Sprite->Release();
    m_PaletteSwapShader->Release();
    for (auto& iter : m_Fonts)
    {
        iter.second->Release();
    }
    for(auto& iter : m_Textures)
    {
        iter.second->texture->Release();
        delete iter.second;
    }
    for (auto& iter : m_Animations)
    {
        delete iter.second;
    }
}

void cImageManager::Begin()
{
    m_Sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
    g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);
    g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DX_FILTER_NONE);
    g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DX_FILTER_NONE);
}

void cImageManager::End()
{
    m_Sprite->End();
}

void cImageManager::OnLostDevice()
{
    m_Sprite->OnLostDevice();
    m_PaletteSwapShader->OnLostDevice();
    for (auto& iter : m_Fonts)
        iter.second->OnLostDevice();
}

void cImageManager::OnResetDevice()
{
    m_Sprite->OnResetDevice();
    m_PaletteSwapShader->OnResetDevice();
    for (auto& iter : m_Fonts)
        iter.second->OnResetDevice();

    LPDIRECT3DSURFACE9 surface;
    D3DSURFACE_DESC desc;
    g_Device->GetRenderTarget(0, &surface);
    surface->GetDesc(&desc);
    m_ScreenScale = Vec2(desc.Width / 1920.f, desc.Height / 1080.f);
    surface->Release();
}

void cImageManager::AddImage(std::string _key, std::string _path)
{
    Texture* tex = new Texture;
    D3DXIMAGE_INFO info;
    if (D3DXCreateTextureFromFileExA(g_Device, _path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX10_FILTER_POINT, 0, &info, nullptr, &tex->texture) == S_OK)
    {
        m_Textures[_key] = tex;
        tex->width = info.Width;
        tex->height = info.Height;
        return;
    }
    delete tex;
    throw std::exception();
}

void cImageManager::AddAnimation(std::string _key)
{
    std::vector<Texture*>* vec = new std::vector<Texture*>;
    char key[64];
    for(int i = 0; true; i++)
    {
        sprintf_s(key, 64, "%s%d", _key.c_str(), i);
        auto find = m_Textures.find(key);
        if (find != m_Textures.end())
            vec->push_back((*find).second);
        else
            break;
    }

    m_Animations[_key] = vec;
}

Texture* cImageManager::GetTexture(std::string _key)
{
    return m_Textures[_key];
}

std::vector<Texture*>* cImageManager::GetAnimation(std::string _key)
{
    return m_Animations[_key];
}

LPD3DXFONT cImageManager::GetFont(std::string _key)
{
    return m_Fonts[_key];
}

void cImageManager::RenderSprite(Texture* _texture, Vec3 _pos, float _rot, Vec2 _scale, Vec2 _pivot, D3DCOLOR _color, bool _isView)
{
    Matrix mat;
    Vec2 center = Vec2(_texture->width * _pivot.x, _texture->height * _pivot.y);
    _pos.x *= m_ScreenScale.x;
    _pos.y *= m_ScreenScale.y;
    _scale.x *= m_ScreenScale.x;
    _scale.y *= m_ScreenScale.y;
    D3DXMatrixTransformation2D(&mat, &center, 0, &_scale, &center, _rot, &Vec2(_pos.x - center.x, _pos.y - center.y));
    if (_isView)
        mat *= CAMERA->GetInversedMatrix();
    mat._43 = _pos.z;
    m_Sprite->SetTransform(&mat);
    m_Sprite->Draw(_texture->texture, nullptr, nullptr, nullptr, _color);
}

void cImageManager::RenderText(std::string _font, std::string _text, Vec3 _pos, float _rot, Vec2 _scale, Vec2 _pivot, D3DCOLOR _color, bool _isView)
{
    RenderText(m_Fonts[_font], _text, _pos, _rot, _scale, _pivot, _color, _isView);
}

void cImageManager::RenderText(LPD3DXFONT _font, std::string _text, Vec3 _pos, float _rot, Vec2 _scale, Vec2 _pivot, D3DCOLOR _color, bool _isView)
{
    Matrix mat;
    RECT rect;
    _pos.x *= m_ScreenScale.x;
    _pos.y *= m_ScreenScale.y;
    _scale.x *= m_ScreenScale.x;
    _scale.y *= m_ScreenScale.y;
    _font->DrawTextA(m_Sprite, _text.c_str(), -1, &rect, DT_NOCLIP | DT_CALCRECT, 0);
    Vec2 center = Vec2((rect.right - rect.left) * _pivot.x, (rect.bottom - rect.top) * _pivot.y);
    D3DXMatrixTransformation2D(&mat, &center, 0, &_scale, &center, _rot, &(Vec2(_pos.x, _pos.y) - center));
    if (_isView)
        mat *= CAMERA->GetInversedMatrix();
    mat._43 = _pos.z;
    m_Sprite->SetTransform(&mat);
    _font->DrawTextA(m_Sprite, _text.c_str(), -1, nullptr, 0, _color);
}

int cImageManager::GetTextWidth(LPD3DXFONT _font, LPCSTR _text, int _offset, int _count)
{
    RECT rect;
    _font->DrawTextA(m_Sprite, _text + _offset, _count, &rect, DT_CALCRECT, 0);
    return rect.right - rect.left;
}

void cImageManager::BeginPaletteSwapShader()
{
    UINT passes = 0;
    IMAGE->End();
    m_PaletteSwapShader->Begin(&passes, 0);
    IMAGE->Begin();
    CAMERA->Render();
    m_PaletteSwapShader->BeginPass(0);
}

void cImageManager::SetPaletteSwapShaderParams(cPalette* _palette)
{
    m_PaletteSwapShader->SetTexture((D3DXHANDLE)"gPaletteTex", _palette->GetTexture());
    m_PaletteSwapShader->CommitChanges();
}

void cImageManager::EndPaletteSwapShader()
{
    m_PaletteSwapShader->EndPass();
    IMAGE->End();
    m_PaletteSwapShader->End();
    IMAGE->Begin();
    CAMERA->Render();
}
