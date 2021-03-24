//
// Created by jorge on 6/15/20.
//

#ifndef RPG_GAME_ENGINE_CMAP_HPP
#define RPG_GAME_ENGINE_CMAP_HPP

#include "CTextureManager.hpp"

#include <string>

/**
 * Represents a map in the game.
 * Class can load the map and draw it on the background.
 */
class CMap {
public:
    ///Map constructor.
    CMap(std::string texName, int tileSize, int tileScale);
    /// Map destructor.
    ~CMap();
    /**
     * Loads a map from a file.
     * Map is required to be in format:
     * <int: map width> <int: map height>
     * <int int: map tile index and collision flag pairs>
     * <string: map name>
     * ex.:
     * 3 2
     * 1 1  1 1  1 1
     * 1 1  0 0  1 1
     * Example_map
     * @return True when the map loaded successfully. False otherwise.
     */
    bool loadMap(const char * pathToMapFile);
    /// Adds a tile.
    void addTile(int id,  int posX, int posY);
    /// Returns width in tiles
    int width(){return m_Width;}
    /// Returns height in tiles
    int height(){return m_Height;}
private:
    std::string m_MapTexName;
    int m_TileSize;
    int m_TileScale;
    int m_ScaledSize;
    int m_Width;
    int m_Height;
};


#endif //RPG_GAME_ENGINE_CMAP_HPP
