#pragma once
#include "../Data/cCharacterData.h"

class cCharacterEffect : public cComponent, cCharacterEventHandler, Serializer
{
public:
    cCharacterEffect(cObject* _owner) : cComponent(_owner) {}
    ~cCharacterEffect() override = default;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;
    void OnCollision(cObject* _other) override;
    void OnAlarm(std::string _key) override;
    void OnAnimationEnd(cCharacterAnimation* _anim) override;
    void OnHurt(cCharacter* _by, cHurtBox* _myHurtBox, cHitBox* _enemyHitBox, RECT _overlappedRect) override;
    void OnHit(cCharacter* _to, cHurtBox* _enemyHurtBox, cHitBox* _myHitBox, RECT _overlappedRect) override;
    void OnThrown(cCharacter* _by, cBodyBox* _myBodyBox, cThrowBox* _enemyThrowBox, RECT _overlappedRect) override;
    void OnThrow(cCharacter* _to, cThrowBox* _myThrowBox, cBodyBox* _enemyBodyBox, RECT _overlappedRect) override;
    void OnCollisionWithCharacter(cCharacter* _with, const RECT& _bodyRect, const RECT& _overlapped) override;
    void OnCollisionWithMap(cBlock* _with, const RECT& _bodyRect, const RECT& _overlapped) override;
    void OnSpriteChanged(cCharacterSprite* _sprite) override;
    void HandleAnimationEvent(const std::string& _key, const std::string& _value) override;
    void HandleSpriteEvent(const std::string& _key, const std::string& _value) override;
    void HandleHurtBoxEvent(const std::string& _key, const std::string& _value) override;
    void HandleHitBoxEvent(const std::string& _key, const std::string& _value) override;
    void HandleThrowBoxEvent(const std::string& _key, const std::string& _value) override;
    void HandleBodyBoxEvent(const std::string& _key, const std::string& _value) override;

    void SetHitStop(int _hitStop) {m_HitStop = _hitStop;}
    int GetHitStop() const override {return m_HitStop;}
    void SetFrom(cCharacter* _from) {m_From = _from;}
    void SetAnimation(cCharacterData* _charData, cCharacterAnimation* _anim);

    void Serialize(char* _buffer, UINT& _pointer) const override;
    void Deserialize(char* _buffer, UINT& _pointer) override;
    size_t GetSize() const override;

private:
    cCharacter* m_From;
    short m_HitStop;
};
