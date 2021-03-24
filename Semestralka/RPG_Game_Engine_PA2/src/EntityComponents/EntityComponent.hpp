//
// Created by jorge on 6/18/20.
//

/**
 * * This header file implements an entity component system.
 *
 * * Allows to create entities and add components to them.  Components add functionality to the entity.
 * Components can be of all different sorts, collision, keyboard control, movement, etc. All components inherit from the CComponent class.
 * * The entity holds all its components in a container and during its own update() and draw() methods loops through all
 * its components and through polymorphism each component does its own update() and draw().
 *
 * I'm no expert in this, so here are some links to get more information on this matter.
 * @see https://en.wikipedia.org/wiki/Entity_component_system
 * @see https://www.youtube.com/watch?v=2rW7ALyHaas&ab_channel=BoardToBitsGames
 */

#ifndef RPG_GAME_ENGINE_ENTITYCOMPONENT_H
#define RPG_GAME_ENGINE_ENTITYCOMPONENT_H

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "../RPGconfig.hpp"

// For more info about constexpr
// https://www.geeksforgeeks.org/understanding-constexper-specifier-in-c/
/// Set the maximum number of components.
constexpr std::size_t maxComponents = 32;
/// Set maximum number of render & collision groups.
constexpr std::size_t maxGroups = 32;

/*!
 * Parent component class. All other components will inherit from this one.
 * Components will have their own behaviour, this is where polymorphism is used.
 */
class CComponent;
/**
 * Entity class represents any game object.
 */
class CEntity;
/**
 * Entity manager stores all games entities.
 */
class CEntityManager;

// Basically just an alias.
using ComponentID = std::size_t;
using Group = std::size_t;

