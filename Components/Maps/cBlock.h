#pragma once

class cBlock : public cComponent
{
public:
    cBlock(cObject* _owner) : cComponent(_owner) {}
    ~cBlock() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;

private:
    int m_Flag = 0;
    RECT m_Rect;

public:
    const RECT& GetRect() const {return m_Rect;}
};
