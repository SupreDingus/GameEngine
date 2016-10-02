///////////////////////////////////////////////////////////////////////////////////////////////////
// TODOs: 
// ---- Object IDs aren't handled properly.  When an object is destroyed, new objects aren't
//      given the old ID.  Instead, the last number ID is used.  This means the IDs can get REALLY
//      BIG after a while.
// ---- 
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Factory.h"
#include "GameObjectComposition.h"
#include "ComponentCreator.h"
#include "ComponentTypeIds.h"
//#include "TextSerialization.h"
#include "MessageRecipientList.h"
#include "WindowsSystem.h"  //For Messaging stuff

// This is here Temporarily.  We may just include a header file with all of the components so other systems
// don't have to register components.
#include "CameraController.h"

namespace OBALFramework
{
  GameObjectFactory * FACTORY = NULL;

  /////////////////////////////////////////////////////////////////////////////
  /// *****CONSTRUCTOR-DESTRUCTOR***** ///
  /////////////////////////////////////////////////////////////////////////////
  GameObjectFactory::GameObjectFactory()
  {
    //ErrorIf(FACTORY != NULL, "Factory already created");
    FACTORY = this;
    LastGame_ObjectId = 0;
  }
  GameObjectFactory::~GameObjectFactory()
  {
    OutputDebugStringA("DESTRUCTING: Object Factory\n");

    //Delete all component creators
    ComponentMapType::iterator it = _ComponentMap.begin();
    for (; it != _ComponentMap.end(); ++it)
    {
      OutputDebugStringA("Destroying Component Creator: '");
      OutputDebugStringA(it->first.c_str());
      OutputDebugStringA("'\n");

      delete it->second;
    }

    DestroyAllObjects();
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEM INITIALIZATIONS***** ///
  /////////////////////////////////////////////////////////////////////////////
  // Just Register for messages
  void GameObjectFactory::Initialize()
  {
    // Register Components
    RegisterAppropriateComponents();

    if (MSGRECIPIENTS != NULL)
    {
      //Tell the CoreEngine to Register the windows system into the Quit message Recipient Vector
      //This way, the windows system will ONLY get CharacterKey messages
      OutputDebugStringA("REGISTERING FOR HeldKey MESSAGE: Graphics\n");
      MSGRECIPIENTS->AddRecipient(this, Mid::CharacterKey);
    }
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEM UPDATE***** ///
  /////////////////////////////////////////////////////////////////////////////
  void GameObjectFactory::Update(float dt)
  {
    //Delete all objects in the ObjectsToBeDeleted_ list 
    std::set<GOC*>::iterator it = _ObjectsToBeDeleted.begin();
    for (; it != _ObjectsToBeDeleted.end(); ++it)
    {
      GOC * gameObject = *it;
      GameObjectIdMapType::iterator idItr = _GameObjectIdMap.find(gameObject->_ObjectId);
      //ErrorIf(idItr == GameObjectIdMap_.end(), "Object %d was double deleted or is bad memory.", gameObject->_ObjectId);
      if (idItr != _GameObjectIdMap.end())
      {
        OutputDebugStringA("Deleting Object: '");
        OutputDebugStringA(gameObject->Name.c_str());
        OutputDebugStringA("'\n");

        //Delete it and remove its entry in the Id map
        delete gameObject;
        _GameObjectIdMap.erase(idItr);
      }
    }
    //All objects to be delete have been deleted
    _ObjectsToBeDeleted.clear();


    // Update all living objects
    for (unsigned i = 0; i < _GameObjectsForUpdate.size(); ++i)
      _GameObjectsForUpdate[i]->Update(dt);

  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****STANDARD OBJECT CREATION***** ///
  /////////////////////////////////////////////////////////////////////////////
  // Common object that are pre-implemented for use (No serialization as of 8/21)
  GOC * GameObjectFactory::CreateEmptyObject(bool startwithtransform)
  {
    //Create the composition and give it an id
    //this function is used to create compositions
    //programmatically
    GOC * gameObject = new GOC();
    GiveIdtoGameObject(gameObject);

    if (startwithtransform == true)
    {
      GiveComponentToObject("Transform", gameObject);
      gameObject->Name = "Transform";
    }
    else
    {
      gameObject->Name = "Empty Object";
    }

    _GameObjectsForUpdate.push_back(gameObject);

    return gameObject;
  }
  
  GOC * GameObjectFactory::CreateCamera()
  {
    GOC * gameObject = CreateEmptyObject(true);
    GiveComponentToObject("Camera", gameObject);
    GiveComponentToObject("CameraController", gameObject);

    gameObject->Name = "Camera";

    return gameObject;
  }
  GOC * GameObjectFactory::CreateSprite()
  {
    GOC * gameObject = CreateEmptyObject(true);
    GiveComponentToObject("Sprite", gameObject);

    gameObject->Name = "Sprite";

    return gameObject;
  }
  GOC * GameObjectFactory::CreateAnimatableSprite()
  {
    GOC * gameObject = CreateEmptyObject(true);
    GiveComponentToObject("AnimatableSprite", gameObject);

    gameObject->Name = "AnimatableSprite";

    return gameObject;
  }
  GOC * GameObjectFactory::CreateDefaultText()
  {
    GOC * gameObject = CreateEmptyObject(true);
    GiveComponentToObject("DefaultText", gameObject);

    gameObject->Name = "DefaultText";

    return gameObject;
  }

  GOC * GameObjectFactory::CreatePremadeObject(const std::string& filename)
  {
    //Create a new game object to hold the components
    GOC * gameObject = new GOC();//BuildAndSerialize(filename);

                                 //Find the component's creator
    ComponentMapType::iterator it = _ComponentMap.find("Transform");      //TEMPORARY

    if (it != _ComponentMap.end())
    {
      //ComponentCreator is an object that creates the component
      ComponentCreator * creator = it->second;

      //Create the component by using the interface
      GameComponent * component = creator->Create();

      //Serialize the component with current stream (this will move
      //the stream to the next component

      //Add the new component to the composition
      gameObject->AddComponent(creator->TypeId, component);

    } // This is where serializations stuff would go

      //Id and initialize the game object composition
    GiveIdtoGameObject(gameObject);

    if (gameObject) gameObject->Initialize();
    return gameObject;
  }

  ///BuildAndSerialize function
  /*
  GOC * GameObjectFactory::BuildAndSerialize(const std::string& filename)
  {
    //Open the text file stream serializer
    TextSerializer txtstream;
    bool fileOpened = txtstream.Open(filename);
    //ErrorIf(!fileOpened, "Could not open file %s. File does not exist or is protected.", filename.c_str());
    std::string componentName;

    //Make sure the stream is valid
    if (txtstream.IsGood())
    {
      //Create a new game object to hold the components
      GOC * gameObject = new GOC();

      while (txtstream.IsGood())
      {
        //Read the component's name
        StreamRead(txtstream, componentName);

        //Find the component's creator
        ComponentMapType::iterator it = _ComponentMap.find(componentName);
        //ErrorIf(it == ComponentMap_.end(), "Could not find component creator with name %s", componentName.c_str());
        if (it != _ComponentMap.end())
        {
          //ComponentCreator is an object that creates the component
          ComponentCreator * creator = it->second;

          //Create the component by using the interface
          GameComponent * component = creator->Create();

          //Serialize the component with current stream (this will move
          //the stream to the next component

          //Note: this invokes the base case of serialization in which
          //the object serializes itself. component->Serialize( stream );
          StreamRead(txtstream, *component);

          //Add the new component to the composition
          gameObject->AddComponent(creator->TypeId, component);
        }
      }

      //Id and initialize the game object composition
      IdGameObject(gameObject);

      return gameObject;
    }

    return NULL;
  }
  */


  /////////////////////////////////////////////////////////////////////////////
  /// *****COMPONENT CREATION FOR GAMEOBJECTS***** ///
  /////////////////////////////////////////////////////////////////////////////
  void GameObjectFactory::GiveComponentToObject(char * componentname, GOC * object)
  {
    //Find the component's creator
    ComponentMapType::iterator it = _ComponentMap.find(componentname);
    if (it != _ComponentMap.end())
    {
      OutputDebugStringA("ComponentCreator for Component: '");
      OutputDebugStringA(componentname);
      OutputDebugStringA("' was found.\n");

      //ComponentCreator is an object that creates the component
      ComponentCreator * creator = it->second;

      //Create the component by using the interface
      GameComponent * component = creator->Create();

      //Tell the component who its owning object is
      component->SetOwner(object);
      //Tell the component to initialize
      component->Initialize();


      //Add the new component to the GameObject composition
      object->AddComponent(creator->TypeId, component);
    }
    else
    {
      WINDOWSSYSTEM->Request_ERRORMessageBox(componentname, "ComponentCreator not found _ComponentMap of Factory.");
      WINDOWSSYSTEM->Request_ERRORMessageBox("^^^^  Object with this Name.", object->Name.c_str());
      
      OutputDebugStringA("ComponentCreator for Component: '");
      OutputDebugStringA(componentname);
      OutputDebugStringA("' could not be found within the _ComponentMap of the factory.\n");
    }
  }

  void GameObjectFactory::GiveIdtoGameObject(GOC* gameObject)
  {
    //Just increment the last id used. Does not handle overflow
    //  but it would take over 4 billion objects to break
    ++LastGame_ObjectId;
    gameObject->_ObjectId = LastGame_ObjectId;

    //Store the game object in the global object id map
    _GameObjectIdMap[LastGame_ObjectId] = gameObject;
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****GAMEOBJECT SEARCHING***** ///
  /////////////////////////////////////////////////////////////////////////////
  GOC * GameObjectFactory::GetObjectWithId(GOCId id)
  {
    GameObjectIdMapType::iterator it = _GameObjectIdMap.find(id);
    if (it != _GameObjectIdMap.end())
      return it->second;
    else
      return NULL;
  }

  GOC * GameObjectFactory::GetObjectWithName(std::string& name)
  {
    GameObjectIdMapType::iterator it = _GameObjectIdMap.begin();

    for (; it != _GameObjectIdMap.end(); ++it)
    {
      if (it->second->Name == name && it != _GameObjectIdMap.end())
        return it->second;
    }
    
    // The game object was not found
    return NULL;

  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEMS MESSAGING***** ///
  /////////////////////////////////////////////////////////////////////////////
  void GameObjectFactory::ThrowMessage(Message * message)
  {
    OutputDebugStringA("MESSAGE THROWN TO THIS RECIPIENT: Object Factory\n");

    OutputDebugStringA("THROWING MESSAGE TO ALL LIVING OBJECTS: Object Factory\n");
    // Give the message to all objects that are alive
    GameObjectIdMapType::iterator it = _GameObjectIdMap.begin();
    for (; it != _GameObjectIdMap.end(); ++it)
      it->second->ThrowMessage(message);


    // Now, check for messages this system actually needs to act upon
    switch (message->MessageId)
    {
      OutputDebugStringA("FINDING MESSAGE CASE: Factory\n");
      // The user has pressed a (letter/number) key, we may respond by creating
      // a specific object based on the key pressed.
    case Mid::CharacterKey:
    {
      
    }
    }
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****COMPONENT REGISTRATION***** ///
  /////////////////////////////////////////////////////////////////////////////
  void GameObjectFactory::RegisterAppropriateComponents()
  {
    // COMPONENT REGISTRATION
    RegisterComponent(Transform);
    RegisterComponent(CameraController);
  }

  /////////////////////////////////////////////////////////////////////////////
  /// *****COMPONENT CREATOR CREATION***** ///
  /////////////////////////////////////////////////////////////////////////////
  //Bind the creator for this component with the component name
  void GameObjectFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
  {
    OutputDebugStringA("Adding new component creator: '");
    OutputDebugStringA(name.c_str());
    OutputDebugStringA("' for Object Factory Component Creator Map.\n");

    _ComponentMap[name] = creator;
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****CLEANUP***** ///
  /////////////////////////////////////////////////////////////////////////////
  // Set the passed object to be destroyed
  void GameObjectFactory::Destroy(GOC * gameObject)
  {
    //Add the object to the to be deleted list they will be delete
    //when the factory is updated
    _ObjectsToBeDeleted.insert(gameObject);
  }
  
  //Clean up the game world
  void GameObjectFactory::DestroyAllObjects()
  {
    GameObjectIdMapType::iterator it = _GameObjectIdMap.begin();
    for (; it != _GameObjectIdMap.end(); ++it)
    {
      OutputDebugStringA("Destroying Object: '");
      OutputDebugStringA(it->second->Name.c_str());
      OutputDebugStringA("'\n");

      delete it->second;
    }

    _GameObjectIdMap.clear();
  }

}