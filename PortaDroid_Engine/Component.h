#pragma once

#include "ComponentTypeIds.h"
#include "Message.h"
//#include "Serialization.h"


namespace OBALFramework
{
  // Forward Declaration of the GOC class
  class GameObjectComposition;
  /// GOC = GameObjectComposition
  typedef GameObjectComposition GOC;


  // A component is an atomic piece of functionality that is
  // composed onto a GameObjectComposition to form game objects.
  class GameComponent
  {
    public:
      friend class GameObjectComposition;

      // Signal that the component is now active in the game world.
      virtual void Initialize() {};

      virtual void Update(float dt) {};

      // GameComponents receive all messages sent to thier owning composition
      virtual void ThrowMessage(Message *) {};

      // Component Serialization Interface
      ///virtual void Serialize(BaseSerializer& str) {};

      // Get the GameObjectComposition this component is owned by
      GOC* GetOwner() { return _Owner; }
      void SetOwner(GOC * ownerobject) { _Owner = ownerobject; }


      // The enum type of component this is (See ComponentTypeIds.h to see each ID)
      ComponentTypeId TypeId;

    protected:
      // Component Destructor
      virtual ~GameComponent() {};

    private:
      // Each component has a pointer back to the composition/object that owns it
      GOC* _Owner;
  };
}