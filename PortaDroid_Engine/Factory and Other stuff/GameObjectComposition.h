#pragma once

#include "Message.h"
#include "Serialization.h"
#include "Component.h"

namespace OBALFramework
{
  // Game Object Id Type
  typedef unsigned int GOCId;

  /// This class is the foundational object for all dynamic objects in the game world.
  /// The GOC is a place of logical interaction content and the primary mechanism
  /// game systems (Graphics, Physics, etc.) provide functionality and communicate.
  /// A GOC can be anything from physical objects like trees, tanks, and players to 
  /// logical objects like teams, triggers, or AIs.
  /// GameObject are formed by composing component objects into the game object compositon
  /// This is usually done by the GameObjectFactory.
  
  /// Game Object Lifetime States
  ///   - Constructed: The game object is constructed but is not initialized.Components can
  ///     be added and component data can be adjusted either by serialization or code.
  /// 
  ///   - Initialized : All components are initialized.This is the GOC's normal state.
  /// 
  ///   - Dead : The object still exists but it is on the GameObjectFactory's destroy list
  ///     and it's GOCId is no longer valid. it will be destoryed at the end of the frame.

  // A vector of component pointers is used to store components
  typedef std::vector<GameComponent*> ComponentArray;

  class GameObjectComposition
  {
    public: 

      // The factory is the only class that is allowed create and destroy game objects
      friend class GameObjectFactory;

      // Initialize all the of the components on the composition/object
      void Initialize();

      // Add the composition/object onto to the factory's destroy list
      void Destroy();

      // Send a message to each component on the composition/object
      void ThrowMessage(Message* message);


      // Get a component on this composition/object by the component's typename.
      // This will return NULL if the component could not be found
      GameComponent* GetComponent(ComponentTypeId typeId);

      // Type safe way of accessing components
      template <typename type>
      type* GetComponentType(ComponentTypeId typeId);

      // Add a component used only for dynamic composition construction.
      void AddComponent(ComponentTypeId typeId, GameComponent* component); 

      // Get the game object's Id
      GOCId GetId() { return _ObjectId; }


      // Variables
      std::string Name = "No Name";


    private:
      // Sorted array of components
      ComponentArray _Components;
      typedef ComponentArray::iterator _ComponentIt;

      // A unique ID for each object  used to safely reference GOCs
      GOCId _ObjectId;
         
      // The constructor and destructor are private to prevent a user
      // from creating or deleting an object directly they must use the Destroy function
         
      // FACTORY ONLY: Use CreateEmptyComposition on factory instead
      GameObjectComposition();
      GameObjectComposition(std::string name);

      // FACTORY ONLY: Use Destroy instead, factory will call the destructor correctly
      ~GameObjectComposition();
  };

  // A short name for GameObjectComposition
  typedef GameObjectComposition GOC;

  // A more type safe way of accessing components.
  // INterface becomes Transform* transform = object->has(Transform);
  template<typename type>
  type * GameObjectComposition::GetComponentType(ComponentTypeId typeid)
  {
    return static_cast<type*>(GetComponent(typeid));
  }

#define has(type) GetComponentType<type>( CT_##type )

} 