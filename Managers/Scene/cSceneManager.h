#pragma once
#include "cScene.h"

class cSceneManager : public cSingleton<cSceneManager>
{
public:
    void Init();
    void Update();
    void Render();
    void Release();

private:
    std::map<std::string, cScene*> m_SceneMap;
    cScene* m_CurScene = nullptr;
    cScene* m_NewScene = nullptr;
    std::string m_CurSceneKey;
    std::string m_TempCurSceneKey;

public:
    void AddScene(std::string _key, cScene* _scene);
    void ChangeScene(std::string _key);
    bool CheckCurrentScene(std::string _key);

    template<typename T>
    T* GetScene()
    {
        return static_cast<T*>(m_CurScene);
    }
};

#define SCENE cSceneManager::GetInstance()