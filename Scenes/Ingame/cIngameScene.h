#pragma once
#include "../../Components/Characters/cCharacter.h"

class cIngameScene : public cScene
{
public:
    ~cIngameScene() override;
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;

private:
    cCharacter* m_Characters[MAX_PLAYER];
};
