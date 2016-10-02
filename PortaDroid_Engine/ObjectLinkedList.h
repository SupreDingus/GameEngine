#pragma once



namespace OBALFramework
{
  ///	Intrusively linked list for objects. This enables the ability to store 
  /// objects in an efficient linked list structure without duplicating code. 
  /// Object to be used in this container must have Prev and Next pointers as
  /// members.
  template< typename type >
  class ObjectLinkList
  {
  public:
    typedef type * pointer;
    typedef type& reference;

    ObjectLinkList() : First(0), Last(0), ObjectCount(0)
    {
    }

    //Pop the front element. If the list is empty return 0.
    pointer pop_front()
    {
      if (First == 0)
      {
        //List is empty
        return 0;
      }
      else
      {
        //get the first object
        type * first = First;
        First = first->Next;

        //Update linked list pointers
        if (First)
        {
          //more elements in list
          First->Prev = 0;
        }
        else
        {
          //popped last object in list
          Last = 0;
        }

        --ObjectCount;
        return first;
      }



    }

    void push_back(pointer object)
    {
      ++ObjectCount;

      if (Last == 0)
      {
        //list was empty
        First = object;
        Last = object;
        object->Next = 0;
        object->Prev = 0;
      }
      else
      {
        object->Prev = Last;
        object->Next = 0;
        Last->Next = object;
        Last = object;
      }
    }

    void erase(pointer object)
    {
      --ObjectCount;

      if (object->Next == 0 && object->Prev == 0)
      {
        //Only object in list make list empty
        Last = 0;
        First = 0;
      }
      else if (object->Next == 0)
      {
        //Object is last update Last
        Last = object->Prev;
        if (Last) Last->Next = 0;

      }
      else if (object->Prev == 0)
      {
        //Object is first update first
        First = object->Next;
        if (First) First->Prev = 0;
      }
      else
      {
        //Object is in middle just update pointers
        object->Prev->Next = object->Next;
        object->Next->Prev = object->Prev;
      }

    }

    ///Intrusive linked list iterator
    class iterator
    {
    public:
      friend class ObjectLinkList;
      iterator() {};
      iterator(pointer p) : item(p) {}
      void operator--() { item = item->Prev; }
      void operator++() { item = item->Next; }
      reference operator*() { return *item; }
      pointer operator->() { return item; }
      bool operator==(const iterator& i) { return item == i.item; }
      bool operator!=(const iterator& i) { return item != i.item; }
      operator bool() { return item != 0; }
      operator pointer() { return item; }
      pointer GetPtr() { return item; }
    private:
      pointer item;
    };

    iterator begin() { return First; }
    iterator end() { return 0; }
    pointer last() { return Last; }
  private:
    pointer First;
    pointer Last;
    unsigned ObjectCount;
  };
}