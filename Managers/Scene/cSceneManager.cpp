#include "DXUT.h"
#include "cSceneManager.h"

void cSceneManager::Init()
{
}

void cSceneManager::Update()
{
    if (m_NewScene != nullptr)
    {
        if (m_CurScene)
            m_CurScene->Release();
        m_CurScene = m_NewScene;
        m_NewScene = nullptr;
        m_CurScene->Init();
        m_CurSceneKey = m_TempCurSceneKey;
    }

    if (m_CurScene != nullptr)
    {
        m_CurScene->Update();
    }
}

void cSceneManager::Render()
{
    if (m_CurScene != nullptr)
    {
        m_CurScene->Render();
    }
}

void cSceneManager::Release()
{
    for (auto& iter : m_SceneMap)
    {
        iter.second->Release();
        delete iter.second;
    }
}

void cSceneManager::AddScene(std::string _key, cScene* _scene)
{
    m_SceneMap[_key] = _scene;
}

void cSceneManager::ChangeScene(std::string _key)
{
    m_NewScene = m_SceneMap[_key];
    m_TempCurSceneKey = _key;
}

bool cSceneManager::CheckCurrentScene(std::string _key)
{
    return _key == m_CurSceneKey;
}