/// Generates a new component id for each type of component.
inline ComponentID getNewComponentTypeID(){
    static ComponentID last_ID = 0u;
    return last_ID++;
}
/// Components of same type will have the same ID. This allows to identify the component.
template <typename T> inline ComponentID getComponentTypeID() noexcept{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

class CComponent{
public:
    /**
     * Initialize component. Always gets called first.
     */
    virtual void init(){};
    /**
     * Update component. Gets called every game update method.
     */
    virtual void update(){};
    /**
     * Draw gets called every game render method.
     */
    virtual void draw(){};
    /**
     * Virtual destructor to prevent problems.
     */
    virtual ~CComponent(){};
public:
    /// Pointer to component owner.
    CEntity * m_Entity;
};

class CEntity {
public:
    /**
     * @param manager Games CEntityManager.
     */
    CEntity(CEntityManager& manager):m_EntManager(manager){}
    /// Updates all stored components.
    void update(){
        for (auto & c : m_Components)
            c->update();
    }
    /// Draws all stored components.
    void draw(){
        for (auto & c : m_Components)
            c->draw();
    }
    /// States whether the entity is active. The entity manager checks for non active entities and destroys them.
    bool isActive() const {return m_Active;}
    /// Sets the entity up for destruction by the entity manager.
    void destroy(){m_Active = false;}
    /// States whether the entity is part of a group given by the param.
    bool isInGroup(Group group){return m_GroupBitSet[group];}
    /// Adds entity to a group.
    void addGroup(Group group); // placed in EntityComponent.cpp file because CEntityManager is incomplete here.
    /// Removes the entity from a group.
    void delGroup(Group group){m_GroupBitSet[group] = false;}
    /**
     * Returns a boolean indicating whether the entity has the component.
     * @tparam T Component type to be checked
     * @return True when entity has component. False otherwise.
     */
    template <typename T> bool hasComponent() const {
        return m_ComponentBitSet[getComponentTypeID<T>()];
    }
    /**
     * Allows to add one or more components to an entity.
     * This works using variadic arguments and parameter packs.
     * @tparam T Single component
     * @tparam TArgs Components collection
     * @param mArgs Collection of components. Passed from tparams.
     * @return Pointer to the component.
     */
     // More about variadic arguments and parameter packs here:
     // https://en.cppreference.com/w/cpp/language/variadic_arguments
     // https://en.cppreference.com/w/cpp/language/parameter_pack
    template <typename T, typename ... TArgs> T& addComponent(TArgs&&... mArgs){
        // Forwards arguments to the new operator.
        // std::forward figures how to pass argument as rvalue or lvalue based on the argument.
        // https://en.cppreference.com/w/cpp/utility/forward
        T* c(new T(std::forward<TArgs>(mArgs)...)); // Create a pointer to the component.
        c->m_Entity = this;
        std::unique_ptr<CComponent> uniquePtr{ c };
        m_Components.emplace_back(std::move(uniquePtr)); // Place the pointer in the vector of pointers.
        // Components will always be placed in the same place in the array.
        m_ComponentArray[getComponentTypeID<T>()] = c;
        m_ComponentBitSet[getComponentTypeID<T>()] = true;
        c->init();
        return *c;
    }
    /**
     * Allows to access components of each class.
     * @tparam T Component to be accessed.
     * @return Static pointer to component.
     */
    template <typename T> T& getComponent() const {
        // get the ID of the component and make a pointer to it. The ID is based of the components type.
        // Make a pointer using the ID in the array
        CComponent * ptr = m_ComponentArray[getComponentTypeID<T>()];
        // This static cast allows access to the component with this syntax:
        // CEntity.getComponent<Component>()
        // https://en.cppreference.com/w/cpp/language/static_cast // keyword: upcast
        // upcasting allows to access the common virtual interface of parent class.
        return *static_cast<T*>(ptr);
    }
private:
    CEntityManager& m_EntManager;
    bool m_Active = true;
    std::vector<std::unique_ptr<CComponent>> m_Components; ///< Vector with unique pointers to the components. Used in update() and draw()
    std::array<CComponent *, maxComponents> m_ComponentArray; ///< Array of components. Allows access with the components ID
    std::bitset<maxComponents> m_ComponentBitSet; ///< Bitset with components that the entity has.
    std::bitset<maxComponents> m_GroupBitSet; ///< Bitset with groups that the entity is part of.
};

class CEntityManager{
public:
    /// Updates all stored entities.
    void update(){
        for (auto& ent : m_Entities)
            ent->update();
    }
    /// Draws all stored entities.
    void draw(){
        for (auto& ent : m_Entities)
            ent->draw();
    }

    /**
     * Iterates through all entities. Remove them from groups or completely if they are not active.
     */
     // Note to future self:
     // This method heavily uses lambdas so here are a few links with explanations.
     // https://en.cppreference.com/w/cpp/language/lambda
     // https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=vs-2019
     // Also there's comments in the code... So gl figuring it out if you ever look here again...
     void refresh(){
         // This loop removes entities from groups.
         for (auto i(0u); i< maxGroups; i++){                                              // iterate all groups saved in array, start from uint 0
            auto & v(m_EntityGroups[i]);                                                  // create reference to the vector in the array
            v.erase(                                                                      // erase entity from the vector in range from [first,last)
                    std::remove_if(std::begin(v),                                 // remove_if moves all elements for which the predicate returns true
                                        std::end(v),                                   // remove_if only shifts elements and moves the true ones to the back
                                        [i](CEntity * entity){                       // lambda predicate
                                            return !entity->isActive() || !entity->isInGroup(i);}),
                    std::end(v));                                                       // end of erase

        }
        // This part deletes nonactive entities.
        m_Entities.erase(std::remove_if(std::begin(m_Entities),std::end(m_Entities),
                 [](const std::unique_ptr<CEntity> & mEntity){
                            return !mEntity->isActive();
                 }),
        std::end(m_Entities)
        );
    }
    /// Adds an entity to a group.
    void addToGroup(CEntity * entity, Group group){
         m_EntityGroups[group].emplace_back(entity);
     }
     /// Gives access to a group of entities.
     std::vector<CEntity*> & getGroup(Group group){
         return m_EntityGroups[group];
     }
    /// Adds entity to manager.
    CEntity& addEntity(){
        CEntity * ent = new CEntity(*this);
        std::unique_ptr<CEntity> uniquePtr {ent};
        m_Entities.emplace_back(std::move(uniquePtr));
        return *ent;
    }
private:
    std::vector<std::unique_ptr<CEntity>> m_Entities;
    std::array<std::vector<CEntity*>, maxGroups> m_EntityGroups;
};

#endif //RPG_GAME_ENGINE_ENTITYCOMPONENT_H
