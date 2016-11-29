//! @file
//! @brief Implementation file for the AggressorStrategy class
//!
//! Implement character (friendly or unfriendly) actions as strategy pattern
//! Each turn, a character can proceed with:
//! - move
//! - attack
//! - free action
//!
//! Must implement these ConcreteStrategies:
//! 1) a HumanPlayerStrategy that lets the user decide where to move, who to attack, and what free actions to take;
//! 2) an AggressorStrategy that make the character automatically move towards and attack the player character;
//! 3) a FriendlyStrategy that makes the character automatically move towards the character, but not attack unless attacked, in which case it adopts the AggressorStrategy.klA

#include <iostream>
#include "AggressorStrategy.h"
#include "Character.h"
#include "Map.h"
#include "Global.h"

//! Implementation of the Strategy pattern execute method. In this case, if the character is alive, they are displaced by 1 cell towards the human player. After moving, if one adjacent character is available to attack, it is done.
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::execute(Map& m, Character& c) {
  if (c.getHitPoints() > 0) {
    c.displayCharacter();
    this->moveCloserToHuman(m, c);
    this->canAttackOneAdjacentCharacter(m, c);
  }
}

//! Implementation of the moveCloserToHuman method. This method evaluates the character's current position and its distance from the human player.  Once the proper direction is established, the character is moved one tile in the general direction of the human player.
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::moveCloserToHuman(Map& m, Character& c) {
  int charPosX = c.getCurrentPositionX();
  int charPosY = c.getCurrentPositionY();
  int humanPosX = m.getHumanPosition()[0];
  int humanPosY = m.getHumanPosition()[0];
  int originalDistance = this->shortestDistanceToHuman(charPosX, charPosY, humanPosX, humanPosY);

  if (originalDistance != 0) {
    if (charPosX>0 &&
        m.getCell(charPosX-1, charPosY) == ' ' &&
        this->shortestDistanceToHuman(charPosX-1, charPosY, humanPosX, humanPosY) <= originalDistance) {
      this->moveUp(m,c);
    } else if (charPosY>0 &&
        m.getCell(charPosX, charPosY-1) == ' ' &&
        this->shortestDistanceToHuman(charPosX, charPosY-1, humanPosX, humanPosY) <= originalDistance) {
      this->moveLeft(m,c);
    } else if (charPosX<m.getMapWidth()-1 &&
        m.getCell(charPosX+1, charPosY) == ' ' &&
        this->shortestDistanceToHuman(charPosX+1, charPosY, humanPosX, humanPosY) <= originalDistance) {
      this->moveDown(m,c);
    } else if (charPosY<m.getMapLength()-1 &&
        m.getCell(charPosX, charPosY+1) == ' ' &&
        this->shortestDistanceToHuman(charPosX, charPosY+1, humanPosX, humanPosY) <= originalDistance) {
      this->moveRight(m,c);
    }
  }
}

//! Implementation of the shortestDistanceToHuman method. This method is the measure for how far away a human player is. Is it used in established the direction in which the human player is.
//! IMPORTANT TO DO FOR FINAL DELIVERABLE:
//! - Better path finding algorithm (eg. shortestDistanceToHuman method). Currently, the algorithm used does not consider obstacles such as walls, other characters, etc.
//! @param int for current character's x-coordinate
//! @param int for current character's y-coordinate
//! @param int for human player's x-coordinate
//! @param int for human player's y-coordinate
int AggressorStrategy::shortestDistanceToHuman(int charPosX, int charPosY, int humanPosX, int humanPosY) {
    return abs(charPosX-humanPosX) + abs(charPosY-humanPosY);
}

//! Implementation of the moveUp method. This changes the character's position state and the map's representation one cell upwards
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::moveUp(Map& m, Character& c) {
  vector<int> currentPosition = {c.getCurrentPositionX(), c.getCurrentPositionY()};
  vector<int> humanPosition = {m.getHumanPosition()[0], m.getHumanPosition()[1]};
  m.clearCell(currentPosition[0], currentPosition[1]);
  currentPosition[0]--;
  m.setCell(currentPosition[0], currentPosition[1], c.getTypeOnMap());
  c.setCurrentPositionX(currentPosition[0]);
}

//! Implementation of the moveDown method. This changes the character's position state and the map's representation one cell downwards.
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::moveDown(Map& m, Character& c) {
  vector<int> currentPosition = {c.getCurrentPositionX(), c.getCurrentPositionY()};
  vector<int> humanPosition = {m.getHumanPosition()[0], m.getHumanPosition()[1]};
  m.clearCell(currentPosition[0], currentPosition[1]);
  currentPosition[0]++;
  m.setCell(currentPosition[0], currentPosition[1], c.getTypeOnMap());
  c.setCurrentPositionX(currentPosition[0]);
}

