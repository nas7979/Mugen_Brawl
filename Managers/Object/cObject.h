#pragma once

class cComponent;

enum ObjectTag : short
{
    Obj_Character,
    Obj_Map,
    Obj_Missile,
    Obj_Effect,
    Obj_GUI,
    Obj_None,
    Obj_End
};

class cObject : public Serializer
{
struct Alarm
{
    std::string key;
    int frameLeft;
};
    
public:
    cObject(ObjectTag _tag)
    {
        m_Tag = _tag;
        m_UID = staticUID++;
    }
    
    void Init();
    void Update();
    void Render();
    void Release();

    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;
private:
    static int staticUID;
    int m_UID;
    
    Vec3 m_Pos = Vec3(0,0,0);
    Vec2 m_Scale = Vec2(1,1);
    float m_Rot = 0;
    bool m_IsActive = true;
    bool m_IsDestroyed = false;
    ObjectTag m_Tag = Obj_None;
    int m_CreatedAt;
    SerializedString m_Name;
    std::vector<cComponent*> m_Components;
    std::vector<Alarm> m_Alarms;

public:
    int GetUID() {return m_UID;}
    void SetPos(Vec3 _pos) {m_Pos = _pos;}
    Vec3 GetPos() {return m_Pos;}
    void SetScale(Vec2 _scale) {m_Scale = _scale;}
    Vec2 GetScale() {return m_Scale;}
    void SetRot(float _rot) {m_Rot = _rot;}
    float GetRot() {return m_Rot;}
    void SetActive(bool _isActive) {m_IsActive = _isActive;}
    bool IsActive() {return m_IsActive;}
    void Destroy() {m_IsDestroyed = true;}
    bool IsDestroyed() {return m_IsDestroyed;}
    void SetCreatedAt(int _frame) {m_CreatedAt = _frame;}
    int GetCreatedAt() const {return m_CreatedAt;}
    void SetName(std::string _name) {m_Name = _name;}
    std::string GetName() {return m_Name;}
    ObjectTag GetTag() {return m_Tag;}

    void AddAlarm(std::string _key, int _frame);
    void ClearAlarm() {m_Alarms.clear();}

    template<typename T>
    T* AddComponent()
    {
        T* component = new T(this);
        m_Components.push_back(component);
        component->Init();
        return component;
    }

    template<typename T>
    T* GetComponent()
    {
        for(auto& iter : m_Components)
        {
            if (typeid(T*) == typeid(iter))
                return static_cast<T*>(iter);
        }

        T* casted;
        for(auto& iter : m_Components)
        {
            casted = dynamic_cast<T*>(iter);
            if (casted != nullptr)
                return casted;
        }
        return nullptr;
    }
};
