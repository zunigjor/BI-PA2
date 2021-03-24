//
// Created by jorge on 6/20/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CUICOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CUICOMPONENT_HPP

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "EntityComponent.hpp"
#include "Components.hpp"
#include "../CAssetFactory.hpp"
#include "../CTextureManager.hpp"
#include "../CGame.hpp"

/**
 * UI component allows to draw text on the screen.
 */
class CUIComponent : public CComponent {
public:
    /**
     * Sets the text to draw.
     * @param x Position to draw the texture
     * @param y Position to draw the texture
     * @param text Text to display
     * @param font Font to use
     * @param color Color of the text
     */
    CUIComponent(int x, int y, std::string text, std::string font, SDL_Color & color):
    m_LabelText(text), m_LabelFont(font), m_LabelColor(color){
        m_Position.x = x;
        m_Position.y = y;
        setLabelText(text, font);
    }
    ~CUIComponent(){}
    /**
     * Sets the label text. Text can be changed by a stringstream.
     * @param text Text of the label
     * @param font Font to use
     */
    void setLabelText(std::string text, std::string font){
        SDL_Surface* surface = TTF_RenderText_Blended(CGame::m_AssetFactory->getFont(font), text.c_str(), m_LabelColor);
        m_LabelTexture = SDL_CreateTextureFromSurface(CGame::m_Renderer, surface);
        SDL_FreeSurface(surface);
        m_SourceRect.x = m_SourceRect.y = 0;
        SDL_QueryTexture(m_LabelTexture, nullptr, nullptr, &m_SourceRect.w, &m_SourceRect.h);
        SDL_QueryTexture(m_LabelTexture, nullptr, nullptr, &m_Position.w, &m_Position.h);
    }
    /// Displays the label on the screen
    void display(){
        SDL_RenderCopy(CGame::m_Renderer, m_LabelTexture, &m_SourceRect, &m_Position);

    }
    std::string getText(){return m_LabelText;}
private:
    SDL_Rect m_Position;
    std::string m_LabelText;
    std::string m_LabelFont;
    SDL_Color m_LabelColor;
    SDL_Texture * m_LabelTexture;
private:
    SDL_Rect m_SourceRect; ///< Used to prevent memory leaks.
};


#endif //RPG_GAME_ENGINE_PA2_CUICOMPONENT_HPP
