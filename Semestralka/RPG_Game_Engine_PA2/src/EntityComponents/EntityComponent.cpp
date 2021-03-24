//
// Created by jorge on 6/19/20.
//

#include "EntityComponent.hpp"

// Explanation of why this is here in EntityComponent.hpp
void CEntity::addGroup(Group group){
    m_GroupBitSet[group] = true;
    m_EntManager.addToGroup(this, group);
}
