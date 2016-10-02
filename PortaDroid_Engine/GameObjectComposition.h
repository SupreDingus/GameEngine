#pragma once

#include "Message.h"
//#include "Serialization.h"
#include "Component.h"
#include "Containers.h"

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

      void Update(float dt);

      // Add the composition/object onto to the factory's destroy list
      void Destroy();

      // Send a message to each component on the composition/object
      void ThrowMessage(Message* message);


      // Get a component on this composition/object by the component's typename.
      // This will return NULL if the component could not be found
      GameComponent* GetComponent(ComponentTypeId typeId);



      /////////////////////////////////////////////////////////////////////////////////////////////
      /// COMPONENT ACCESS (Forward Declaration)
      /////////////////////////////////////////////////////////////////////////////////////////////
      // Type safe way of accessing components
      template <typename componenttype>
      componenttype* GetComponentType(ComponentTypeId typeId);


      // Add a component used only for dynamic composition construction.
      /// Try to make it so that this is never used while the game is running (like in Zero. Just a preference)
      void AddComponent(ComponentTypeId typeId, GameComponent* component); 

      // Get the game object's Id
      GOCId GetId() { return _ObjectId; }


      // Variables
      std::string Name = "No Name";

      //Archetype Archetype


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


  /////////////////////////////////////////////////////////////////////////////////////////////
  /// COMPONENT ACCESS
  /////////////////////////////////////////////////////////////////////////////////////////////
  // A more type safe way of accessing components.
  // Interface becomes Transform* transform = object->has(Transform);
  // Whatever is passed is a ComponentType.  The same ComponentType will be returned
  template<typename componenttype>
  componenttype * GameObjectComposition::GetComponentType(ComponentTypeId typeId)
  {
    // Return the exact type of component (i.e Transform, Sprite, etc.) not the base GameComponent
    return static_cast<componenttype*>(GetComponent(typeId));
  }
  


#define has(type) GetComponentType<type>(CT_##type)
  // For context, "##" in a preprocessor directive function basically pastes the thing on its left and the thing on its right together.
  // For example, here, "CT_" and the actual GameComponent that "type" is templetized to become the whole
  //   name of the Component type (CT_Transform = [1. CT_  +  2. "Transform"])

} 