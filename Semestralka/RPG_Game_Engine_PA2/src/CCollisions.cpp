//
// Created by jorge on 6/18/20.
//

#include "CCollisions.hpp"
#include "EntityComponents/Components.hpp"

bool CCollisions::AABB(const SDL_Rect &rectA, const SDL_Rect &rectB) {
    return (rectA.x + rectA.w >= rectB.x &&
            rectB.x + rectB.w >= rectA.x &&
            rectA.y + rectA.h >= rectB.y &&
            rectB.y + rectB.h >= rectA.y);
}

bool CCollisions::AABB(const CCollisionComponent &colA, const CCollisionComponent &colB) {
    if (AABB(colA.m_Collider, colB.m_Collider)){
        //std::cout << colA.m_CollisionTag << " hit: " << colB.m_CollisionTag << std::endl;
        return true;
    }
    return false;
}
