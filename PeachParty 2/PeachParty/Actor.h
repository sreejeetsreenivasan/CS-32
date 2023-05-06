#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world);
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
    bool still_alive() const;
    void set_alive_status(bool status);
    void update_direction(int& dir);
    virtual bool is_square() const = 0;
    virtual bool is_baddie() const = 0;
    
private:
    // To get this pointer, we can use 'this' when the StudentWorld object creates a new actor
    StudentWorld* m_world;
    bool alive;
};

class Player : public Actor
{
public:
    // When created, sprite direction is 0, walk direction is right, graph_depth = 0, size = 1
    Player(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, int player_type);
    void doSomething();
    
    // Getters
    int get_star_count() const;
    int get_coin_count() const;
    bool has_vortex() const;
    bool get_waiting_to_roll_status() const;
    bool is_square() const;
    bool is_baddie() const;
    bool on_square() const;
    bool get_pos_dir() const;
    int get_walk_dir() const;
    bool is_fork(int dir);
    bool fork_coord(int dir) const;
    bool get_dir_square_status();
    int get_ticks_to_move() const;
    
    // Setters
    void set_coin_amount(int coins);
    void set_star_amount(int stars);
    void set_vortex(bool state);
    void set_dir_square_status(bool status);
    bool update_fork_dir(int& dir);
    void set_walk_dir(int direction);
    void swap_pos_and_movement(Player* p1, Player* p2);
    
private:
    int player_1_or_2;
    int ticks_to_move;
    bool waiting_to_roll;
    int walk_direction;
    int coins;
    int star_count;
    bool has_projectile;
    bool is_on_square;
    bool pos_dir_swap;
    bool on_dir_square;
};

// BADDIES

class Baddie: public Actor
{
public:
    Baddie(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, bool state, int travel_distance, int pause_counter);
    
    // Getters
    int get_walk_direction() const;
    bool get_state() const;
    int get_pause_counter() const;
    int get_travel_distance() const;
    bool is_square() const;
    bool is_baddie() const;
    bool get_impact_status() const;
    int get_ticks_to_move() const;
    int get_squares_to_move() const;
    
    // Setters
    void set_walk_direction(int direction);
    void set_state(bool state);
    void set_pause_counter(int count);
    int set_travel_distance();
    void update_square();
    void set_impact_status(bool status);
    void set_ticks_to_move(int ticks);
    void set_squares_to_move(int squares);
    bool update_fork_direction(int& direction);
    
private:
    int walk_direction;
    bool state;
    int travel_distance;
    int pause_counter;
    bool impacted;
    int ticks_to_move;
    int squares_to_move;
};


class Bowser: public Baddie
{
public:
    Bowser(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, bool state, int travel_distance, int pause_counter);
    void doSomething();
};

class Boo: public Baddie
{
public:
    Boo(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, bool state, int travel_distance, int pause_counter);
    void doSomething();
};

// SQUARES

class Square : public Actor
{
public:
    Square(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world);
    bool is_square() const;
    bool is_baddie() const;
    bool get_has_given() const;
    void set_has_given(int given);
private:
    bool has_given;
};

class Coin_Square : public Square
{
public:
    // When created, sprite direction is 0, graph_depth = 1, size = 1
    Coin_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int coin_grant, StudentWorld* world);
    void doSomething();
    int coin_amount() const;
    void coin_method(Player* player);
    
private:
    int coin_grant;
};

class Star_Square : public Coin_Square
{
public:
    Star_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int coin_grant, int star_grant, StudentWorld* world);
    void doSomething();
    void star_method(Player* player);
    
private:
    int star_grant;
};

class Event_Square : public Square
{
public:
    Event_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world);
    void doSomething();
    int get_event() const;
};

class Bank_Square : public Square
{
public:
    Bank_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int current_amount, StudentWorld* world);
    void doSomething();
    int get_amount() const;
    
private:
    int amount;
};


class Direction_Square : public Square
{
public:
    Direction_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int forcing_direction, StudentWorld* world);
    void doSomething();
    
private:
    int direction;
};


class Dropping_Square : public Square
{
public:
    Dropping_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world);
    void doSomething();
};

// OBJECTS

class Vortex: public Actor
{
public:
    Vortex(int im_id, int start_x, int start_y, int sprite_direction, int depth, int size, StudentWorld* world, int object_direction);
    void doSomething();
    bool is_square() const;
    bool is_baddie() const;
    
    // Getters
    int get_object_direction() const;
    
private:
    int object_direction;
};

#endif // ACTOR_H_
