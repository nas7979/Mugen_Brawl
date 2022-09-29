#include "DXUT.h"
#include "cMainGame.h"

#include "../Managers/Image/cImageManager.h"
#include "../Scenes/Editor/cEditorScene.h"
#include "../Scenes/Loading/cLoadingScene.h"

void cMainGame::Init()
{
    SCENE->CreateInstance();
    IMAGE->CreateInstance();
    OBJECT->CreateInstance();
    CAMERA->CreateInstance();
    INPUT->CreateInstance();
    SOUND->CreateInstance();
    GAME->CreateInstance();
    GGPO->CreateInstance();
    
    SCENE->AddScene("LOADING", new cLoadingScene());
    SCENE->AddScene("EDITOR", new cEditorScene());

    SCENE->ChangeScene("LOADING");

}

void cMainGame::Update()
{
    GGPO->Update();
    INPUT->Update();
    SCENE->Update();
    OBJECT->Update();
    CAMERA->Update();
    GAME->Update();
    SOUND->Update();
}

void cMainGame::Render()
{
    IMAGE->Begin();

    CAMERA->Render();
    SCENE->Render();
    OBJECT->Render();
    INPUT->Render();
    
    IMAGE->End();
}

void cMainGame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (INPUT == nullptr)
        return;
    
    INPUT->HandleWindowInput(uMsg, wParam, lParam);
}

void cMainGame::Release()
{
    SCENE->ReleaseInstance();
    OBJECT->ReleaseInstance();
    CAMERA->ReleaseInstance();
    INPUT->ReleaseInstance();
    SOUND->ReleaseInstance();
    IMAGE->ReleaseInstance();
    GAME->ReleaseInstance();
    GGPO->ReleaseInstance();
}

void cMainGame::OnLostDevice()
{
    IMAGE->OnLostDevice();
}

void cMainGame::OnResetDevice()
{
    IMAGE->OnResetDevice();
}

