//
// Created by jorge on 6/18/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CCOLLISIONCOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CCOLLISIONCOMPONENT_HPP

#include <string>
#include <SDL2/SDL.h>
#include "EntityComponent.hpp"
#include "Components.hpp"
#include "../CTextureManager.hpp"

/**
 * Collision component allows the object to detect collisions with other collision enabled objects.
 */
class CCollisionComponent : public CComponent {
public:
    CCollisionComponent(const char * tag){
        m_CollisionTag = tag;
    }
    CCollisionComponent(std::string tag, int x, int y, int size){
        m_CollisionTag = tag;
        m_Collider.x = x;
        m_Collider.y = y;
        m_Collider.h = m_Collider.w = size;
    }
    /// Initializes the component. Makes sure other required components are present. Adds them otherwise.
    void init() override {
        if (!m_Entity->hasComponent<CTransformationComponent>()){
            m_Entity->addComponent<CTransformationComponent>();
        }
        m_Transform = &m_Entity->getComponent<CTransformationComponent>();
        if (m_CollisionTag != "terrain") {
            m_Collider.x = m_Transform->m_Position.m_x;
            m_Collider.y = m_Transform->m_Position.m_y;
            m_Collider.w = m_Transform->m_Width;
            m_Collider.h = m_Transform->m_Height;
        }
        m_Texture = CGame::m_AssetFactory->getTexture("solid");
        m_SourceRect = {0,0, TEXTURE_RESOLUTION, TEXTURE_RESOLUTION};
        m_DestRect = {m_Collider.x, m_Collider.y, m_Collider.w, m_Collider.h};
    }
    /// Updates the component. If the component tag isn't "terrain" it will track its movement.
    void update() override {
        if (m_CollisionTag != "terrain"){
            m_Collider.x = static_cast<int>(m_Transform->m_Position.m_x);
            m_Collider.y = static_cast<int>(m_Transform->m_Position.m_y);
            m_Collider.w = m_Transform->m_Width * m_Transform->m_Scale;
            m_Collider.h = m_Transform->m_Height * m_Transform->m_Scale;
        }
        m_DestRect.x = m_Collider.x - CGame::m_Camera.x;
        m_DestRect.y = m_Collider.y - CGame::m_Camera.y;
    }
    /// Draws the collision detection square only if its terrain.
    void draw() override {
        if (m_CollisionTag == "terrain"){
            CTextureManager::draw(m_Texture, m_SourceRect, m_DestRect);
        }
    }
public:
    SDL_Rect m_Collider; ///< SDL_Rectangle used as a bounding box.
    std::string m_CollisionTag; ///< Tag of the collision allows to identify what type of collision has happened.
private:
    CTransformationComponent * m_Transform; ///< Pointer to transformation component.
    SDL_Texture * m_Texture; ///< SDL_Texture to display solid objects.
    SDL_Rect m_SourceRect;
    SDL_Rect m_DestRect;
};

#endif //RPG_GAME_ENGINE_PA2_CCOLLISIONCOMPONENT_HPP
