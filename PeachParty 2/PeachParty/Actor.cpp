#include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include <sstream>

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world)
:GraphObject(im_id, start_x, start_y, direction, depth, size)
{
    m_world = world;
    alive = true;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

bool Actor::still_alive() const
{
    return alive;
}

void Actor::set_alive_status(bool status)
{
    alive = status;
}

void Actor::update_direction(int& dir)
{
    // setDirection changes the angle of the sprite, set_walk_dir the direction of the object
    // Each grid square is 16x16 pixels
    if (dir == right)
    {
        
        // If player cannot move right anymore, check to see if they can move either up or down
        // Favor up over down when both options are available
        if (this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) == Board::empty || (this->getX() + BOARD_WIDTH) / BOARD_WIDTH > BOARD_WIDTH-1)
        {
            
            if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
                dir = up;
            else if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
                dir = down;
            else
                dir = left;
        }
    }
    else if (dir == left)
    {
        // If player cannot move left anymore, check to see if they can move either up or down
        // Favor up over down when both options are available
        if (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) == Board::empty || (this->getX() - BOARD_WIDTH) / BOARD_WIDTH < 0)
        {
            if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
                dir = up;
            else if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
                dir = down;
            else
                dir = right;
        }
    }
    else if (dir == up)
    {
        // If player cannot move up anymore, check to see if they can move either right or left
        // Favor right over left when both options are available
        if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) == Board::empty || (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT > BOARD_HEIGHT-1)
        {
            if (this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
                dir = right;
            else if (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
                dir = left;
            else
                dir = down;
        }
    }
    else if (dir == down)
    {
        // If player cannot move down anymore, check to see if they can move either right or left
        // Favor right over left when both options are available
        if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) == Board::empty || (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT < 0)
        {
            if (this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
                dir = right;
            else if (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
                dir = left;
            else
                dir = up;
        }
    }
}


// PLAYERS

Player::Player(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, int player_type)
:Actor(im_id, start_x, start_y, direction, depth, size, world)
{
    ticks_to_move = 0;
    waiting_to_roll = true;
    player_1_or_2 = player_type;
    walk_direction = right;
    coins = 0;
    star_count = 0;
    has_projectile = false;
    is_on_square = true;
    pos_dir_swap = false;
    on_dir_square = false;
}

int Player::get_walk_dir() const
{
    return walk_direction;
}

bool Player::has_vortex() const
{
    return has_projectile;
}

void Player::set_walk_dir(int direction)
{
    walk_direction = direction;
}

void Player::set_coin_amount(int coins)
{
    this->coins = coins;
}

void Player::set_star_amount(int stars)
{
    this->star_count = stars;
}

int Player::get_star_count() const
{
    return star_count;
}

int Player::get_coin_count() const
{
    return coins;
}

bool Player::get_waiting_to_roll_status() const
{
    return waiting_to_roll;
}

bool Player::is_square() const
{
    return false;
}

bool Player::is_baddie() const
{
    return false;
}

void Player::set_vortex(bool state)
{
    has_projectile = state;
}

bool Player::on_square() const
{
    return is_on_square;
}

bool Player::get_pos_dir() const
{
    return pos_dir_swap;
}

bool Player::get_dir_square_status()
{
    return on_dir_square;
}

void Player::set_dir_square_status(bool status)
{
    on_dir_square = status;
}

void Player::swap_pos_and_movement(Player* p1, Player* p2)
{
    int p1_x = p1->getX();
    int p1_y = p1->getY();
    int p1_ticks = p1->ticks_to_move;
    int p1_dir = p1->get_walk_dir();
    int p1_sprite_dir = p1->getDirection();
    bool p1_roll_state = p1->waiting_to_roll;
    
    p1->moveTo(p2->getX(), p2->getY());
    p1->ticks_to_move = p2->ticks_to_move;
    p1->set_walk_dir(p2->get_walk_dir());
    p1->setDirection(p2->getDirection());
    p1->waiting_to_roll = p2->waiting_to_roll;
    
    p2->moveTo(p1_x, p1_y);
    p2->ticks_to_move = p1_ticks;
    p2->set_walk_dir(p1_dir);
    p2->setDirection(p1_sprite_dir);
    p2->waiting_to_roll = p1_roll_state;
    
    p1->pos_dir_swap = true;
    p2->pos_dir_swap = true;
}

bool Player::update_fork_dir(int& dir)
{
    int latestAction = this->getWorld()->getAction(player_1_or_2);
    if (latestAction == ACTION_NONE)
        return false;
    if (dir == right)
    {
        if (latestAction == ACTION_UP && (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1))
        {
            dir = up;
            return true;
        }
        else if (latestAction == ACTION_DOWN && this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
        {
            dir = down;
            return true;
        }
        else if (latestAction == ACTION_RIGHT && this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
        {
            dir = right;
            return true;
        }
    }
    else if (dir == left)
    {
        if (latestAction == ACTION_UP && (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1))
        {
            dir = up;
            return true;
        }
        else if (latestAction == ACTION_DOWN && this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
        {
            dir = down;
            return true;
        }
        else if (latestAction == ACTION_LEFT && this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
        {
            dir = left;
            return true;
        }
    }
    else if (dir == up)
    {
        if (latestAction == ACTION_RIGHT && this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
        {
            dir = right;
            return true;
        }
        else if (latestAction == ACTION_LEFT && this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
        {
            dir = left;
            return true;
        }
        else if (latestAction == ACTION_UP && (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1))
        {
            dir = up;
            return true;
        }
    }
    else if (dir == down)
    {
        if (latestAction == ACTION_RIGHT && this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
        {
            dir = right;
            return true;
        }
        else if (latestAction == ACTION_LEFT && this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1)
        {
            dir = left;
            return true;
        }
        else if (latestAction == ACTION_DOWN && this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1)
        {
            dir = down;
            return true;
        }
    }
    return false;
}

bool Player::fork_coord(int dir) const
{
    int open_squares = 0;
    
    if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty)
        open_squares += 1;
    if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty)
        open_squares += 1;
    if (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty)
        open_squares += 1;
    if (this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty)
        open_squares += 1;
    
    if (open_squares >= 3)
        return true;
    else
        return false;
}

int Player::get_ticks_to_move() const
{
    return ticks_to_move;
}

void Player::doSomething()
{
    // Bottom-left pixel is (0,0) and top-right pixel is (255,255)
    // X increases to the right and Y increases upwards
    // setDirection changes the direction of the sprite image
    if (waiting_to_roll)
    {
        is_on_square = true;
        
        // Check if Player has invalid direction
        int direction = this->get_walk_dir();
        this->update_direction(direction);
        this->set_walk_dir(direction);
        
        if (get_walk_dir() == left)
            setDirection(180);
        else
            setDirection(0);
        
        int action = this->getWorld()->getAction(player_1_or_2);
        if (action == ACTION_ROLL)
        {
            
            int die_roll = randInt(1, 10);
            ticks_to_move = die_roll * 8;
            waiting_to_roll = false;
            pos_dir_swap = false;
        }
        else if (action == ACTION_FIRE && this->has_vortex())
        {
            if (get_walk_dir() == right)
                this->getWorld()->create_proj(getX() + BOARD_WIDTH, getY(), get_walk_dir());
            else if (get_walk_dir() == left)
                this->getWorld()->create_proj(getX() - BOARD_WIDTH, getY(), get_walk_dir());
            else if (get_walk_dir() == up)
                this->getWorld()->create_proj(getX(), getY() + BOARD_HEIGHT, get_walk_dir());
            else if (get_walk_dir() == down)
                this->getWorld()->create_proj(getX(), getY() - BOARD_HEIGHT, get_walk_dir());
            this->getWorld()->playSound(SOUND_PLAYER_FIRE);
            this->set_vortex(false);
            return;
        }
        else
            return;
    }
    
    if (!waiting_to_roll && getX() % BOARD_WIDTH == 0 && getY() % BOARD_HEIGHT == 0)
    {
        is_on_square = true;
        int dir = this->get_walk_dir();
        if (!on_dir_square && fork_coord(dir))
        {
           if (!update_fork_dir(dir))
               return;
        }
        else
            update_direction(dir);
        this->set_walk_dir(dir);
    }
    
    //Code to update the direction the sprite/image faces
    if (this->get_walk_dir() == left)
        setDirection(180);
    else
        setDirection(0);
        
    switch (this->get_walk_dir())
    {
        case right: moveTo(getX() + 2, getY()); break;
        case left: moveTo(getX() - 2, getY()); break;
        case up: moveTo(getX(), getY() + 2); break;
        case down: moveTo(getX(), getY() - 2); break;
        default: return;
    }
    
    is_on_square = false;
    on_dir_square = false;
    
    ticks_to_move -= 1;
    if (ticks_to_move == 0)
    {
        waiting_to_roll = true;
        is_on_square = true;
    }
}


// SQUARES

Square::Square(int im_id, int start_x, int start_y, int dir, int depth, int size, StudentWorld* world)
:Actor(im_id, start_x, start_y, dir, depth, size, world) // depth = 1
{
    this->has_given = false;
}

bool Square::get_has_given() const
{
    return has_given;
}

void Square::set_has_given(int given)
{
    has_given = given;
}

bool Square::is_square() const
{
    return true;
}

bool Square::is_baddie() const
{
    return false;
}

Coin_Square::Coin_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int coin_grant, StudentWorld* world)
:Square(im_id, start_x, start_y, direction, depth, size, world)
{
    this->coin_grant = coin_grant;
}

int Coin_Square::coin_amount() const
{
    return coin_grant;
}

void Coin_Square::coin_method(Player* player)
{
    player->set_coin_amount(player->get_coin_count() + this->coin_amount());
}

void Coin_Square::doSomething()
{
    if (!(this->still_alive()))
        return;
    // A player needs to get +/-3 coins when they land on a coin square
    
    Player* player = this->getWorld()->player_square_pos(this);
    if (player != nullptr && player->on_square() && !get_has_given())
    {
        int current_coins = player->get_coin_count();
        this->coin_method(player);
        if (player->get_coin_count() < 0)
            player->set_coin_amount(0);
        else if (player->get_coin_count() < current_coins)
            this->getWorld()->playSound(SOUND_TAKE_COIN);
        else
            this->getWorld()->playSound(SOUND_GIVE_COIN);
        this->set_has_given(true);
    }
}


Star_Square::Star_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int coin_grant, int star_grant, StudentWorld* world)
:Coin_Square(im_id, start_x, start_y, direction, depth, size, coin_grant, world)
{
    this->star_grant = star_grant;
}

void Star_Square::star_method(Player* player)
{
    player->set_star_amount(player->get_star_count() + 1);
}

void Star_Square::doSomething()
{
    Player* player = this->getWorld()->player_square_pos(this);
    if (player != nullptr && player->on_square() &&!get_has_given())
    {
        if (player->get_coin_count() < 20)
            return;
        else
        {
            this->coin_method(player);
            this->star_method(player);
            this->getWorld()->playSound(SOUND_GIVE_STAR);
        }
        this->set_has_given(true);
    }
}


Event_Square::Event_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world)
:Square(im_id, start_x, start_y, direction, depth, size, world)
{}

void Event_Square::doSomething()
{
    Player* player = this->getWorld()->player_square_pos(this);
    if (player != nullptr && player->on_square() &&!get_has_given())
    {
        // Randomly chooses what action to perform
        // If Option #1: Teleport player
        int event = randInt(1, 3);
        if (event == 1)
        {
            this->getWorld()->teleport(player);
            this->getWorld()->playSound(SOUND_PLAYER_TELEPORT);
        }
        // If Option #2: Swap position and movement with another player
        else if (event == 2)
        {
            if (!player->get_pos_dir() == true)
            {
                this->getWorld()->swap_pos_and_dir();
                this->getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
        }
        else if (event == 3)
        {
            if (!player->has_vortex())
            {
                player->set_vortex(true);
                //this->getWorld()->create_proj(player->getX() + SPRITE_WIDTH, player->getY() + SPRITE_HEIGHT, player->get_walk_dir());
                this->getWorld()->playSound(SOUND_GIVE_VORTEX);
            }
        }
        this->set_has_given(true);
    }
}

Bank_Square::Bank_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int current_amount, StudentWorld* world)
:Square(im_id, start_x, start_y, direction, depth, size, world)
{
    this->amount = current_amount;
}

void Bank_Square::doSomething()
{
    Player* player = this->getWorld()->player_square_pos(this);
    if (player != nullptr && player->on_square() && !get_has_given())
    {
        int central_amount = this->getWorld()->get_central_bank_amount();
        player->set_coin_amount(player->get_coin_count() + central_amount);
        this->getWorld()->set_central_bank_amount(0);
        this->getWorld()->playSound(SOUND_WITHDRAW_BANK);
        this->set_has_given(true);
    }
    else if (player != nullptr && !player->on_square() &&!get_has_given())
    {
        int player_amount = player->get_coin_count();
        if (player_amount >= 5)
        {
            player->set_coin_amount(player_amount - 5);
            this->getWorld()->set_central_bank_amount(this->getWorld()->get_central_bank_amount() + 5);
        }
        else
        {
            player->set_coin_amount(0);
            this->getWorld()->set_central_bank_amount(this->getWorld()->get_central_bank_amount() + player_amount);
        }
        this->getWorld()->playSound(SOUND_DEPOSIT_BANK);
        this->set_has_given(true);
    }
}

Direction_Square::Direction_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, int forcing_direction, StudentWorld* world)
:Square(im_id, start_x, start_y, direction, depth, size, world)
{
    this->direction = forcing_direction;
}

void Direction_Square::doSomething()
{
    Player* player = this->getWorld()->player_square_pos(this);
    if (player != nullptr && player->on_square())
    {
        player->set_walk_dir(direction);
        player->set_dir_square_status(true);
        if (direction == left)
            player->setDirection(180);
        else
            player->setDirection(0);
    }
}

Dropping_Square::Dropping_Square(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world)
:Square(im_id, start_x, start_y, direction, depth, size, world)
{}

void Dropping_Square::doSomething()
{
    Player* player = this->getWorld()->player_square_pos(this);
    if (player != nullptr && player->on_square() &&!get_has_given())
    {
        int choice = randInt(1, 2);
        if (choice == 1)
        {
            if (player->get_coin_count() < 10)
                player->set_coin_amount(0);
            else
                player->set_coin_amount(player->get_coin_count() - 10);
            
        }
        else if (choice == 2)
        {
            if (player->get_star_count() >= 1)
                player->set_star_amount(player->get_star_count() - 1);
        }
        
        this->getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
        this->set_has_given(true);
    }
}

// BADDIE

Baddie::Baddie(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, bool state, int travel_distance, int pause_counter)
:Actor(im_id, start_x, start_y, direction, depth, size, world)
{
    this->walk_direction = right;
    this->state = state;
    this->travel_distance = travel_distance;
    this->pause_counter = pause_counter;
    this->impacted = false;
    this->ticks_to_move = 0;
    this->squares_to_move = 0;
}

bool Baddie::is_square() const
{
    return false;
}

bool Baddie::is_baddie() const
{
    return true;
}

int Baddie::get_walk_direction() const
{
    return walk_direction;
}

bool Baddie::get_state() const
{
    return state;
}

int Baddie::get_ticks_to_move() const
{
    return ticks_to_move;
}

int Baddie::get_pause_counter() const
{
    return pause_counter;
}

int Baddie::get_travel_distance() const
{
    return travel_distance;
}

void Baddie::set_pause_counter(int count)
{
    pause_counter = count;
}

void Baddie::set_walk_direction(int direction)
{
    walk_direction = direction;
}

void Baddie::set_state(bool state)
{
    this->state = state;
}

bool Baddie::get_impact_status() const
{
    return impacted;
}

void Baddie::set_impact_status(bool status)
{
    impacted = true;
}

void Baddie::set_ticks_to_move(int ticks)
{
    ticks_to_move = ticks;
}

void Baddie::set_squares_to_move(int squares)
{
    squares_to_move = squares;
}

int Baddie::get_squares_to_move() const
{
    return squares_to_move;
}

bool Baddie::update_fork_direction(int& direction)
{
    // setDirection changes the angle of the sprite, set_walk_dir the direction of the object
    // Each grid square is 16x16 pixels
    if (direction == right)
    {
        
        // If player cannot move right anymore, check to see if they can move either up or down
        if (this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) == Board::empty || (this->getX() + BOARD_WIDTH) / BOARD_WIDTH > BOARD_WIDTH-1)
        {
            // If the Baddie can move both up OR down (expression evaluates to true when at a fork)
            if ((this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1) && (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1))
                {
                    int choice = randInt(1, 2);
                    if (choice == 1)
                        direction = up;
                    else
                        direction = down;
                    return true;
                }
        }
    }
    else if (direction == left)
    {
        // If player cannot move left anymore, check to see if they can move either up or down
        if (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) == Board::empty || (this->getX() - BOARD_WIDTH) / BOARD_WIDTH < 0)
        {
            // If the Baddie can move both up OR down (expression evaluates to true when at a fork)
            if ((this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1) && (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) != Board::empty && (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT <= BOARD_HEIGHT-1))
                {
                    int choice = randInt(1, 2);
                    if (choice == 1)
                        direction = up;
                    else
                        direction = down;
                    return true;
                }
        }
    }
    else if (direction == up)
    {
        // If player cannot move up anymore, check to see if they can move either right or left
        if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() + BOARD_HEIGHT) / BOARD_HEIGHT) == Board::empty || (this->getY() + BOARD_HEIGHT) / BOARD_HEIGHT > BOARD_HEIGHT-1)
        {
            if ((this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1) && (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1))
            {
                int choice = randInt(1, 2);
                if (choice == 1)
                    direction = left;
                else
                    direction = right;
            }
        }
    }
    else if (direction == down)
    {
        // If player cannot move down anymore, check to see if they can move either right or left
        // Favor right over left when both options are available
        if (this->getWorld()->current_board().getContentsOf(getX() / BOARD_WIDTH, (getY() - BOARD_HEIGHT) / BOARD_HEIGHT) == Board::empty || (this->getY() - BOARD_HEIGHT) / BOARD_HEIGHT < 0)
        {
            if ((this->getWorld()->current_board().getContentsOf((getX() + BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() + BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1) && (this->getWorld()->current_board().getContentsOf((getX() - BOARD_WIDTH) / BOARD_WIDTH, getY() / BOARD_HEIGHT) != Board::empty && (this->getX() - BOARD_WIDTH) / BOARD_WIDTH <= BOARD_WIDTH-1))
            {
                int choice = randInt(1, 2);
                if (choice == 1)
                    direction = left;
                else
                    direction = right;
                return true;
            }
        }
    }
    return false;
}

// Bowser subclass
Bowser::Bowser(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, bool state, int travel_distance, int pause_counter)
:Baddie(im_id, start_x, start_y, direction, depth, size, world, state, travel_distance, pause_counter)
{}

void Bowser::doSomething()
{
    if (get_ticks_to_move() == 4)
        this->getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    // If Bowser in 'paused' state
    if (!this->get_state())
    {
        bool player_activated = false;
        unsigned long size = this->getWorld()->player_size();
        int x = 0; int y = 0; // Coordinates to test against the player objects
        
        for (int i = 0; i < size; i++)
        {
            if (player_activated == true)
                break;
            
            this->getWorld()->player_x_and_y(i, x, y);
            
            if (x == this->getX() && y == this->getY() && this->getWorld()->pointer_to_actor(i)->get_waiting_to_roll_status() == true)
            {
                int chance = randInt(1, 2);
                if (chance == 1)
                    continue;
                else
                {
                    // Take all coins from player and play sound
                    this->getWorld()->pointer_to_actor(i)->set_coin_amount(0);
                    this->getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                    player_activated = true;
                }
            }
        }
        
        this->set_pause_counter(get_pause_counter() - 1); // Decrement pause counter
        if (this->get_pause_counter() == 0)
        {
            this->set_squares_to_move(randInt(1, 10));
            this->set_ticks_to_move(get_squares_to_move() * 8);
            
            int dir = randInt(1, 4);
            int direction;
            
            // Pick random direction for Bowser to move
            if (dir == 1)
                direction = right;
            else if (dir == 2)
                direction = left;
            else if (dir == 3)
                direction = up;
            else
                direction = down;
            
            update_direction(direction);
            this->set_walk_direction(direction);
            
            if (this->get_walk_direction() == left)
                setDirection(180);
            else
                setDirection(0);
            
            this->set_state(true); // Set Bowser to 'walking' state
        }
    }
    // If Bowser in 'walking' state
    if (this->get_state())
    {
        // Use update_direction function to update the direction of Bowser's movement when at a fork or not
        if (getX() % BOARD_WIDTH == 0 && getY() % BOARD_HEIGHT == 0)
        {
            int dir = this->get_walk_direction();
            update_direction(dir);
            this->set_walk_direction(dir);
        }
        
        if (this->get_walk_direction() == left)
            setDirection(180);
        else
            setDirection(0);
        
        switch (this->get_walk_direction())
        {
            case right: moveTo(getX() + 2, getY()); break;
            case left: moveTo(getX() - 2, getY()); break;
            case up: moveTo(getX(), getY() + 2); break;
            case down: moveTo(getX(), getY() - 2); break;
            default: return;
        }
        
        this->set_ticks_to_move(get_ticks_to_move() - 1);
        if (get_ticks_to_move() == 0)
        {
            this->set_state(false);
            this->set_pause_counter(180);
            int chance = randInt(1, 4);
            if (chance == 2) // 25% chance that Bowser will insert a dropping square beneath him
            {
                this->getWorld()->update_square(getX(), getY());
                this->getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
            }
        }
    }
}


// Boo subclass
Boo::Boo(int im_id, int start_x, int start_y, int direction, int depth, int size, StudentWorld* world, bool state, int travel_distance, int pause_counter)
:Baddie(im_id, start_x, start_y, direction, depth, size, world, state, travel_distance, pause_counter)
{}

void Boo::doSomething()
{
    // If Boo in 'paused' state
    if (!this->get_state())
    {
        // Test to see if Boo and a Player are on the same square
        bool player_activated = false;
        unsigned long size = this->getWorld()->player_size();
        int x = 0; int y = 0; // Coordinates to test against the player objects
        
        for (int i = 0; i < size; i++)
        {
            if (player_activated == true)
                break;
            
            Player* player_1 = this->getWorld()->player_x_and_y(i, x, y);
            
            if (x == this->getX() && y == this->getY() && this->getWorld()->pointer_to_actor(i)->get_waiting_to_roll_status() == true)
            {
                player_activated = true;
                Player* player_2 = nullptr;
                int choice = randInt(1, 2);
                // Boo will swap coins between players
                if (choice == 1)
                {
                    if (i == 0)
                        player_2 = this->getWorld()->pointer_to_actor(1);
                    else
                        player_2 = this->getWorld()->pointer_to_actor(0);
                    this->getWorld()->swap_coins(player_1, player_2);
                }
                // Boo will swap stars between players
                else
                {
                    if (i == 0)
                        player_2 = this->getWorld()->pointer_to_actor(1);
                    else
                        player_2 = this->getWorld()->pointer_to_actor(0);
                    this->getWorld()->swap_stars(player_1, player_2);
                }
                
                this->getWorld()->playSound(SOUND_BOO_ACTIVATE);
            }
        }
        
        this->set_pause_counter(get_pause_counter() - 1); // Decrement pause counter
        if (get_pause_counter() == 0)
        {
            this->set_squares_to_move(randInt(1, 3));
            this->set_ticks_to_move(get_squares_to_move() * 8);
            
            int dir = randInt(1, 4);
            int direction;
            
            // Pick random direction for Boo to move
            if (dir == 1)
                direction = right;
            else if (dir == 2)
                direction = left;
            else if (dir == 3)
                direction = up;
            else
                direction = down;
            
            update_direction(direction);
            this->set_walk_direction(direction);
            
            if (this->get_walk_direction() == left)
                setDirection(180);
            else
                setDirection(0);
            
            this->set_state(true); // Set Boo to 'walking' state
        }
    }
    
    if (this->get_state())
    {
        // If on top of a square
        if (getX() % BOARD_WIDTH == 0 && getY() % BOARD_HEIGHT == 0)
        {
            int dir = this->get_walk_direction();
            // Boo should choose a random direction if at a fork
            // If walk_dir = right/left, then fork_dir = up/down
            // If walk_dir = up/down, then fork_dir = right/left
            
            // If Boo is at a fork (function returns true if at fork)
            if (this->update_fork_direction(dir))
                this->set_walk_direction(dir);
            else
            {
                this->update_direction(dir);
                this->set_walk_direction(dir);
            }
        }
        
        if (this->get_walk_direction() == left)
            setDirection(180);
        else
            setDirection(0);
        
        switch (this->get_walk_direction())
        {
            case right: moveTo(getX() + 2, getY()); break;
            case left: moveTo(getX() - 2, getY()); break;
            case up: moveTo(getX(), getY() + 2); break;
            case down: moveTo(getX(), getY() - 2); break;
            default: return;
        }
        this->set_ticks_to_move(get_ticks_to_move() - 1);
        if (get_ticks_to_move() == 0)
        {
            this->set_state(false);
            this->set_pause_counter(180);
        }
    }
}

// OBJECTS

Vortex::Vortex(int im_id, int start_x, int start_y, int sprite_direction, int depth, int size, StudentWorld* world, int object_direction)
:Actor(im_id, start_x, start_y, sprite_direction, depth, size, world)
{
    this->object_direction = object_direction;
    this->set_alive_status(true); // Member of Actor superclass
}

bool Vortex::is_square() const
{
    return false;
}

bool Vortex::is_baddie() const
{
    return false;
}

int Vortex::get_object_direction() const
{
    return object_direction;
}

void Vortex::doSomething()
{
    if (!still_alive()) // If Vortex is not in the 'active' state
        return;
    switch (this->get_object_direction())
    {
        case right: moveTo(getX() + 2, getY()); break;
        case left: moveTo(getX() - 2, getY()); break;
        case up: moveTo(getX(), getY() + 2); break;
        case down: moveTo(getX(), getY() - 2); break;
        default: return;
    }
    
    if (this->getX() < 0 || this->getX() >= VIEW_WIDTH || this->getY() < 0 || this->getY() >= VIEW_HEIGHT)
        this->set_alive_status(false);
    
    // Check if Vortex has impacted any actor objects
    /* We call a StudentWorld method to see if any actors have a set of coordinates (x, y) which overlap with
     the vortex's getX() and getY()*/
    
    if (this->getWorld()->check_overlap(this))
    {
        this->set_alive_status(false);
        this->getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}
