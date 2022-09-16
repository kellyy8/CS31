//
//  main.cpp
//  garks2
//
//  Created by Kelly Yu on 11/24/21.

// garks.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;


///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the mesa
const int MAXCOLS = 25;             // max number of columns in the mesa
const int MAXGARKS = 150;           // max number of garks allowed
const int INITIAL_GARK_HEALTH = 2;

const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Mesa;  // This is needed to let the compiler know that Mesa is a
             // type name, since it's mentioned in the Gark declaration.

class Gark
{
  public:
        // Constructor
    Gark(Mesa* mp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();
    bool getAttacked(int dir);

  private:
    Mesa* m_mesa;
    int   m_row;
    int   m_col;
    int m_health;
};

class Player
{
  public:
        // Constructor
    Player(Mesa *mp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

        // Mutators
    void   stand();
    void   moveOrAttack(int dir);
    void   setDead();

  private:
    Mesa* m_mesa;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class Mesa
{
  public:
        // Constructor/destructor
    Mesa(int nRows, int nCols);
    ~Mesa();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     garkCount() const;
    int     numGarksAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;

        // Mutators
    bool   addGark(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackGarkAt(int r, int c, int dir);
    bool   moveGarks();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Gark*   m_garks[MAXGARKS];
    int     m_nGarks;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nGarks);
    ~Game();

        // Mutators
    void play();

  private:
    Mesa* m_mesa;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Gark implementation
///////////////////////////////////////////////////////////////////////////

Gark::Gark(Mesa* mp, int r, int c)
{
    if (mp == nullptr)
    {
        cout << "***** A gark must be created in some Mesa!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > mp->rows()  ||  c < 1  ||  c > mp->cols())
    {
        cout << "***** Gark created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_mesa = mp;
    m_row = r;
    m_col = c;
    m_health = INITIAL_GARK_HEALTH;
}

int Gark::row() const
{
    return m_row;
}

int Gark::col() const
{
    return m_col;
}

void Gark::move()
{
      // Attempt to move in a random direction; if it can't move, don't move
    int dir = randInt(0, NUMDIRS-1);  // dir is now UP, DOWN, LEFT, or RIGHT

    switch(dir){
        case UP:
            if(m_row != 1)                      //only move if we're NOT on the edge
                m_row -= 1;
            break;
        case DOWN:
            if(m_row != m_mesa->rows())                       //m_mesa = mesa pointer Gark objects have
                m_row += 1;
            break;
        case LEFT:
            if(m_col != 1)
                m_col -= 1;
            break;
        case RIGHT:
            if(m_col != m_mesa->cols())
                m_col += 1;
            break;
        default:
            break;
    }
}

bool Gark::getAttacked(int dir)  // return true if dies
{
    //this is the second attack
    if(m_health == 1)
        return true;
    
    //this is the first attack; check if Gark will fall off edge
    if( (m_row == 1 && dir == UP) || (m_row == m_mesa->rows() && dir == DOWN)
        || (m_col == 1 && dir == LEFT) || (m_col == m_mesa->cols() && dir == RIGHT))
        return true;
    else{
        m_health--;
        switch(dir){
            case UP:        m_row--; break;
            case DOWN:      m_row++; break;
            case LEFT:      m_col--; break;
            case RIGHT:     m_col++; break;
            default:        break;
        }
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Mesa* mp, int r, int c)
{
    if (mp == nullptr)
    {
        cout << "***** The player must be created in some Mesa!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > mp->rows()  ||  c < 1  ||  c > mp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_mesa = mp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::moveOrAttack(int dir)      //dir == UP, DOWN, LEFT, RIGHT represented by const integers
{
    m_age++;

    switch(dir){
        case UP:
            if(0 != m_mesa->numGarksAt(m_row-1, m_col)){              //attack if there is at least 1 Gark
                m_mesa->attackGarkAt(m_row-1, m_col, dir);
                return;                                         //attacked Gark; don't need to move Player
            }
//            cout << "Number of garks here: " << m_mesa->numGarksAt(m_row-1, m_col) << endl;
            break;          //no Garks; move the Player
        case DOWN:
            if(0 != m_mesa->numGarksAt(m_row+1, m_col)){
                m_mesa->attackGarkAt(m_row+1, m_col, dir);
                return;
            }
//            cout << "Number of garks here: " << m_mesa->numGarksAt(m_row+1, m_col) << endl;
            break;
        case LEFT:
            if(0 != m_mesa->numGarksAt(m_row, m_col-1)){
                m_mesa->attackGarkAt(m_row, m_col-1, dir);
                return;
            }
//            cout << "Number of garks here: " << m_mesa->numGarksAt(m_row+1, m_col) << endl;
            break;
        case RIGHT:
            if(0 != m_mesa->numGarksAt(m_row, m_col+1)){
                m_mesa->attackGarkAt(m_row, m_col+1, dir);
                return;
            }
//            cout << "Number of garks here: " << m_mesa->numGarksAt(m_row+1, m_col) << endl;
            break;
        default:
            break;
    }
    
    //the below only runs if we broke out of switch statement == did not attack a Gark
    m_mesa->determineNewPosition(m_row, m_col, dir);
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Mesa implementations
///////////////////////////////////////////////////////////////////////////

Mesa::Mesa(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Mesa created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nGarks = 0;
}

Mesa::~Mesa()
{
    delete m_player;
    
    for(int i=0; i<m_nGarks; i++){
        delete m_garks[i];          //no dangling pointers, b/c pointer array goes away when mesa goes away
    }
}

int Mesa::rows() const
{
    return m_rows;
}

int Mesa::cols() const
{
    return m_cols;
}

Player* Mesa::player() const
{
    return m_player;
}

int Mesa::garkCount() const
{
    return m_nGarks;
}

int Mesa::numGarksAt(int r, int c) const
{
    //counter variable
    int numGarksHere=0;
    
    if(r < 1 || c < 1 || r > m_rows || c > m_cols)
        return 0;
    
    //go through all Garks; count the Garks with matching coordinates
    for(int i=0; i<m_nGarks; i++){
        if(m_garks[i]->row() == r && m_garks[i]->col() == c)
            numGarksHere++;
    }
    
    return numGarksHere;
}

bool Mesa::determineNewPosition(int& r, int& c, int dir) const
{
    //Player does not move if Player will fall off edge
    if( (r == 1 && dir == UP) || (r == m_rows && dir == DOWN)
       || (c == 1 && dir == LEFT) || (c == m_cols && dir == RIGHT)){
//        cout << "Player: (r,c) = " << m_row << "," << m_col <<endl;
        return false;
    }
    
    //otherwise, Player moves in direction dir
    switch (dir)
    {
        case UP:    r--; break;
        case DOWN:  r++; break;
        case LEFT:  c--; break;
        case RIGHT: c++; break;
        default:
            return false;
    }
    return true;
}

void Mesa::display() const
{
      // Position (row,col) in the mesa coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    //follow Gark pointer to Gark object & retrieve Gark's coordinates
    int garkRow=0, garkCol=0;
    
    for (int i=0; i < m_nGarks; i++){
        garkRow = m_garks[i]->row()-1;              //element position is one less than row/col number
        garkCol = m_garks[i]->col()-1;
        
        //first Gark at position
        if(grid[garkRow][garkCol] == '.'){
            grid[garkRow][garkCol] = 'G';
        }
        //second Gark at position
        else if (grid[garkRow][garkCol] == 'G'){
            grid[garkRow][garkCol] = '2';
        }
        //3+ Garks at position
        else if ('2' <= grid[garkRow][garkCol] && grid[garkRow][garkCol] < '9'){
            grid[garkRow][garkCol]++;
        }
    }


        // Indicate player's position
    if (m_player != nullptr)
    {
          // Set the char to '@', unless there's also a gark there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, gark, and player info
    cout << endl;
    cout << "There are " << garkCount() << " garks remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Mesa::addGark(int r, int c)
{
    if(m_nGarks == MAXGARKS)        //out of bounds
        return false;
    
    m_garks[m_nGarks] = new Gark(this, r, c);           //here, don't adjust r,c for element positions
    m_nGarks++;
    return true;
}

bool Mesa::addPlayer(int r, int c)
{
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically allocate a new Player and add it to the mesa
    m_player = new Player(this, r, c);
    return true;
}

bool Mesa::attackGarkAt(int r, int c, int dir)
{
    int k;                           //if we need to delete later, use kth element as the starting point
    bool garkIsDead = false;
    
    //at least one Gark exists in order for this function to run; already checked
    //find pointer to Gark at (r,c) to attack Gark object
    for(k=0; k < m_nGarks; k++){
        if(m_garks[k]->row() == r && m_garks[k]->col() == c){
            garkIsDead = m_garks[k]->getAttacked(dir);          //attack gark & tell me if it's dead after
            break;
        }
    }
    
    if(garkIsDead){           //expression == true if Gark died
        //removes the Gark we killed
        delete m_garks[k];
        
        //shift the pointers from k up to m_nGarks-1, so we don't have a dangling pointer
        for(; k<m_nGarks-1; k++){
            m_garks[k]=m_garks[k+1];
        }
        
        m_nGarks--;
        return true;
    }

    return false;
}

bool Mesa::moveGarks()
{
    for (int k = 0; k < m_nGarks; k++)
    {
        m_garks[k]->move();         //make Gark object change its own row# and col# since private members

        //check if player is at Gark's new position
        //m_player == mesa's pointer to a Player object
        if(m_garks[k]->row() == m_player->row() && m_garks[k]->col() == m_player->col()){
            m_player->setDead();
        }
    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();      //isDead == true -> returns false; isDead == false -> returns true
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nGarks)
{
    if (nGarks < 0)
    {
        cout << "***** Cannot create Game with negative number of garks!" << endl;
        exit(1);
    }
    if (nGarks > MAXGARKS)
    {
        cout << "***** Trying to create Game with " << nGarks
             << " garks; only " << MAXGARKS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nGarks > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the garks!" << endl;
        exit(1);
    }

        // Create mesa
    m_mesa = new Mesa(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_mesa->addPlayer(rPlayer, cPlayer);

      // Populate with garks
    while (nGarks > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a gark where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_mesa->addGark(r, c);
        nGarks--;
    }
}

Game::~Game()
{
    delete m_mesa;
}

void Game::play()
{
    m_mesa->display();
    Player* player = m_mesa->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_mesa->garkCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r/q or nothing): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player stands
            player->stand();
        else
        {
            switch (action[0])          //read element 0 of string == char
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                player->moveOrAttack(decodeDirection(action[0]));   //decode translates char to const dir#
                break;
            }
        }
        m_mesa->moveGarks();
        m_mesa->display();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

void doBasicTests();
int main()
{
//    doBasicTests(); // Remove this line after completing test.
//       return 0;       // Remove this line after completing test.
//      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

#include <type_traits>
#include <cassert>

#define CHECKTYPE(c, f, r, a)  \
    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
       "FAILED: You changed the type of " #c "::" #f);  \
    [[gnu::unused]] auto xxx##c##_##f = static_cast<r(c::*)a>(&c::f)

void thisFunctionWillNeverBeCalled()
{
      // If the student deleted or changed the interfaces to the public
      // functions, this won't compile.  (This uses magic beyond the scope
      // of CS 31.)

    Gark(static_cast<Mesa*>(0), 1, 1);
    CHECKTYPE(Gark, row, int, () const);
    CHECKTYPE(Gark, col, int, () const);
    CHECKTYPE(Gark, move, void, ());
    CHECKTYPE(Gark, getAttacked, bool, (int));

    Player(static_cast<Mesa*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, age, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, stand, void, ());
    CHECKTYPE(Player, moveOrAttack, void, (int));
    CHECKTYPE(Player, setDead, void, ());

    Mesa(1, 1);
    CHECKTYPE(Mesa, rows, int, () const);
    CHECKTYPE(Mesa, cols, int, () const);
    CHECKTYPE(Mesa, player, Player*, () const);
    CHECKTYPE(Mesa, garkCount, int, () const);
    CHECKTYPE(Mesa, numGarksAt, int, (int, int) const);
    CHECKTYPE(Mesa, determineNewPosition, bool, (int&, int&, int) const);
    CHECKTYPE(Mesa, display, void, () const);
    CHECKTYPE(Mesa, addGark, bool, (int, int));
    CHECKTYPE(Mesa, addPlayer, bool, (int, int));
    CHECKTYPE(Mesa, attackGarkAt, bool, (int, int, int));
    CHECKTYPE(Mesa, moveGarks, bool, ());

    Game(1, 1, 1);
    CHECKTYPE(Game, play, void, ());
}

void doBasicTests()
{
    {
        Mesa arizona(10, 20);
        assert(arizona.addPlayer(2, 6));
        Player* pp = arizona.player();
        assert(pp->row() == 2  &&  pp->col() == 6  && ! pp->isDead());
        pp->moveOrAttack(UP);
        assert(pp->row() == 1  &&  pp->col() == 6  && ! pp->isDead());
        pp->moveOrAttack(UP);
        assert(pp->row() == 1  &&  pp->col() == 6  && ! pp->isDead());
        pp->setDead();
        assert(pp->row() == 1  &&  pp->col() == 6  && pp->isDead());
    }
    {
        Mesa lliance(2, 2);
        assert(lliance.addPlayer(1, 1));
        assert(lliance.addGark(2, 2));
        Player* pp = lliance.player();
        assert(lliance.moveGarks());
        assert( ! pp->isDead());
        for (int k = 0; k < 1000  &&  ! pp->isDead()  &&  lliance.moveGarks(); k++)
            assert(lliance.numGarksAt(1, 1) == 0);
        assert(pp->isDead()  &&  lliance.numGarksAt(1, 1) == 1);
    }
    {
        Mesa potamia(2, 6);
        assert(potamia.addPlayer(2, 1));
        assert(potamia.addGark(2, 3));
        assert(potamia.addGark(1, 1));
        assert(potamia.addGark(1, 6));
        Player* pp = potamia.player();
        pp->moveOrAttack(RIGHT);
        assert(potamia.garkCount() == 3  &&  potamia.numGarksAt(2, 3) == 1);
        pp->moveOrAttack(RIGHT);
        assert(potamia.garkCount() == 3  &&  potamia.numGarksAt(2, 4) == 1);
        pp->moveOrAttack(RIGHT);
        assert(potamia.garkCount() == 3  &&  potamia.numGarksAt(2, 4) == 1);
        pp->moveOrAttack(RIGHT);
        assert(potamia.garkCount() == 2  &&  potamia.numGarksAt(2, 4) == 0   &&  potamia.numGarksAt(2, 5) == 0);
        potamia.addGark(1, 3);
        assert(potamia.garkCount() == 3  &&  potamia.numGarksAt(1, 3) == 1);
        pp->moveOrAttack(UP);
        assert(potamia.garkCount() == 2  &&  potamia.numGarksAt(1, 3) == 0);
          // If the program crashes after leaving this compound statement, you
          // are probably messing something up when you delete a dead Gark
          // (or you have mis-coded the destructor).
          //
          // Draw a picture of your m_garks array before attcking
          // and also note the values of m_nGarks or any other variables you
          // might have that are involved with the number of Garks.  Trace
          // through your code step by step as the Garks die and are
          // removed, updating the picture according to what the code says, not
          // what you want it to do.  If you don't see a problem then, try
          // tracing through the destruction of the mesa.
          //
          // If you execute the code, use the debugger to check on the values
          // of key variables at various points.  If you didn't try to learn
          // to use the debugger, insert statements that write the values of
          // key variables to cerr so you can trace the execution of your code
          // and see the first place where something has gone amiss.  (Comment
          // out the call to clearScreen in Mesa::display so that your output
          // doesn't disappear.)
    }
    cout << "Passed all basic tests" << endl;
}
