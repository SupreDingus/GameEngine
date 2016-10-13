#include "LevelGen.h"

#define DEFAULT_WIDTH 16
#define DEFAULT_HEIGHT 9

namespace OBALFramework
{
  LevelGen::LevelGen() { }
  LevelGen::~LevelGen() { }

  void LevelGen::CreateEmptyLevel()
  {
    //Call it empty.txt.
    std::ofstream out("empty.txt", std::ofstream::out);

    //First write out the width and height.
    out << DEFAULT_WIDTH << std::endl;
    out << DEFAULT_HEIGHT << std::endl;

    //Write out all the 1's and 0's. Make it just a rectangle, with 0's in the middle.
    for (int i = 0; i < DEFAULT_HEIGHT; ++i)
    {
      for (int j = 0; j < DEFAULT_WIDTH; ++j)
      {
        //If it's the first or last row, or first or last column, it's a 1.
        if (i == 0 || i == DEFAULT_HEIGHT - 1 || j == 0 || j == DEFAULT_WIDTH - 1)
          out << '1';
        else
          out << '0';
      }
      out << '\n';
    }
  }

  void LevelGen::LoadLevel(std::string in)
  {
    GOC * object;
    Transform * trans;
    int width, height; //Width and height of the level.
    char check;

    //Open the input stream.
    std::ifstream file(in, std::ifstream::in);

    //Check to see if we didn't open the file.
    if (!file.is_open())
      return;

    //Get the width and height.
    file >> width;
    file >> height;

    //Get past the newline.
    check = file.get();

    //Step through the rest of the file.
    for (int i = 0; i < height; ++i)
    {
      for (int j = 0; j < width; ++j)
      {
        check = file.get();

        //Only make stuff if we don't find a 0.
        if (check == '1')
        {
          object = FACTORY->CreateSprite();
          FACTORY->GiveComponentToObject("Transform", object);

          trans = object->has(Transform);
          trans->Position.x = static_cast<float>(j - width / 2);
          trans->Position.y = static_cast<float>(-i + height / 2);
        }
        else if (check == '2')
        {
          object = FACTORY->CreateSprite();
          FACTORY->GiveComponentToObject("PlayerController", object); //PlayerController has a tranform.

          trans = object->has(Transform);
          trans->Position.x = static_cast<float>(j - width / 2);
          trans->Position.y = static_cast<float>(-i + height / 2);
        }
      }
      //Get past the newline.
      check = file.get();
    }
  }
}
