/**
 solitaire.h

 see https://github.com/plentifullack/solitaire
 (steve hardy <plentifullackofwit@hotmail.com>)
 */

#pragma once
#include "deck.h"

#include <iostream>
#include <exception>
#include <sstream>
#include <algorithm>
#include <random>
#include <iomanip>


class Game;

/**
* A Pile contains an ordered collection of Cards and a Game reference.
*
* Has a choose method that requests corresponding game update if choice is valid (as determined by target Pile).
*/
class Pile
{
public:
    // gets game ref
    Game& game;
    /**
     Request corresponding game update if choice is valid

     @param ct Number of cards, where multi-pick is allowed. Defaults to 1.
     @return true if choice caused change in game state.
     */
    virtual bool choose(int ct = 1) = 0;
    virtual std::string toString() = 0;
private:
    std::string id;
public:
    Pile(Game &g, std::string ID);
    virtual ~Pile();
public:
    std::vector<Card> cards;
    std::string getid();
};

class Discards : public Pile
{
public:
    Discards(Game&g, std::string id);
    virtual ~Discards();
    /**
     Request corresponding game update if choice is valid

     @param ct Number of cards for a source pick. (Optional and Ignored in this context. 1 is used)
     @return true if choice caused change in game state.
     */
    bool choose(int ct=1);
    std::string toString();
};

class Stock : public Pile
{
    // gets ref to Discards
    Discards&discards;

public:
    /**
     @param g Game in which this Stock Pile participates.
     @param disc Associated Discard pile. During gameplay, the only valid destination for a
     Stock Pile pick is the Discard Pile. So choosing Stock source automatically completes the implied move.
     */
    Stock(Game&g, std::string id, Discards&disc);
    virtual ~Stock();

    /**
     Request corresponding game update if choice is valid

     @param ct Number of cards for a source pick. (Optional and Ignored in this context. 1 is used)
     @return true if choice caused change in game state.
     */
    bool choose(int ct=1);
    std::string toString();
    bool restock();
};

class Tableau : public Pile
{
public:
    Tableau(Game&g, std::string id);
    virtual ~Tableau();
    /**
     Request corresponding game update if choice is valid

     @param ct Number of cards (for a source pick - ignored for destination). Defaults to 1.
     @return true if choice caused change in game state.
     */
    bool choose(int ct=1);
    std::string toString();
};

class Foundation : public Pile
{
public:
    /**
     Request corresponding game update if choice is valid

     @param ct Number of cards for a source pick. (Optional and Ignored in this context. 1 is used)
     @return true if choice caused change in game state.
     */
    bool choose(int ct=1);
    Foundation(Game&g, std::string id);
    virtual ~Foundation();
    std::string toString();
};

class Command
{
public:
    Pile *p;
    int count;
    char id;
    Command();
    Command(char ID, Pile *pile, int ct = 1);
};

class Game
{
public:
    class Selection
    {
    public:
        Selection(Pile *p, Card* c, int ct = 1);
        Selection();
        bool isSelected() const;
    public:
        Pile*pRef;
        Card*cRef;
        int count;
    };
private:
    Selection currentPick;
public:
    void unpick();
    void pick(Pile*p, Card*c, int n = 1);
    enum { FOUNDATION_CT = 4, TABLEAU_CT = 7 };

    std::vector<Tableau> tableau;
    std::vector<Foundation> foundation;
    std::vector<Discards> discards;
    std::vector<Stock> stock;

    bool hasPick() const;
    Pile* pickedPile();
    Card* pickedCard();
    int pickedCount() const;
    bool isWon();
    Game();
    void start(Deck&d);

    std::vector<Command> get_cmd();

    void show(bool minimal=false);
};
