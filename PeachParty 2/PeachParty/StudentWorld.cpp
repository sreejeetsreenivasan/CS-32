#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

Board StudentWorld::current_board() const
{
    return m_board;
}

unsigned long StudentWorld::player_size() const
{
    return m_players.size();
}

int StudentWorld::get_central_bank_amount() const
{
    return central_bank_amount;
}

void StudentWorld::set_central_bank_amount(int amount)
{
    central_bank_amount = amount;
}

// Returns a pointer to a single item in the m_players collection
Player* StudentWorld::pointer_to_actor(int index) const
{
    if (index >= m_players.size() || m_players.empty())
        return nullptr;
    else
        return m_players[index];
}

Player* StudentWorld::player_x_and_y(int index, int& x, int& y)
{
    if (index >= m_players.size())
        return nullptr;
    else
    {
        x = m_players[index]->getX();
        y = m_players[index]->getY();
        return m_players[index];
    }
}

void StudentWorld::update_square(int x, int y)
{
    for (unsigned long i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->getX() == x && m_actors[i]->getY() == y && m_actors[i]->is_square())
        {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
            
            Dropping_Square* dropping_square = new Dropping_Square(IID_DROPPING_SQUARE, x, y, 0, 1, 1, this);
            m_actors.push_back(dropping_square);
        }
    }
}

void StudentWorld::create_proj(int x, int y, int object_dir)
{
    Vortex* vortex = new Vortex(IID_VORTEX, x, y, 0, 0, 1, this, object_dir);
    m_actors.push_back(vortex);
}

Actor* StudentWorld::teleport(Actor* player)
{
    for (unsigned long i = 0; i < m_actors.size(); i++)
    {
        if (player == m_actors[i])
        {
            int x = randInt(0, 16); int y = randInt(0, 16);
            while (m_board.getContentsOf(x, y) == Board::empty)
            {
                x = randInt(0, 16);
                y = randInt(0, 16);
            }
            m_actors[i]->moveTo(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
            return m_actors[i];
        }
    }
    return nullptr;
}

void StudentWorld::swap_coins(Player* player_1, Player* player_2)
{
    int p2_coins = player_2->get_coin_count();
    player_2->set_coin_amount(player_1->get_coin_count());
    player_1->set_coin_amount(p2_coins);
}

void StudentWorld::swap_stars(Player* player_1, Player* player_2)
{
    int p2_stars = player_2->get_star_count();
    player_2->set_star_amount(player_1->get_star_count());
    player_1->set_star_amount(p2_stars);
}


void StudentWorld::swap_pos_and_dir()
{
    Player* p1 = pointer_to_actor(0);
    Player* p2 = pointer_to_actor(1);
    
    if (p1 != nullptr && p2 != nullptr)
        p1->swap_pos_and_movement(p1, p2);
}

bool StudentWorld::check_overlap(Vortex* vortex)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if ((abs(m_actors[i]->getX() - vortex->getX()) <= SPRITE_WIDTH && abs(m_actors[i]->getY() - vortex->getY()) <= SPRITE_HEIGHT) && m_actors[i]->is_baddie())
        {
            teleport(m_actors[i]);
            return true;
        }
    }
    return false;
}

Player* StudentWorld::player_square_pos(Square* square)
{
    for (int i = 0; i < m_players.size(); i++)
    {
        if (square->getX() == m_players[i]->getX() && square->getY() == m_players[i]->getY())
            return pointer_to_actor(i);
    }
    return nullptr;
}


