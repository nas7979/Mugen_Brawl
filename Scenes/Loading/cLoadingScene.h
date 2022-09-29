#pragma once

class cLoadingScene : public cScene
{
public:
    void Init() override;
    void Update() override;
    void Render() override;
    void Release() override;

private:
    void LoadImages(std::string _path);
    void LoadSounds(std::string _path);
};
