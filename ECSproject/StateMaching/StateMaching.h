#pragma once

#include "../ECS/AIContext.h"
#include "AIStates.h"

class AIStateFactory {
public:
    static AIState* createState(EnemyState stateName) {
        if (stateName == EnemyState::patrol) {
            return new PatrolState();
        }
        else if (stateName == EnemyState::fighting) {
            return new FightingState();
        }
        else if (stateName == EnemyState::tracking) {
            return new TrackingState();
        }
        else if (stateName == EnemyState::backing) {
            return new BackingState();
        }
        return nullptr;
    }
};

class AIStateMachine {
private:
    EnemyState _currentStateName = EnemyState::patrol;
    AIState* _currentState = AIStateFactory::createState(_currentStateName);

public:
    ~AIStateMachine() { delete _currentState; }

    void changeState(AIState* newState) {
        if (_currentState) {
            _currentState->exit();
            delete _currentState;
        }
        _currentState = newState;
        if (_currentState) {
            _currentState->enter();
        }
    }

    int to_int(EnemyState state) {
        switch (state) {
        case EnemyState::patrol: return 0;
        case EnemyState::tracking: return 1;
        case EnemyState::fighting: return 2;
        case EnemyState::backing: return 3;
        default: return -1;
        }
    }

    void update(AIContext& ai) {
        if (_currentState) {
            _currentState->update(ai);
        }
        // std::cout << to_int(_currentStateName) << std::endl;
        // 检查是否需要切换状态
        EnemyState nextState = _currentState->getNextState(ai);
        if (nextState != _currentStateName) {
            _currentStateName = nextState;
            AIState* newState = AIStateFactory::createState(nextState);
            changeState(newState);
        }
    }

    
};