int StudentWorld::init()
{
    int board_num = getBoardNumber();
    string path = assetPath(); // Get path to directory with board/img/sound files
    
    switch(board_num)
    {
        case 1: path += "board01.txt"; break;
        case 2: path += "board02.txt"; break;
        case 3: path += "board03.txt"; break;
        case 4: path += "board04.txt"; break;
        case 5: path += "board05.txt"; break;
        case 6: path += "board06.txt"; break;
        case 7: path += "board07.txt"; break;
        case 8: path += "board08.txt"; break;
        case 9: path += "board09.txt"; break;
        default: return GWSTATUS_BOARD_ERROR;
    }
    
    Board::LoadResult result = m_board.loadBoard(path);
    if (!(result == Board::load_success)) // Check to see if board loaded properly
        return GWSTATUS_BOARD_ERROR;
    
    for (int gy = 0; gy < BOARD_HEIGHT; gy++)
    {
        for (int gx = 0; gx < BOARD_WIDTH; gx++)
        {
            if (m_board.getContentsOf(gx, gy) == Board::blue_coin_square)
            {
                // Sprite dir = 0, depth = 1, size = 1
                Coin_Square* coin_square = new Coin_Square(IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, 3, this);
                m_actors.push_back(coin_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::player)
            {
                // Sprite dir = 0, depth = 0, size = 1, player_type for Peach = 1, Yoshi = 2
                Player* peach = new Player(IID_PEACH, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 0, 1, this, 1);
                m_players.push_back(peach);
                
                
                Player* yoshi = new Player(IID_YOSHI, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 0, 1, this, 2);
                m_players.push_back(yoshi);
                
                Coin_Square* coin_square = new Coin_Square(IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, 3, this);
                m_actors.push_back(coin_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::red_coin_square)
            {
                // coin_grant = -3;
                Coin_Square* coin_square = new Coin_Square(IID_RED_COIN_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, -3, this);
                m_actors.push_back(coin_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::star_square)
            {
                // coin_grant = -20, star_grant = 1
                Star_Square* star_square = new Star_Square(IID_STAR_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, -20, 1, this);
                m_actors.push_back(star_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::event_square)
            {
                // 1 = teleport, 2 = position swap, 3 = vortex projectile
                Event_Square* event_square = new Event_Square(IID_EVENT_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, this);
                m_actors.push_back(event_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::bank_square)
            {
                Bank_Square* bank_square = new Bank_Square(IID_BANK_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, 0, this);
                m_actors.push_back(bank_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::up_dir_square)
            {
                Direction_Square* up_dir_square = new Direction_Square(IID_DIR_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 90, 1, 1, 90, this);
                m_actors.push_back(up_dir_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::down_dir_square)
            {
                Direction_Square* down_dir_square = new Direction_Square(IID_DIR_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 270, 1, 1, 270, this);
                m_actors.push_back(down_dir_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::left_dir_square)
            {
                Direction_Square* left_dir_square = new Direction_Square(IID_DIR_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 180, 1, 1, 180, this);
                m_actors.push_back(left_dir_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::right_dir_square)
            {
                Direction_Square* right_dir_square = new Direction_Square(IID_DIR_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, 0, this);
                m_actors.push_back(right_dir_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::bowser)
            {
                // state = paused (false), travel_dir = 0 pixels, pause_counter = 180
                Bowser* bowser = new Bowser(IID_BOWSER, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 0, 1, this, false, 0, 180);
                m_actors.push_back(bowser);
                
                Coin_Square* coin_square = new Coin_Square(IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, 3, this);
                m_actors.push_back(coin_square);
            }
            
            else if (m_board.getContentsOf(gx, gy) == Board::boo)
            {
                // state = paused (false), travel_dir = 0 pixels, pause_counter = 180
                Boo* boo = new Boo(IID_BOO, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 0, 1, this, false, 0, 180);
                m_actors.push_back(boo);
                
                Coin_Square* coin_square = new Coin_Square(IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * gx, SPRITE_HEIGHT * gy, 0, 1, 1, 3, this);
                m_actors.push_back(coin_square);
            }
        }
    }
    startCountdownTimer(99);
    setGameStatText("Game started");
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    
    ostringstream oss;
    Player* p1 = pointer_to_actor(0);
    Player* p2 = pointer_to_actor(1);
    std::string vor1 = "";
    std::string vor2 = "";
    
    if (p1->has_vortex())
        vor1 = " VOR";
    if (p2->has_vortex())
        vor2 = " VOR";
    
    oss << "P1 Roll: " << (p1->get_ticks_to_move()) / 8 << " Stars: " << p1->get_star_count() << " $$: " << p1->get_coin_count() << vor1 << " | " << "Time: " << timeRemaining() << " | " << "Bank: " << get_central_bank_amount() << " | " << "P2 Roll: " << (p2->get_ticks_to_move()) / 8 << " Stars: " << p2->get_star_count() << " $$: " << p2->get_coin_count() << vor2;
    
    setGameStatText(oss.str());
    
    if (timeRemaining() <= 0)
    {
        int peach_sum = pointer_to_actor(0)->get_coin_count() + pointer_to_actor(0)->get_star_count();
        int yoshi_sum = pointer_to_actor(1)->get_coin_count() + pointer_to_actor(1)->get_star_count();
        if (peach_sum > yoshi_sum)
        {
            setFinalScore(pointer_to_actor(0)->get_star_count(), pointer_to_actor(0)->get_coin_count());
            return GWSTATUS_PEACH_WON;
        }
        else if (yoshi_sum > peach_sum)
        {
            setFinalScore(pointer_to_actor(1)->get_star_count(), pointer_to_actor(1)->get_coin_count());
            return GWSTATUS_YOSHI_WON;
        }
        else
        {
            if (pointer_to_actor(0)->get_coin_count() > pointer_to_actor(1)->get_coin_count())
            {
                setFinalScore(pointer_to_actor(0)->get_star_count(), pointer_to_actor(0)->get_coin_count());
                return GWSTATUS_PEACH_WON;
            }
            else if (pointer_to_actor(1)->get_coin_count() > pointer_to_actor(0)->get_coin_count())
            {
                setFinalScore(pointer_to_actor(1)->get_star_count(), pointer_to_actor(1)->get_coin_count());
                return GWSTATUS_YOSHI_WON;
            }
            else
            {
                int rand = randInt(1, 2);
                if (rand == 1)
                {
                    setFinalScore(pointer_to_actor(0)->get_star_count(), pointer_to_actor(0)->get_coin_count());
                    return GWSTATUS_PEACH_WON;
                }
                else
                {
                    setFinalScore(pointer_to_actor(1)->get_star_count(), pointer_to_actor(1)->get_coin_count());
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }
    
    for (int i = 0; i < m_players.size(); i++) // Call doSomething() on players that are still active
    {
        if (m_players[i]->still_alive())
            m_players[i]->doSomething();
    }
    
    for (int i = 0; i < m_actors.size(); i++) // Call doSomething() on actors that are still active
    {
        if (m_actors[i]->still_alive())
            m_actors[i]->doSomething();
    }
    
    for (int i = 0; i < m_players.size(); i++) // Clean up dead players
    {
        if (!(m_players[i]->still_alive()))
        {
            delete m_players[i];
            m_players.erase(m_players.begin() + i);
        }
    }
    
    for (int i = 0; i < m_actors.size(); i++) // Clean up dead actors
    {
        if (!(m_actors[i]->still_alive()))
        {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (!m_players.empty())
    {
        delete m_players.back();
        m_players.pop_back();
    }
    
    while (!m_actors.empty())
    {
        delete m_actors.back();
        m_actors.pop_back();
    }
}
