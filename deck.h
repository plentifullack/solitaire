#pragma once

//
//  deck
#include <vector>

class Deck;

class Card
{
    friend Deck;
public:
    enum Suit {
        CLUBS = 0,
        DIAMONDS = 1,
        HEARTS = 2,
        SPADES = 3,
        SUIT_CT = 4
    };

    enum Rank {
        ACE = 0,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        RANK_CT = 13
    };
    // 
    int cmpAdjacency(Card&other) const;
    int cmpAdjacency(Card&other, std::function<bool()>) const;
    Rank getRank() const;
    Suit getSuit() const;

    std::string suitName() const;

    std::string rankName();

    std::string shortname() const ;
    std::string read();

    Card& flip();

private:
    Card() = delete;//{throw std::invalid_argument("no value provided for Card initializer.");}
    enum { DECK_SIZE = SUIT_CT*RANK_CT };
    /**
    can only create from friend context (Deck)
    */
    Card(int val, bool shown = false);

    int value;
    bool isShowing;
public:
    bool isHidden();
};

class Deck
{

public:
    Deck(bool randomize=true);
    Deck& shuffle(int ct=1);
    void show();
    /**
    give one card from deck to caller. (throws range_error if no cards available)
    */
    Card deal();
    int card_count();
private:
    std::vector<Card> cards;
    const bool rnd;
};
