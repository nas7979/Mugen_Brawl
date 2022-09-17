#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "dsound.lib")

#include <math.h>
#include <time.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <functional>
#include <dsound.h>
#include "GGPO/include/ggponet.h"

#define g_Device DXUTGetD3D9Device()
#define CURRENT_VERSION 2
#define MAX_PLAYER 8

using Vec2 = D3DXVECTOR2;
using Vec3 = D3DXVECTOR3;
using Matrix = D3DXMATRIX;

constexpr float SCREEN_WIDTH = 1920;
constexpr float SCREEN_HEIGHT = 1080;

#include "Utility/Math.h"
using namespace Math;
#include "Utility/Utility.h"
using namespace Utility;
#include "Utility/Serializer.h"
#include "Utility/SerializedString.h"
#include "Utility/Serialized.h"
#include "Components/Characters/Data/cEventKey.h"

#include "Components/cComponent.h"
#include "Managers/cSingleton.h"
#include "Managers/Scene/cSceneManager.h"
#include "Managers/Image/cImageManager.h"
#include "Managers/Object/cObjectManager.h"
#include "Managers/Camera/cCameraManager.h"
#include "Managers/Input/cInputManager.h"
#include "Managers/Sound/cSoundManager.h"
#include "Managers/Game/cGameManager.h"

#include "Components/Renderer/cRenderer.h"
#include "Components/Characters/cCharacter.h"
#include "Components/Characters/HitBoxCollider/cHitBoxCollider.h"
#include "Components/Characters/CharacterAnimationPlayer/cCharacterAnimationPlayer.h"

#include "Scenes/Editor/cEditorScene.h"
#define GET_CURRENT_DATA_VERSION SCENE->GetScene<cEditorScene>()->GetCharacterData()->GetVersion()