//
//  main.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include <stdio.h>
#include "Game.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;


int main()
{
  srand(static_cast<unsigned int>(time(0)));
  srand(17); // Uncomment for testing

  Game g;
  g.play();
}
