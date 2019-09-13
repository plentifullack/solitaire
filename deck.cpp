//
//  deck
#include "deck.h"
#include <iostream>
#include <exception>
#include <sstream>
#include <algorithm>
#include <random>
#include <iomanip>

// Card defs
Card::Rank Card::getRank() const
{
    return (Rank)(this->value % RANK_CT);
}

Card::Suit Card::getSuit() const
{
    return (Suit)(this->value / RANK_CT);
}

std::string Card::suitName() const
{
    Suit s = getSuit();
    switch (s) {
    case CLUBS:
        return "Clubs";
    case DIAMONDS:
        return "Diamonds";
    case HEARTS:
        return "Hearts";
    case SPADES:
        return "Spades";
    default:
        std::ostringstream msg;
        msg << "name requested for invalid suit: " << s;
        throw std::invalid_argument(msg.str());
    }
}

std::string Card::rankName()
{
    Rank r = getRank();
    switch (r) {
    case ACE:
        return "Ace";
    case TWO:
        return "Two";
    case THREE:
        return "Three";
    case FOUR:
        return "Four";
    case FIVE:
        return "Five";
    case SIX:
        return "Six";
    case SEVEN:
        return "Seven";
    case EIGHT:
        return "Eight";
    case NINE:
        return "Nine";
    case TEN:
        return "Ten";
    case JACK:
        return "Jack";
    case QUEEN:
        return "Queen";
    case KING:
        return "King";
    default:
        std::ostringstream msg;
        msg << "name requested for invalid rank: " << r;
        throw std::invalid_argument(msg.str());
    }
}

std::string Card::shortname() const
{
    std::ostringstream name;
    Rank r = getRank();
    switch (r) {
    case ACE:
        name << "A";
        break;
    case TWO:
        name << "2";
        break;
    case THREE:
        name << "3";
        break;
    case FOUR:
        name << "4";
        break;
    case FIVE:
        name << "5";
        break;
    case SIX:
        name << "6";
        break;
    case SEVEN:
        name << "7";
        break;
    case EIGHT:
        name << "8";
        break;
    case NINE:
        name << "9";
        break;
    case TEN:
        name << "10";
        break;
    case JACK:
        name << "J";
        break;
    case QUEEN:
        name << "Q";
        break;
    case KING:
        name << "K";
        break;
    default:
        std::ostringstream msg;
        msg << "name requested for invalid rank: " << r;
        throw std::invalid_argument(msg.str());
    }
    name << "_" << suitName()[0];
    return name.str();
}
int Card::cmpAdjacency(Card&other) const
{
	auto always_allow = []() {return true; }; // by default, no caller imposed constraint.
    return cmpAdjacency(other, always_allow);
}
int Card::cmpAdjacency(Card&other, std::function<bool()> is_allowed) const
{
    if (is_allowed()) {
        int diff = ((int)getRank())%(int)Card::RANK_CT - (int)other.getRank() % (int)Card::RANK_CT;
        if (1 == diff) {
            return -1; // ascending adjacency
        }
        if (-1 == diff) {
            return 1; // descending adjacency
        }
    }
    return 0; // not adjacent
}

std::string Card::read()
{
    return this->isShowing ? shortname() : "[?]";
}

Card& Card::flip()
{
    isShowing = !isShowing;
    return *this;
}

Card::Card(int val, bool shown) : value(val), isShowing(shown)
{
    if (val < 0 || val >= DECK_SIZE) {
        std::ostringstream msg;
        msg << "bad value for Card init: " << val;
        throw std::invalid_argument(std::string(msg.str()));
    }
}

bool Card::isHidden()
{
    return !isShowing;
}


// Deck defs

Deck::Deck(bool randomize) : rnd(randomize)
{
    for (int i = 0; i < 52; ++i) {
        cards.push_back(Card(i));
    }
}

struct RNG {
    int operator() (int n)
    {
        return (int)(std::rand() / (1.0 + RAND_MAX) * n);
    }
};

Deck& Deck::shuffle(int count)
{
    for (int i = 0; i < count; ++i) {
        if (rnd) {
            std::random_shuffle(cards.begin(), cards.end(), RNG());
        } else {
            std::random_shuffle(cards.begin(), cards.end());
        }
    }
    return *this;
}

void Deck::show()
{
    std::vector<Card>::iterator it;
    for (it = cards.begin(); it != cards.end(); ++it) {
        std::cout << it->shortname() << std::endl;
    }
}
/**
give one card from deck to caller. (throws range_error if no cards available)
*/
Card Deck::deal()
{
    if (cards.size() <= 0) {
        throw std::range_error("no cards left");
    }
    Card c = cards[cards.size() - 1];
    cards.pop_back();
    return c;
}

int Deck::card_count()
{
    return cards.size();
}

