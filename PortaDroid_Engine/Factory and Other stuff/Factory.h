#pragma once
#include "GameObjectComposition.h"
#include "SystemsBase.h"
#include "VMath.h"

namespace OBALFramework
{
  // Forward declaration of the component creator
  class ComponentCreator;

  // The game object factory creates compositions/objects from data stream(files) and manages 
  // their lifetime. As part of the controlling the lifetimes of GOC it also provides integer
  // based ID system for safe referencing of game objects through integer ID handles.
  class GameObjectFactory : public ISystem
  {
    public:

      // Constructors and Destructor
      GameObjectFactory();
      ~GameObjectFactory();


      void Initialize();

      // Create the object and give it an ID
      GOC* Create(const std::string& filename);

      ///Add a GOC to the destroy list for delayed destruction.
      void Destroy(GOC * gameObject);

      ///Update the factory, destroying dead objects.
      virtual void Update(float dt);

      ///Name of the system is FACTORY.
      virtual std::string GetName() { return "FACTORY"; }

      ///Message Interface see Message.h
      virtual void ThrowMessage(Message * message);

      ///Destroy all the GOCs in the world. Used for final shutdown.
      void DestroyAllObjects();

      ///Create and Id a GOC at runtime. Used to dynamically build GOC.
      ///After components have been added call GOC->Initialize().
      GOC * CreateEmptyComposition();

      ///Build a composition and serialize from the data file but do not initialize the GOC.
      ///Used to create a composition and then adjust its data before initialization
      ///see GameObjectComposition::Initialize for details.
      GOC * BuildAndSerialize(const std::string& filename);

      ///Id object and store it in the object map.
      void IdGameObject(GOC* gameObject);

      ///Add a component creator enabling data driven composition
      void AddComponentCreator(const std::string& name, ComponentCreator* creator);

      ///Get the #include game object with given id. This function will return NULL if
      ///the object has been destroyed.
      GOC * GetObjectWithId(GOCId id);

      GOC * GetObjectWithName(std::string & name);

    private:

      ///Used to incrementally generate unique id's.
      unsigned LastGame_ObjectId;

      ///Map of component creator used for data driven composition
      typedef std::map<std::string, ComponentCreator*> ComponentMapType;
      ComponentMapType _ComponentMap;

      ///Map of GOC to their Ids used for safe referencing of game objects
      typedef std::map<unsigned, GOC*> GameObjectIdMapType;
      GameObjectIdMapType _GameObjectIdMap;

      ///Objects to be deleted
      ///this is a set to prevent problems when a game object
      ///is deleted in multiple locations
      std::set<GOC*> _ObjectsToBeDeleted;
  };


  extern GameObjectFactory * FACTORY;
}
