//
// Created by jorge on 6/18/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CKEYBOARDCOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CKEYBOARDCOMPONENT_HPP

#include "../CGame.hpp"
#include "EntityComponent.hpp"
#include "Components.hpp"

#include "CSpriteComponent.hpp"
#include "CTransformationComponent.hpp"
#include "CHealthComponent.hpp"

/**
 * Keyboard controller component. Allows to move the entity using the wsad keys.
 */
class CKeyboardComponent : public CComponent {
public:
    /// Initializes the controller. Accesses the transformation component.
    void init() override{
        m_Transformation = &m_Entity->getComponent<CTransformationComponent>();
        m_Sprite = &m_Entity->getComponent<CSpriteComponent>();
        m_Health = &m_Entity->getComponent<CHealthComponent>();
    }
    /// Updates the entity accordingly to keyboard input.
    void update() override {
        if (!m_Health->alive()){
            m_Transformation->m_Velocity.setZero();
            m_Sprite->playAnimation("dead");
            if (CGame::m_Event.type == SDL_KEYUP){
                switch (CGame::m_Event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        CGame::m_Running = false;
                        break;
                    default:
                        break;
                }
            }
        } else {
            if (CGame::m_Event.type == SDL_KEYDOWN){
                switch (CGame::m_Event.key.keysym.sym) {
                    case SDLK_w:
                        m_Transformation->m_Velocity.m_y = -1.0f;
                        m_Sprite->playAnimation("walk_up");
                        break;
                    case SDLK_s:
                        m_Transformation->m_Velocity.m_y = 1.0f;
                        m_Sprite->playAnimation("walk_down");
                        break;
                    case SDLK_a:
                        m_Transformation->m_Velocity.m_x = -1.0f;
                        m_Sprite->playAnimation("walk_left");
                        break;
                    case SDLK_d:
                        m_Transformation->m_Velocity.m_x = 1.0f;
                        m_Sprite->playAnimation("walk_right");
                        break;
                    case SDLK_SPACE:
                        m_Sprite->playAnimation("attack");
                        m_Attacking = true;
                        break;
                    default:
                        break;
                }
            }
            if (CGame::m_Event.type == SDL_KEYUP){
                switch (CGame::m_Event.key.keysym.sym) {
                    case SDLK_w:
                        m_Transformation->m_Velocity.m_y = 0.0f;
                        m_Sprite->playAnimation("idle");
                        break;
                    case SDLK_s:
                        m_Transformation->m_Velocity.m_y = 0.0f;
                        m_Sprite->playAnimation("idle");
                        break;
                    case SDLK_a:
                        m_Transformation->m_Velocity.m_x = 0.0f;
                        m_Sprite->playAnimation("idle");
                        break;
                    case SDLK_d:
                        m_Transformation->m_Velocity.m_x = 0.0f;
                        m_Sprite->playAnimation("idle");
                        break;
                    case SDLK_SPACE:
                        m_Sprite->playAnimation("idle");
                        m_Attacking = false;
                        break;
                    case SDLK_ESCAPE:
                        CGame::m_Running = false;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    bool isAttacking(){return m_Attacking;}
private:
    CTransformationComponent * m_Transformation; ///< Pointer to transformation component.
    CSpriteComponent * m_Sprite; ///< Pointer to sprite component.
    CHealthComponent * m_Health; ///< Pointer to health component.
private:
    bool m_Attacking;
};


#endif //RPG_GAME_ENGINE_PA2_CKEYBOARDCOMPONENT_HPP
