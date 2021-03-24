//
// Created by jorge on 6/18/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CCOLLISIONS_HPP
#define RPG_GAME_ENGINE_PA2_CCOLLISIONS_HPP

#include <SDL2/SDL.h>

class CCollisionComponent;

/**
 * Class provides interface to check collisions of objects.
 */
class CCollisions {
public:
    /**
     * Axis aligned bounding box collision check.
     * @see https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
     * @return True when the rectangles of two obejcts collide. False otherwise.
     */
    static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
    /**
     * @overload Overloaded for collision components.
     * @param colA First collision component.
     * @param colB Second collision component.
     * @return True when they collide, false otherwise.
     */
    static bool AABB(const CCollisionComponent& colA, const CCollisionComponent& colB);
};


#endif //RPG_GAME_ENGINE_PA2_CCOLLISIONS_HPP
