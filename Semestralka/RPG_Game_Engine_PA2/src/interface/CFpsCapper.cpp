//
// Created by jorge on 6/14/20.
//

#include "CFpsCapper.hpp"

CFpsCapper &CFpsCapper::get() {
    static CFpsCapper fpsCapper;
    return fpsCapper;
}

CFpsCapper::CFpsCapper():m_FPS(FRAMES_PER_SECOND),m_FrameDelay(1000/m_FPS),m_FrameStart(0),m_FrameTime(0){}

void CFpsCapper::saveTicks() {
    m_FrameStart = SDL_GetTicks();
}

void CFpsCapper::delay() {
    m_FrameTime = SDL_GetTicks() - m_FrameStart;
    if (m_FrameDelay > m_FrameTime){
        SDL_Delay(m_FrameDelay - m_FrameTime);
    }
}


