#include "DXUT.h"
#include "cIngameScene.h"

cIngameScene::~cIngameScene()
{
}

void cIngameScene::Init()
{
    for (int i = 0; i < MAX_PLAYER; i++)
        m_Characters[i] = nullptr;
}

void cIngameScene::Update()
{
}

void cIngameScene::Render()
{
}

void cIngameScene::Release()
{
}
