#ifndef ALIEN_H
#define ALIEN_H

#include <queue>

#include "InputManager.h"
#include "Component.h"

#define ALIEN_SPRITE            "assets/img/alien.png"
#define ALIEN_START_HP          100
#define ALIEN_SCALAR_SPEED      200.0f
#define ALIEN_PASSING_DISTANCE  10.0f
#define ALIEN_START_POSITION    512, 300
#define ALIEN_ROTATION_SPEED    20.0f
#define ALIEN_MINIONS_AMOUNT    6

class Alien: public Component {
    private:
        class Action {
            public:
                enum ActionType {MOVE, SHOOT};
                ActionType type;
                Vec2 pos;

                Action(ActionType type, float x, float y);
        };
        
        int hp;
        int nMinions;
        Vec2 speed;
        std::queue<Action> taskQueue;
        std::vector<std::weak_ptr<GameObject>> minionArray;
        
        Action ScheduleAction(InputManager* input, Action::ActionType type);

        // sylar's alien breath extra effects
        bool spriteIncreasing;

    public:
        Alien(GameObject& associated, int nMinions);
        ~Alien();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
};

#endif