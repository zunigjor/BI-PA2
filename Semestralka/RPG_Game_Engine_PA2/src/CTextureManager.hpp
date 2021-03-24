//
// Created by jorge on 6/11/20.
//

#ifndef RPG_GAME_ENGINE_CTEXTUREMANAGER_HPP
#define RPG_GAME_ENGINE_CTEXTUREMANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
/**
 * Texture manager class takes care of loading and displaying textures.
 * Static methods can be accessed without the need to instantiate the class.
 */
class CTextureManager {
public:
    /**
     * Loads a texture.
     * @param fileName Name of the texture file
     * @return Pointer to SDL_Texture
     */
    static SDL_Texture* loadTexture(const char* fileName);
    /**
     * Draws a texture.
     * @param tex Pointer to SDL_Texture
     * @param src Source SDL_Rectangle
     * @param dest Destination SDL_Rectangle
     */
    static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
    /**
     * @overload Overloaded draw method allows to draw with a flip.
     * @param tex SDL texture to draw
     * @param src SDL source rectangle
     * @param dest SDL destination rectangle
     * @param flip SDL flip option
     */
    static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    /**
     * Draws to the full window.
     * @param tex Texture to draw.
     */
    static void drawFullWindow(SDL_Texture * tex);
};


#endif //RPG_GAME_ENGINE_CTEXTUREMANAGER_HPP
