//
//  Actor.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include "Actor.hpp"
#include "Dungeon.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"


// ******************************
// * Constructor
// ******************************

Actor::Actor(Dungeon* dungeon, char symbol, string name, int hp, int armor, int strength, int dexterity) : Object(dungeon, symbol, name), m_hp(hp), m_maxHP(hp), m_armor(armor), m_strength(strength), m_dexterity(dexterity), m_turnsAsleep(0), m_overGameObject(nullptr), m_weapon(nullptr) {}

Actor::~Actor() {
  if (!isPlayer()) {
    delete m_weapon;
  }
}


// ******************************
// * Accessors - Stats
// ******************************

int Actor::getHP() {
  return m_hp;
}

int Actor::getMaxHP() {
  return m_maxHP;
}

int Actor::getArmor() {
  return m_armor;
}

int Actor::getStrength() {
  return m_strength;
}

int Actor::getDexterity() {
  return m_dexterity;
}

bool Actor::isOverGameObject() {
  return m_overGameObject != nullptr;
}

bool Actor::isAsleep() {
  return m_turnsAsleep > 0;
}


// ******************************
// * Accessors - Other
// ******************************

Weapon* Actor::getWeapon() {
  return m_weapon;
}

GameObject* Actor::getOverGameObject() {
  return m_overGameObject;
}

// ******************************
// * Mutators - Stats
// ******************************

void Actor::changeHP(int delta) {
  m_hp += delta;
  m_hp = (m_hp > m_maxHP ? m_maxHP : m_hp);
  m_hp = (m_hp < 0 ? 0 : m_hp);
}

void Actor::changeMaxHP(int delta) {
  m_maxHP += delta;
  m_maxHP = (m_maxHP > MAX_STATS ? MAX_STATS : m_maxHP);
}

void Actor::changeArmor(int delta) {
  m_armor += delta;
  m_armor = (m_armor > MAX_STATS ? MAX_STATS : m_armor);
}

void Actor::changeStrength(int delta) {
  m_strength += delta;
  m_strength = (m_strength > MAX_STATS ? MAX_STATS : m_strength);
}

void Actor::changeDexterity(int delta) {
  m_dexterity += delta;
  m_dexterity = (m_dexterity > MAX_STATS ? MAX_STATS : m_dexterity);
}

void Actor::changeAsleep(int delta) {
  if (m_turnsAsleep > 0 && delta > 0) {
    m_turnsAsleep = max(m_turnsAsleep, delta);
  } else {
    m_turnsAsleep += delta;
  }
}


// ******************************
// * Mutators - Other
// ******************************

void Actor::setWeapon(Weapon* weapon) {
  m_weapon = weapon;
}

void Actor::setOverGameObject(GameObject* gameObject) {
  m_overGameObject = gameObject;
}


// ******************************
// * Combat
// ******************************

// Moves player from current cell to next cell
void Actor::move(int curRow, int curCol, int nextRow, int nextCol) {
  // Sets current cell to Space or GameObject (if Actor isOverGameObject)
  getDungeon()->updateCurrentCell(this, curRow, curCol);
  
  // Remove overGameObject if exists
  if (isOverGameObject()) {
    setOverGameObject(nullptr);
  }
  
  // Set overGameObject if exists on next cell
  if (getDungeon()->isGameObject(nextRow, nextCol)) {
    setOverGameObject((GameObject*)getDungeon()->getObject(nextRow, nextCol));
  }
  
  // Set position of this Actor
  setPosition(nextRow, nextCol);
  
  // Set next cell to this Actor
  getDungeon()->updateNextCell(this, nextRow, nextCol);
}

void Actor::attack(Actor* attacker, Actor* defender) {
  Weapon* attackerWeapon = attacker->getWeapon();
  string actionString = attacker->getName() + " " + attackerWeapon->getActionString() + " " + attackerWeapon->getName() + " at " + defender->getName() + " and ";
  
  // Determine if attacker hits
  bool hits = isHit(attacker, defender);
  
  if (hits) {
    // Calculate amount of damage
    int damagePoints = randInt(attacker->getStrength() + attackerWeapon->getDamage());
    defender->changeHP(-1 * damagePoints);
    actionString += "hits for " + to_string(damagePoints) + " damage.";
    
    if (defender->isPlayer()) {
      actionString += " Move toward " + attacker->getName() + " to attack back!";
    }
    
    getDungeon()->addAction(actionString); // WARNING: Place here for proper action order
    
    // Determine if Magic Fangs put defender to sleep
    if (attackerWeapon->getName() == "Magic Fangs" and trueWithProbability(0.2)) {
      defender->changeAsleep(randInt(5)+2);
      getDungeon()->addAction(defender->getName() + " fell asleep by the power of " + attacker->getName() + "'s Magic Fangs!");
    }
    
    // Determine if the defender is dead
    checkIsDead(defender);
  } else {
    actionString += "misses.";
    getDungeon()->addAction(actionString);
  }
}

// Determine if attacker (generally this Actor) hits defender
bool Actor::isHit(Actor* attacker, Actor* defender) {
  int attackerPoints = attacker->getDexterity() + attacker->getWeapon()->getDexBonus();
  int defenderPoints = defender->getDexterity() + defender->getArmor();
  
  if (randInt(attackerPoints)+1 >= randInt(defenderPoints)+1) {
    return true;
  }
  
  return false;
}

void Actor::checkIsDead(Actor* actor) {
  if (actor->getHP() == 0) {
    if (actor->isMonster()) {
      // If mosnter is dead, destroy monster from dungeon
      getDungeon()->destroyMonster((Monster*)actor);
    } else if (actor->isPlayer()) {
      // If player is dead, end the game
      getDungeon()->endGame("Player was defeated. Game over!");
    }
  }
}

bool Actor::checkIsAsleep() {
  if (isAsleep()) {
    getDungeon()->addAction(getName() + " is asleep and cannot move.");
    changeAsleep(-1);
    return true;
  }
  
  return false;
}
