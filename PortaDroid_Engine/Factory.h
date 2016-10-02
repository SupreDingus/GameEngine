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

      /////////////////////////////////////////////////////////////////////////////
      /// *****CONSTRUCTOR-DESTRUCTOR***** ///
      /////////////////////////////////////////////////////////////////////////////
      // Constructors and Destructor
      GameObjectFactory();
      ~GameObjectFactory();

      ///Name of the system is FACTORY.
      virtual std::string GetName() { return "FACTORY"; }


      /////////////////////////////////////////////////////////////////////////////
      /// *****SYSTEM INITIALIZATIONS***** ///
      /////////////////////////////////////////////////////////////////////////////
      void Initialize();


      /////////////////////////////////////////////////////////////////////////////
      /// *****SYSTEM UPDATE***** ///
      /////////////////////////////////////////////////////////////////////////////
      ///Update the factory, destroying dead objects.
      virtual void Update(float dt);


      /////////////////////////////////////////////////////////////////////////////
      /// *****STANDARD OBJECT CREATION***** ///
      /////////////////////////////////////////////////////////////////////////////
      ///Create and Id a GOC at runtime. Used to dynamically build GOC.
      ///After components have been added call GOC->Initialize().
      GOC * CreateEmptyObject(bool startwithtransform);

      // NEEDS IMPLEMENTATION TO WORK!!!!!!!!!!!!!!!!!!!!!!!!
      GOC * CreateCamera();
      GOC * CreateSprite();
      GOC * CreateAnimatableSprite();
      GOC * CreateDefaultText();

      // Create the object and give it an ID
      GOC* CreatePremadeObject(const std::string& filename);

      ///Build a composition and serialize from the data file but do not initialize the GOC.
      ///Used to create a composition and then adjust its data before initialization
      ///see GameObjectComposition::Initialize for details.
      ///
      ///GOC * BuildAndSerialize(const std::string& filename);


      /////////////////////////////////////////////////////////////////////////////
      /// *****COMPONENT CREATION FOR GAMEOBJECTS***** ///
      /////////////////////////////////////////////////////////////////////////////
      void GiveComponentToObject(char * componentname, GOC * object);

      ///Id object and store it in the object map.
      void GiveIdtoGameObject(GOC* gameObject);


      /////////////////////////////////////////////////////////////////////////////
      /// *****GAMEOBJECT SEARCHING***** ///
      /////////////////////////////////////////////////////////////////////////////
      ///Get the #include game object with given id. This function will return NULL if
      ///the object has been destroyed.
      GOC * GetObjectWithId(GOCId id);

      GOC * GetObjectWithName(std::string & name);


      /////////////////////////////////////////////////////////////////////////////
      /// *****SYSTEMS MESSAGING***** ///
      /////////////////////////////////////////////////////////////////////////////
      ///Message Interface see Message.h
      virtual void ThrowMessage(Message * message);


      /////////////////////////////////////////////////////////////////////////////
      /// *****COMPONENT REGISTRATION***** ///
      /////////////////////////////////////////////////////////////////////////////
      // Register all Components that this system will handle and need upon creation an initialization
      void RegisterAppropriateComponents();


      /////////////////////////////////////////////////////////////////////////////
      /// *****COMPONENT CREATOR CREATION***** ///
      /////////////////////////////////////////////////////////////////////////////
      ///Add a component creator enabling data driven composition
      void AddComponentCreator(const std::string& name, ComponentCreator* creator);
      

      /////////////////////////////////////////////////////////////////////////////
      /// *****CLEANUP***** ///
      /////////////////////////////////////////////////////////////////////////////
      ///Add a GOC to the destroy list for delayed destruction.
      void Destroy(GOC * gameObject);
      ///Destroy all the GOCs in the world. Used for final shutdown.
      void DestroyAllObjects();


    private:

      ///Used to incrementally generate unique id's.
      unsigned LastGame_ObjectId;

      ///Map of component creator used for data driven composition
      typedef std::map<std::string, ComponentCreator*> ComponentMapType;
      ComponentMapType _ComponentMap;

      ///Map of GOC to their Ids used for safe referencing of game objects
      typedef std::map<unsigned, GOC*> GameObjectIdMapType;
      GameObjectIdMapType _GameObjectIdMap;

      ///Vector of game objects that want to recieve Update()
      std::vector<GOC*> _GameObjectsForUpdate;

      ///Objects to be deleted
      ///this is a set to prevent problems when a game object
      ///is deleted in multiple locations
      std::set<GOC*> _ObjectsToBeDeleted;
  };


  extern GameObjectFactory * FACTORY;
}
