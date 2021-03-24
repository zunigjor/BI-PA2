//
// Created by jorge on 6/20/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CITEMCOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CITEMCOMPONENT_HPP

#include "EntityComponent.hpp"
#include "Components.hpp"

/**
 * Implements an item component which edits the damage and current health of the health component by the owner who picks it up.
 */
class CItemComponent : public CComponent{
public:
    CItemComponent(std::string name, int heal, int addDamage):
            m_Heal(heal), m_AddDamage(addDamage), m_Name(name){
    }
    ~CItemComponent(){};
    /// Init
    void init() override{};
    /// Checks if the item has been picked up.
    void update() override{
    }
    /// Allows the item be picked up.
    void pickup(CEntity * owner){
        owner->getComponent<CHealthComponent>().heal(m_Heal);
        owner->getComponent<CHealthComponent>().addDamage(m_AddDamage);

        m_Entity->destroy();
    }
    int getHeal(){return m_Heal;}
    int getDamage(){return m_AddDamage;}
private:
    int m_Heal;
    int m_AddDamage;
    std::string m_Name;
};

#endif //RPG_GAME_ENGINE_PA2_CITEMCOMPONENT_HPP
