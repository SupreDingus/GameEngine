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
#include "TextSerialization.h"
#include "MessageRecipientList.h"
#include "WindowsSystem.h"  //For Messaging stuff

namespace OBALFramework
{
  GameObjectFactory * FACTORY = NULL;

  GameObjectFactory::GameObjectFactory()
  {
    //ErrorIf(FACTORY != NULL, "Factory already created");
    FACTORY = this;
    LastGame_ObjectId = 0;
  }
  GameObjectFactory::~GameObjectFactory()
  {
    //Delete all component creators
    ComponentMapType::iterator it = _ComponentMap.begin();
    for (;it != _ComponentMap.end();++it)
      delete it->second;
  }


  // Just Register for messages
  void GameObjectFactory::Initialize()
  {
    //Tell the CoreEngine to Register the windows system into the Quit message Recipient Vector
    //This way, the windows system will ONLY get CharacterKey messages
    OutputDebugStringA("REGISTERING FOR HeldKey MESSAGE: Graphics\n");
    MSGRECIPIENTS->AddRecipient(this, Mid::CharacterKey);
  }


  GOC * GameObjectFactory::CreateEmptyComposition()
  {
    //Create the composition and give it an id
    //this function is used to create compositions
    //programmatically
    GOC * gameObject = new GOC();
    IdGameObject(gameObject);
    return gameObject;
  }

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

  void GameObjectFactory::IdGameObject(GOC* gameObject)
  {
    //Just increment the last id used. Does not handle 
    //overflow but it would take over 4 billion objects
    //to break
    ++LastGame_ObjectId;
    gameObject->_ObjectId = LastGame_ObjectId;

    //Store the game object in the global object id map
    _GameObjectIdMap[LastGame_ObjectId] = gameObject;
  }

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
    GameObjectIdMapType::iterator it = _GameObjectIdMap.begin;

    for (; it != _GameObjectIdMap.end(); ++it)
    {
      if (it->second->Name == name && it != _GameObjectIdMap.end())
        return it->second;
    }
    
    // The game object was not found
    return NULL;

  }


  GOC * GameObjectFactory::Create(const std::string& filename)
  {
    GOC * goc = BuildAndSerialize(filename);
    if (goc) goc->Initialize();
    return goc;
  }



  /// MESSAGING
  void GameObjectFactory::ThrowMessage(Message * message)
  {
    OutputDebugStringA("MESSAGE THROWN TO THIS RECIPIENT: Factory\n");
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



  void GameObjectFactory::Destroy(GOC * gameObject)
  {
    //Add the object to the to be deleted list they will be delete
    //when the factory is updated
    _ObjectsToBeDeleted.insert(gameObject);
  }

  void GameObjectFactory::Update(float dt)
  {
    //Delete all objects in the ObjectsToBeDeleted_ list 

    std::set<GOC*>::iterator it = _ObjectsToBeDeleted.begin();
    for (;it != _ObjectsToBeDeleted.end();++it)
    {
      GOC * gameObject = *it;
      GameObjectIdMapType::iterator idItr = _GameObjectIdMap.find(gameObject->_ObjectId);
      //ErrorIf(idItr == GameObjectIdMap_.end(), "Object %d was double deleted or is bad memory.", gameObject->_ObjectId);
      if (idItr != _GameObjectIdMap.end())
      {
        //Delete it and remove its entry in the Id map
        delete gameObject;
        _GameObjectIdMap.erase(idItr);
      }
    }
    //All objects to be delete have been deleted
    _ObjectsToBeDeleted.clear();
  }

  //Bind the creator for this component with the component name
  void GameObjectFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
  {
    _ComponentMap[name] = creator;
  }

  //Clean up the game world
  void GameObjectFactory::DestroyAllObjects()
  {
    GameObjectIdMapType::iterator it = _GameObjectIdMap.begin();
    for (;it != _GameObjectIdMap.end();++it)
      delete it->second;

    _GameObjectIdMap.clear();
  }

}