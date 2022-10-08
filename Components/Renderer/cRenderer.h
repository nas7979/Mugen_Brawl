#pragma once

class cRenderer : public cComponent
{
public:
    cRenderer(cObject* _owner) : cComponent(_owner) {}
    ~cRenderer() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

private:
    Texture* m_Texture = nullptr;
    Vec2 m_Offset = Vec2(0, 0);
    Vec2 m_Pivot = Vec2(0.5f, 0.5f);
    D3DCOLOR m_Color = 0xffffffff;
    std::function<void()> m_ShaderBeginFunc = nullptr;
    std::function<void()> m_ShaderEndFunc = nullptr;

public:
    void SetTexture(Texture* _texture) {m_Texture = _texture;}
    void SetOffset(Vec2 _offset) {m_Offset = _offset;}
    Vec2 GetOffset() const {return m_Offset;}
    void SetPivot(Vec2 _pivot) {m_Pivot = _pivot;}
    void SetColor(D3DCOLOR _color) {m_Color = _color;}
    void SetShaderBeginFunc(const std::function<void()>& _func) {m_ShaderBeginFunc = _func;}
    void SetShaderEndFunc(const std::function<void()>& _func) {m_ShaderEndFunc = _func;}
};
