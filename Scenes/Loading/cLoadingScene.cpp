#include "DXUT.h"
#include "cLoadingScene.h"
#include <filesystem>

void cLoadingScene::Init()
{
    LoadImages("");

    SCENE->ChangeScene("EDITOR");
}

void cLoadingScene::Update()
{
}

void cLoadingScene::Render()
{
}

void cLoadingScene::Release()
{
}

void cLoadingScene::LoadImages(std::string _path)
{
    std::string key;
    std::string path;
    std::vector<std::string> animations;
    for(auto& iter : std::experimental::filesystem::recursive_directory_iterator("./Resources/Images/" + _path))
    {
        if (iter.status().type() == std::experimental::filesystem::file_type::directory)
            continue;
        
        key = iter.path().filename().string();
        for(int i = 0; i < 4; i++)
            key.pop_back();
        IMAGE->AddImage(key, iter.path().string());
        if (key.back() == '1')
        {
            key.pop_back();
            if (key.back() < '0' && key.back() > '9')
                animations.push_back(key);
        }
    }

    for(const auto& iter : animations)
    {
        IMAGE->AddAnimation(iter);
    }
}

void cLoadingScene::LoadSounds(std::string _path)
{
}