//! Implementation of the moveLeft method. This changes the character's position state and the map's representation one cell to the left.
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::moveLeft(Map& m, Character& c) {
  vector<int> currentPosition = {c.getCurrentPositionX(), c.getCurrentPositionY()};
  vector<int> humanPosition = {m.getHumanPosition()[0], m.getHumanPosition()[1]};
  m.clearCell(currentPosition[0], currentPosition[1]);
  currentPosition[1]--;
  m.setCell(currentPosition[0], currentPosition[1], c.getTypeOnMap());
  c.setCurrentPositionY(int(currentPosition[1]));
}

//! Implementation of the moveRight method. This changes the character's position state and the map's representation one cell to the right.
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::moveRight(Map& m, Character& c) {
  vector<int> currentPosition = {c.getCurrentPositionX(), c.getCurrentPositionY()};
  vector<int> humanPosition = {m.getHumanPosition()[0], m.getHumanPosition()[1]};
  m.clearCell(currentPosition[0], currentPosition[1]);
  currentPosition[1]++;
  m.setCell(currentPosition[0], currentPosition[1], c.getTypeOnMap());
  c.setCurrentPositionY(currentPosition[1]);
}

//! Implementation of the canAttackOneAdjacentCharacter method. This checks all directions for adjacent cells occupied by a character. If a character is found, that character is attacked.
//! @param Reference to the map currently being played
//! @param Reference to the character that this strategy belongs to.
void AggressorStrategy::canAttackOneAdjacentCharacter(Map& m, Character& c) {
  int charPosX = c.getCurrentPositionX();
  int charPosY = c.getCurrentPositionY();
  if (charPosX>0 &&
      (m.getCell(charPosX-1, charPosY) == 'S' ||
       m.getCell(charPosX-1, charPosY) == 'C' ||
       m.getCell(charPosX-1, charPosY) == 'O')) {
    this->attackCharacterAtPosition(c,charPosX-1, charPosY);
  } else if (charPosY>0 &&
      (m.getCell(charPosX, charPosY-1) == 'S' ||
       m.getCell(charPosX, charPosY-1) == 'C' ||
       m.getCell(charPosX, charPosY-1) == 'O')) {
    this->attackCharacterAtPosition(c,charPosX, charPosY-1);
  } else if (charPosX<m.getMapWidth()-1 &&
      (m.getCell(charPosX+1, charPosY) == 'S' ||
       m.getCell(charPosX+1, charPosY) == 'C' ||
       m.getCell(charPosX+1, charPosY) == 'O')) {
    this->attackCharacterAtPosition(c,charPosX+1, charPosY);
  } else if (charPosY<m.getMapLength()-1 &&
      (m.getCell(charPosX, charPosY-1) == 'S' ||
       m.getCell(charPosX, charPosY-1) == 'C' ||
       m.getCell(charPosX, charPosY-1) == 'O')) {
    this->attackCharacterAtPosition(c,charPosX, charPosY+1);
  }
}

//! Implementation of the attackCharacterAtPositionmethod. This checks a specific cell occupied by a character. If a character is found, that character is attacked.
//! @param Reference to the character that this strategy belongs to.
//! @param int for current character's x-coordinate
//! @param int for current character's y-coordinate
void AggressorStrategy::attackCharacterAtPosition(Character& c, int charPosX, int charPosY) {
  //First need to find the character instance at position (charPosX, charPosY)
  Character* charBeingAttacked;
  for (int i = 0; i < (int)Global::gameCharacters.size(); i++) {
    if (Global::gameCharacters[i].getCurrentPositionX() == charPosX &&
        Global::gameCharacters[i].getCurrentPositionY() == charPosY) {
      charBeingAttacked = &Global::gameCharacters[i];
    }
  }

  // Now checking if character had the friendly strategy. If yes, it should now have the aggressor strategy.
  if (charBeingAttacked->getTypeOnMap() == 'C') {
    charBeingAttacked->setTypeOnMap('O');
    charBeingAttacked->setStrategy(new AggressorStrategy());
  }

  // Now remove hitpoints.
  // IMPORTANT TO IMPLEMENT FOR FINAL DELIVERABLE:
  // - Proper multiple attack.
  // - proper attack success based on dice roll and armor class.
  // - proper hitpoints reduction if attack is successful.
  int tempHP = charBeingAttacked->getHitPoints();
  charBeingAttacked->setHitPoints(tempHP-1);
}
