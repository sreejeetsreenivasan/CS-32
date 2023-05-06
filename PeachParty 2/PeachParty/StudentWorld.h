#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

// Needs to keep track of board and all actors

/* init()
 Must Load up board data file using Board class, create Coin Square and Peach objects at right locations, insert into m_actors vector (make sure to have coin square at same square as Peach)*/

/* move()
 Must ask Peach and Coin Squares to do something*/

/* cleanUp()
 Frees dynamically allocated data during calls to init() or move() (destructor can call cleanUp())*/

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  Board current_board() const;
  unsigned long player_size() const;
  Player* pointer_to_actor(int index) const;
  Player* player_x_and_y(int index, int& x, int& y);
  void update_square(int x, int y);
  Actor* teleport(Actor* player);
  void swap_coins(Player* player_1, Player* player_2);
  void swap_stars(Player* player_1, Player* player_2);
  void swap_pos_and_dir();
  bool check_overlap(Vortex* vortex);
  void create_proj(int x, int y, int object_dir);
  Player* player_square_pos(Square* square);
  int get_central_bank_amount() const;
  void set_central_bank_amount(int amount);
    
private:
    Board m_board;
    std::vector<Actor*> m_actors; //  All actors besides players in a single stl container
    std::vector<Player*> m_players; // Vector to store Peach and Yoshi objects
    int central_bank_amount;
};

#endif // STUDENTWORLD_H_
