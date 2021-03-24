//
// Created by jorge on 6/15/20.
//

#include "CMap.hpp"

#include <sstream>
#include <fstream>

#include "interface/CLogger.hpp"
#include "CGame.hpp"
#include "EntityComponents/Components.hpp"


CMap::CMap(std::string texName, int tileSize, int tileScale):
        m_MapTexName(texName), m_TileSize(tileSize), m_TileScale(tileScale){
    m_ScaledSize = tileScale * tileSize;
}

CMap::~CMap() {
}

bool CMap::loadMap(const char * pathToMapFile) {
    int index;
    bool collision;
    std::string m_MapName;
    std::ifstream MapData(pathToMapFile, std::fstream::in | std::fstream::binary);
    if (MapData.is_open()){
        MapData >> m_Width;
        MapData >> m_Height;
        for (int row = 0; row < m_Height ; ++row) {
            for (int column = 0; column < m_Width; ++column) {
                MapData >> index;
                addTile(index,column * (m_TileSize * m_TileScale),row * (m_TileSize * m_TileScale));
                MapData >> collision;
                if (collision){
                    auto& coll(CGame::entityManager.addEntity());
                    coll.addComponent<CCollisionComponent>("terrain", column * m_ScaledSize,row * m_ScaledSize, m_ScaledSize);
                    coll.addGroup(CGame::groupColliders);
                }
            }
        }
        MapData >> m_MapName;
        std::stringstream ss;
        ss << "Loaded: " << m_MapName << " size: " << m_Width << ", " << m_Height;
        CLogger::printToLog(ss.str().c_str());

        MapData.close();
        return true;
    }
    std::stringstream errSS;
    errSS << "Error loading map from file: " << pathToMapFile;
    CLogger::printToLog(errSS.str().c_str());
    return false;
}

void CMap::addTile(int id, int posX, int posY) {
    auto& tile(CGame::entityManager.addEntity());
    tile.addComponent<CTileComponent>(posX, posY, id, m_MapTexName, m_TileSize, m_TileScale);
    tile.addGroup(CGame::groupMap);
}
