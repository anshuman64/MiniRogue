//
//  Player.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include "Player.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"


// ******************************
// * Constructor
// ******************************
Player::Player(Dungeon* dungeon) : Actor(dungeon, '@', "Player", 20, 2, 2, 2), is_godMode(false) {
  Weapon* startWeapon = new Weapon(dungeon, "Short Sword", 2, 0, "slashes");
  m_inventory.push_back(startWeapon);
  m_weapons.push_back(startWeapon);
  setWeapon(startWeapon);
}

Player::~Player() {
  delete getWeapon();
  
  for (int i = 0; i < m_weapons.size(); i++) {
    delete m_weapons[i];
  }
}

// ******************************
// * Actions
// ******************************

void Player::calculateMove(char input) {
  int curRow = getRowPosition();
  int curCol = getColPosition();
  int nextRow = curRow;
  int nextCol = curCol;
  string directionString;
  
  switch (input) {
    case ARROW_LEFT:
      nextCol--;
      directionString = "left";
      break;
    case ARROW_RIGHT:
      nextCol++;
      directionString = "right";
      break;
    case ARROW_UP:
      nextRow--;
      directionString = "up";
      break;
    case ARROW_DOWN:
      nextRow++;
      directionString = "down";
      break;
  }
  
  if (getDungeon()->isWall(nextRow, nextCol)) {
    getDungeon()->addAction("Player tried moving " + directionString + " but was blocked by a wall.");
  }
  
  else if (getDungeon()->isRoom(nextRow, nextCol)) {
    move(curRow, curCol, nextRow, nextCol);
    getDungeon()->addAction("Player moved " + directionString + ".");
  }
  
  else if (getDungeon()->isMonster(nextRow, nextCol)) {
    attack(this, (Actor*)getDungeon()->getObject(nextRow, nextCol));
  }
  
  else if (getDungeon()->isGameObject(nextRow, nextCol)) {
    move(curRow, curCol, nextRow, nextCol);
    getDungeon()->addAction("Player moved " + directionString + " onto " + getOverGameObject()->getName() + ".");
  }
}

// Return true if item picked up to set didPlayerMove in Game.cpp
bool Player::pickGameObject() {
  GameObject* gameObject = getOverGameObject();
  
  if (gameObject == nullptr) {
    getDungeon()->addAction("No item to pick up.");
    return false;
  }
  
  if (gameObject->isGoldenIdol()) {
    getDungeon()->endGame("Player picked up the Golden Idol. You win!");
    return false;
  }
  
  if (m_inventory.size() > 25) {
    getDungeon()->addAction("Inventory full.");
    return false;
  }
  
  m_inventory.push_back(gameObject);
  if (gameObject->isWeapon()) {
    m_weapons.push_back((Weapon*)gameObject);
  }
  setOverGameObject(nullptr);
  getDungeon()->addAction("Player picked up a " + gameObject->getName() + ".");
  return true;
}

void Player::descendStairs() {
  if (!getOverGameObject()->isStairs()) {
    getDungeon()->addAction("No stairs to descend");
    return;
  }

  setOverGameObject(nullptr);
  getDungeon()->addAction("Player moved to next level of dungeon!");
  getDungeon()->nextLevel();
}

void Player::godMode() {
  if (is_godMode) {
    getDungeon()->addAction("God Mode already activated.");
    return;
  }
  
  changeMaxHP(50);
  changeHP(50);
  changeStrength(9);
  getDungeon()->addAction("Player feels the strength of the Almighty.");
  is_godMode = true;
}

// ******************************
// * Display
// ******************************

void Player::displayStats() {
  cout << "Dungeon Level: " << getDungeon()->getLevel() << " | ";
  cout << "HP: " << getHP() << "/" << getMaxHP() << " | ";
  cout << "Armor: " << getArmor() << " | ";
  cout << "Strength: " << getStrength() << "(+" << getWeapon()->getDamage() << ") | ";
  cout << "Dexterity: " << getDexterity() << "(+" << getWeapon()->getDexBonus() << ")";
  cout << endl << endl;
}

void Player::displayInventory() {
  clearScreen();
  
  cout << "Inventory:" << endl;
  
  char letterIndex = 'a';
  for (int i = 0; i < m_inventory.size(); i++) {
    cout << (char)(letterIndex + i) << ". " << m_inventory[i]->getName() << " - " << m_inventory[i]->getDescription() << endl;
  }
  
  cout << endl << "Press any key to continue." << endl;

  getCharacter();
}

void Player::displayWeapons() {
  clearScreen();
  
  cout << "Weapons:" << endl;
  
  char letterIndex = 'a';
  for (int i = 0; i < m_weapons.size(); i++) {
    cout << (char)(letterIndex + i) << ". " << m_weapons[i]->getName() << " - " << m_weapons[i]->getDescription() << endl;
  }
  
  cout << endl << "Press letter to equip or any other key to continue." << endl;

  char input = getCharacter();
  int inputIndex = input - letterIndex;
  
  if (inputIndex < m_weapons.size()) {
    setWeapon(m_weapons[inputIndex]);
    getDungeon()->addAction("Player equipped " + m_weapons[inputIndex]->getName() + ".");
  }
}
