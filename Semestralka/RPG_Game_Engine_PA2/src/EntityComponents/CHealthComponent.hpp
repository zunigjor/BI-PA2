//
// Created by jorge on 6/20/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CHEALTHCOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CHEALTHCOMPONENT_HPP

#include "Components.hpp"

/**
 * Health component implements health management.
 */
class CHealthComponent : public CComponent {
public:
    CHealthComponent(int maxHealth, int damage, int atkSpeed):
    m_MaxHealth(maxHealth),m_Damage(damage),m_AttackSpeed(atkSpeed){}
    /// Initializes the component.
    void init() override {
        m_CurrentHealth = m_MaxHealth;
        m_Alive = true;
        m_LastAttack = 0;
        m_CanAttack = true;
    }
    /// Updates the component.
    void update() override {
        if (m_CurrentHealth <= 0 ){
            m_Alive = false;
            m_CurrentHealth = 0;
        }
        if (!m_CanAttack){
            int timeNow = static_cast<int>(SDL_GetTicks());
            if (timeNow - m_LastAttack > m_AttackSpeed)
                m_CanAttack = true;
        }
    }
    /// Allows to take damage.
    void takeDamage(int damage){
        if (m_CurrentHealth > 0)
            m_CurrentHealth -= damage;
    }
    /// Allows to add damage (from items etc.)
    void addDamage(int damage){
        m_Damage += damage;
    }
    /// Allows to heal.
    void heal(int heal){
        m_CurrentHealth += heal;
        if (m_CurrentHealth > m_MaxHealth)
            m_CurrentHealth = m_MaxHealth;
    }
    /// States whether the entity is alive.
    bool alive(){return m_Alive;}
    int getCurrentHealth(){return m_CurrentHealth;};
    int getMaxHealth(){return m_MaxHealth;}
    /// Returns the amount of damage the entity has. Returns 0 if the attack is on cooldown.
    int getDamage(bool isAttacking){
        if (!isAttacking)
            return m_Damage;
        if (m_CanAttack){
            m_LastAttack = SDL_GetTicks(); // save the time of the last attack
            m_CanAttack = false;
            return m_Damage;
        }
        return 0;
    }
    int getAttackSpeed(){return m_AttackSpeed;}
private:
    int m_MaxHealth;
    int m_CurrentHealth;
    int m_Damage;
    int m_AttackSpeed;
    bool m_Alive;
    int m_LastAttack;
    bool m_CanAttack;
};

#endif //RPG_GAME_ENGINE_PA2_CHEALTHCOMPONENT_HPP
