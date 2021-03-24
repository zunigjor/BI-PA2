//
// Created by jorge on 6/14/20.
//

#ifndef RPG_GAME_ENGINE_CFPSCAPPER_HPP
#define RPG_GAME_ENGINE_CFPSCAPPER_HPP

#include <SDL2/SDL.h>
#include "../CGame.hpp"

/**
 * Frame rate capping class. Caps to 60 fps.
 */
class CFpsCapper {
public:
    static CFpsCapper & get();
    ///Saves current ticks.
    void saveTicks();
    /// Uses SDL_Delay to delay by the necessary amount of time.
    void delay();
private:
    /// Default fps capper constructor.
    CFpsCapper();
    /// Default fps capper destructor.
    ~CFpsCapper() = default;
private:
    const Uint32 m_FPS; ///< Game fps.
    Uint32 m_FrameDelay; ///< Frame delay
    Uint32 m_FrameStart; ///< Frame start stamp
    Uint32 m_FrameTime; ///< Frame time stamp.
};


#endif //RPG_GAME_ENGINE_CFPSCAPPER_HPP
