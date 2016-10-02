#include "Precompiled.h"
#include "GameObjectComposition.h"
#include "Factory.h"
#include <algorithm>

namespace OBALFramework
{
  //Used to sort components using their type Id.
  struct ComponentSorter
  {
    bool operator()(GameComponent* left, GameComponent* right)const
    {
      return left->TypeId < right->TypeId;
    }
  };

  //Binary search a sorted array of components.
  GameComponent* BinaryComponentSearch(ComponentArray& components, ComponentTypeId name)
  {
    size_t begin = 0;
    size_t end = components.size();

    // Search midway between each halfway segment of the array, starting with the whole array
    while (begin < end)
    {
      size_t mid = (begin + end) / 2;
      if (components[mid]->TypeId < name)
        begin = mid + 1;
      else
        end = mid;
    }

    // If the component was found, return it
    if ((begin < components.size()) && (components[begin]->TypeId == name))
      return components[begin];
    else
      return NULL;
  }

  void GameObjectComposition::Initialize()
  {
    //Initialize every component on this composition
    //and set their composition owner. This allows each
    //component to initialize itself separate from its constructor.
    //Which is need for serialization and to allow components to get
    //pointers to each other during initialization.
    for (_ComponentIt it = _Components.begin();it != _Components.end();++it)
    {
      (*it)->_Owner = this;
      (*it)->Initialize();
    }
  }



  // Constructor
  GameObjectComposition::GameObjectComposition()
  {
    _ObjectId = 0;
  }
  GameObjectComposition::GameObjectComposition(std::string name)
  {
    _ObjectId = 0;
    Name = name;
  }

  //Destructor
  GameObjectComposition::~GameObjectComposition()
  {
    //Delete each component using the component's virtual destructor
    //takes care of all resources and memory.
    for (_ComponentIt it = _Components.begin();it != _Components.end();++it)
      delete *it;
  }

  void GameObjectComposition::AddComponent(ComponentTypeId typeId, GameComponent* component)
  {
    //Store the components type Id
    component->TypeId = typeId;
    _Components.push_back(component);

    //Sort the component array so binary search can be used to find components quickly.
    std::sort(_Components.begin(), _Components.end(), ComponentSorter());
  }


  //Catches messages
  void GameObjectComposition::ThrowMessage(Message * message)
  {
    //When a message is sent to a composition it sends to
    //all of its components
    //for( MapIt it = Map.begin();it!=Map.end();++it)
    for (_ComponentIt it = _Components.begin();it != _Components.end();++it)
      (*it)->ThrowMessage(message);
  }


  // Gets component on this object
  GameComponent * GameObjectComposition::GetComponent(ComponentTypeId typeId)
  {
    return BinaryComponentSearch(_Components, typeId);
  }

  // Tells factory to destroy this object
  void GameObjectComposition::Destroy()
  {
    //Signal the factory that an object needs to be destroyed
    //this will happen at the end of the frame.
    FACTORY->Destroy(this);
  }
}