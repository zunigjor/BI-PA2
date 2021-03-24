//
// Created by jorge on 6/18/20.
//

#ifndef RPG_GAME_ENGINE_POSITIONCOMPONENT_H
#define RPG_GAME_ENGINE_POSITIONCOMPONENT_H

#include "Components.hpp"
#include "../RPGconfig.hpp"
#include "../CVector2D.hpp"

/**
 * Allow the object to transform its position.
 */
class CTransformationComponent : public CComponent{
public:
    CTransformationComponent(){
        m_Position.setZero();
    }
    CTransformationComponent(float x, float y){
        m_Position.m_x = x;
        m_Position.m_y = y;
    }
    CTransformationComponent(float x, float y, int scale){
        m_Position.m_x = x;
        m_Position.m_y = y;
        m_Scale = scale;
    }
    CTransformationComponent(float x, float y, int scale, int w, int h){
        m_Position.m_x = x;
        m_Position.m_y = y;
        m_Scale = scale;
        m_Width = w;
        m_Height = h;
    }
    CTransformationComponent(float x, float y, int scale, int w, int h, int speed){
        m_Position.m_x = x;
        m_Position.m_y = y;
        m_Scale = scale;
        m_Width = w;
        m_Height = h;
        m_Speed = speed;
    }
    void init()override {
        m_Velocity.setZero();
    }
    /// Updates the position of the entity by the velocity * speed.
    void update() override {
        m_Position.m_x += static_cast<int>(m_Velocity.m_x * m_Speed);
        m_Position.m_y += static_cast<int>(m_Velocity.m_y * m_Speed);
    }
public:
    CVector2D m_Position;
    CVector2D m_Velocity;
    int m_Height = TEXTURE_RESOLUTION;
    int m_Width = TEXTURE_RESOLUTION;
    int m_Scale = TEXTURE_SCALE;
    int m_Speed = 3;
};


#endif //RPG_GAME_ENGINE_POSITIONCOMPONENT_H
