//
// Created by jorge on 6/20/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CAICOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CAICOMPONENT_HPP

#include <math.h>

#include "EntityComponent.hpp"
#include "Components.hpp"

/**
 * A simple AI component which takes in a CVector2D with coordinates to follow.
 */
class CAIComponent : public CComponent {
public:
    /// Constructor takes in entity to follow.
    CAIComponent(CEntity * entity):m_FollowEntity(entity){
    }
    ~CAIComponent(){}
    /// Initializes the component
    void init() override {
        m_Transform = &m_Entity->getComponent<CTransformationComponent>();
        m_Health = &m_Entity->getComponent<CHealthComponent>();
    }
    /// Updates the entities velocity according to its position relative to the point to follow.
    void update() override {
        if (m_Health->alive() && distance() < 400.0f){
            if (m_Transform->m_Position.m_x > m_FollowEntity->getComponent<CTransformationComponent>().m_Position.m_x){
                m_Transform->m_Velocity.m_x = -1.0f;
            }
            if (m_Transform->m_Position.m_x < m_FollowEntity->getComponent<CTransformationComponent>().m_Position.m_x){
                m_Transform->m_Velocity.m_x = 1.0f;
            }
            if (m_Transform->m_Position.m_y > m_FollowEntity->getComponent<CTransformationComponent>().m_Position.m_y){
                m_Transform->m_Velocity.m_y = -1.0f;
            }
            if (m_Transform->m_Position.m_y < m_FollowEntity->getComponent<CTransformationComponent>().m_Position.m_y){
                m_Transform->m_Velocity.m_y = 1.0f;
            }
        } else {
            m_Transform->m_Velocity.setZero();
        }
        m_Transform->m_Speed = 1;
    }
private:
    /// Returns the distance between the entity and the followed entity
    float distance(){
        // Good ol' distance formula sqrt( (x2 - x1)^2  + (y2 - y1)^2 )
        float distance = sqrt(
                pow(m_Transform->m_Position.m_x - m_FollowEntity->getComponent<CTransformationComponent>().m_Position.m_x, 2)
                + pow(m_Transform->m_Position.m_y - m_FollowEntity->getComponent<CTransformationComponent>().m_Position.m_y, 2));
        return distance;
    }
    CTransformationComponent * m_Transform;
    CHealthComponent * m_Health;
    CEntity * m_FollowEntity; ///< Entity to follow.
};

#endif //RPG_GAME_ENGINE_PA2_CAICOMPONENT_HPP
