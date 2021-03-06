#include "State.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "Resources.h"
#include "TileMap.h"
#include "Sound.h"
#include "Vec2.h"
#include "Alien.h"

State::State () {
    GameObject* bg = new GameObject();
    CameraFollower* bgCamera = new CameraFollower(*bg);
    Sprite* bgSprite = new Sprite(*bg, BG_SPRITE);
    bg->AddComponent(bgCamera);
    bg->AddComponent(bgSprite);
    objectArray.emplace_back(bg);

    music.Open(BG_MUSIC);
    music.Play(MUSIC_REPEAT_ON);

    quitRequested = false;
    started = false;
}

State::~State () {
    objectArray.clear();
}

void State::Start () {
    LoadAssets();
    for (int i=0; i < (int)objectArray.size(); i++) {
        objectArray[i]->Start();
    }
    started = true;
}

void State::LoadAssets () {
    GameObject* gameMap = new GameObject();
    TileSet* gameMapTset = new TileSet(*gameMap, GAMEMAP_TILESET, GAMEMAP_TILE_DIM);
    TileMap* gameMapTmap = new TileMap(*gameMap, gameMapTset, GAMEMAP_TILEMAP);
    gameMap->box = Rect(GAMEMAP_START_POSITION, GAMEMAP_TILE_DIM);
    gameMap->AddComponent(gameMapTmap);
    objectArray.emplace_back(gameMap);

    GameObject* alien = new GameObject();
    alien->AddComponent(new Alien(*alien, ALIEN_MINIONS_AMOUNT));
    alien->box.SetPosition(ALIEN_START_POSITION);
    objectArray.emplace_back(alien);
}

void State::ClearResources () {
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}

void State::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (input.QuitRequested() or input.IsKeyDown(KEY_ESCAPE)) {
        quitRequested = true;
    } else {
        
        Camera::Update(dt);
    
        for (unsigned i=0; i < objectArray.size(); i++) {
            objectArray[i]->Update(dt);
        }

        for (int i=(int)objectArray.size()-1; i >= 0; i--) {
            if (objectArray[i]->IsDead()) {
                objectArray.erase(objectArray.begin()+i);
            }
        }
    }
}

void State::Render () {
    for (unsigned i=0; i < objectArray.size(); i++) {
        objectArray[i]->Render();
    }
}

bool State::QuitRequested () {
    return quitRequested;
}

std::weak_ptr<GameObject> State::AddObject (GameObject* go) {
    std::shared_ptr<GameObject> sptrGo(go);
    std::weak_ptr<GameObject> wptrGo(sptrGo);

    objectArray.push_back(sptrGo);
    if (started) {
        go->Start();
    }

    return wptrGo;
}

std::weak_ptr<GameObject> State::GetObjectPtr (GameObject* go) {
    std::weak_ptr<GameObject> wptrGo;

    for (int i=0; i < (int)objectArray.size(); i++) {
        if (objectArray[i].get() == go) {
            wptrGo = objectArray[i];
        }
    }
    return wptrGo;
}