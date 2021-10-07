#include "Camera.h"
#include "InputManager.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos, Camera::speed;

void Camera::Follow (GameObject* newFocus) {
    focus = newFocus;
}

void Camera::Unfollow () {
    focus = nullptr;
}

void Camera::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (focus) {
        pos = focus->box.GetCenter();
    } else {

        if (input.IsKeyDown(KEY_ARROW_UP)) {
            speed.y = -1;
        }
        if (input.IsKeyDown(KEY_ARROW_DOWN)) {
            speed.y = 1;
        }
        if (input.IsKeyDown(KEY_ARROW_LEFT)) {
            speed.x = -1;
        }
        if (input.IsKeyDown(KEY_ARROW_RIGHT)) {
            speed.x = 1;
        }
        if (speed.x and speed.y) {
            speed.Normalize();
        }
        pos += speed * CAM_SPEED_DEF * dt;
    }

    speed.x = 0;
    speed.y = 0;
}