//
// Created by jorge on 6/18/20.
//

#ifndef RPG_GAME_ENGINE_SPRITECOMPONENT_H
#define RPG_GAME_ENGINE_SPRITECOMPONENT_H

#include <map>
#include <SDL2/SDL.h>
#include "Components.hpp"
#include "../CTextureManager.hpp"
#include "../CAssetFactory.hpp"
#include "../CGame.hpp"

/**
 * Sprite component class allows the entity to be drawn on the screen.
 */
class CSpriteComponent : public CComponent {
public:
    /// By default there is no texture.
    CSpriteComponent() = default;
    /// Loads a texture from the path.
    CSpriteComponent(const std::string & texName):m_TexName(texName){
        setTexture(texName);
    }
    /// Loads a sprite with animations enabled. Requires a sprite sheet in the same format as the players.
    CSpriteComponent(const std::string & texName, bool animated){
        m_Animated = animated;
        SAnimation idle = SAnimation(0,2,200);
        SAnimation walk_right = SAnimation(1 , 2, 200);
        SAnimation walk_left = SAnimation(2 , 2, 200);
        SAnimation walk_down = SAnimation(3 , 2, 200);
        SAnimation walk_up = SAnimation(4 , 2, 200);
        SAnimation dead = SAnimation(5 , 1, 200);
        SAnimation attack = SAnimation(6,2,200);
        m_Animations.emplace("idle",idle);
        m_Animations.emplace("walk_right",walk_right);
        m_Animations.emplace("walk_left",walk_left);
        m_Animations.emplace("walk_down",walk_down);
        m_Animations.emplace("walk_up",walk_up);
        m_Animations.emplace("dead",dead);
        m_Animations.emplace("attack",attack);
        playAnimation("idle");
        setTexture(texName);
    }
    ~CSpriteComponent(){
    }
    /// Initializes the sprite components members.
    void init() override {
        if (!m_Entity->hasComponent<CTransformationComponent>()){
            m_Entity->addComponent<CTransformationComponent>();
        }
        m_Transform = &m_Entity->getComponent<CTransformationComponent>();

        m_SrcRect.x = m_SrcRect.y = 0;
        m_SrcRect.w = m_Transform->m_Width;
        m_SrcRect.h = m_Transform->m_Height;
    }
    /// Updates the rectangles.
    void update() override {

        if (m_Animated){
            m_SrcRect.x = m_SrcRect.w * static_cast<int>((SDL_GetTicks() / m_AnimationSpeed ) % m_AnimationFrames); // loop through the frames
        }
        m_SrcRect.y = m_AnimationIndex * m_Transform->m_Height;

        m_DestRect.x = static_cast<int>(m_Transform->m_Position.m_x - static_cast<float>(CGame::m_Camera.x));
        m_DestRect.y = static_cast<int>(m_Transform->m_Position.m_y - static_cast<float>(CGame::m_Camera.y));
        m_DestRect.w = m_Transform->m_Width * m_Transform->m_Scale;
        m_DestRect.h = m_Transform->m_Height * m_Transform->m_Scale;
    }
    /// Draws the sprite using CTextureManager
    void draw() override {
        CTextureManager::draw(m_Texture,m_SrcRect, m_DestRect, m_SpriteFlip);
    }
public:
    /// Allows to change the texture when needed.
    void setTexture(const std::string & texName){
        m_Texture = CGame::m_AssetFactory->getTexture(texName);
    }
    /// Sets the animation index, frame and speed to play an animation.
    void playAnimation(const char * animName){
        m_AnimationIndex  = m_Animations[animName].index;
        m_AnimationFrames = m_Animations[animName].frames;
        m_AnimationSpeed  = m_Animations[animName].speed;
    }
    /// returns the name of the texture
    std::string getName(){return m_TexName;}
public:
    SDL_RendererFlip m_SpriteFlip = SDL_FLIP_NONE; ///< Allows to render flipped textures.
private:
    std::string m_TexName; ///< Holds the name of the texture.
    CTransformationComponent * m_Transform; ///< Ptr to transformation component.
    SDL_Texture * m_Texture; ///< Texture.
    SDL_Rect m_SrcRect; ///< Source rectangle.
    SDL_Rect m_DestRect; ///< Destination rectangle.
    bool m_Animated = false; ///< States whether the sprite is animated. False by default.
private:
    /**
     * Animation structure holding information about the sprite animation.
     */
    struct SAnimation {
        int index;
        int frames;
        int speed;
        SAnimation(){}
        SAnimation(int idx, int frm, int spd){
            index = idx;
            frames = frm;
            speed = spd;
        }
    };
    int m_AnimationFrames = 0;
    int m_AnimationSpeed = 100;
    int m_AnimationIndex = 0; ///< Index of the animation. Basically just a the row of the sprite sheet.
    std::map<const char *, SAnimation> m_Animations;
};


#endif //RPG_GAME_ENGINE_SPRITECOMPONENT_H
