/**
 solitaire.cpp

 Defines most functionality for playing a simple game of solitaire.

 see https://github.com/plentifullack/solitaire
 (steve hardy <plentifullackofwit@hotmail.com>)
 */

#include <ctype.h>
#include "solitaire.h"

/**
* A Pile contains an ordered collection of Cards and a Game reference.
*
* Has a choose method that can request game update if choice is valid.
*/
Pile::Pile(Game &g, std::string ID) : game(g), id(ID)
{
}

Pile::~Pile()
{
}

std::string Pile::getid()
{
    return id;
}

bool Discards::choose(int)
{
    if (!game.hasPick()) {
        if (!cards.empty()) {
            game.pick(this, &cards.back());
            return true;
        }
    } else if (typeid(*game.pickedPile()) == typeid(*this)) {
        game.unpick();
        return true;
    }
    return false;
}

Discards::Discards(Game&g, std::string id) :Pile(g, id)
{
}

Discards::~Discards()
{
}

std::string Discards::toString()
{
    std::ostringstream msg;
    if (cards.size() == 0) {
        msg << "[" << cards.size() << "]";
    } else {
        msg << " [" << (cards.size() - 1) << "] " << std::setw(4) << cards.back().read();    // (?) or 0
    }
    return msg.str();
}

bool Stock::choose(int)
{
    bool updated = false;
    if (!game.hasPick()) {
        if (cards.empty()) {
            updated = restock();
        } else {
            Card nextcard = cards.back();
            discards.cards.push_back(nextcard.flip());
            cards.pop_back();
            updated = true;
        }
    }
    return updated;
}

Stock::Stock(Game&g, std::string id, Discards&disc) :Pile(g, id), discards(disc)
{
}

Stock::~Stock()
{
}

std::string Stock::toString()
{
    std::ostringstream msg;
    msg << "[" << cards.size() << "]";
    return msg.str();
}

bool Stock::restock()
{
    bool empty = discards.cards.size() == 0;
    for (int i = discards.cards.size() - 1; i >= 0; --i) {
        Card nextcard = discards.cards[i];
        cards.push_back(nextcard.flip());
    }
    discards.cards.clear();
    return !empty;
}

bool Tableau::choose(int ct)
{
    bool updated = false;
    if (game.hasPick()) {
        Pile*srcp = game.pickedPile();
        unsigned srcsz = srcp->cards.size();
        if ( srcp == this) { // same src pile
            game.unpick();
            updated = true;
        } else { // different src pile
            unsigned ct = game.pickedCount();
            bool visiblesrc = ct <= srcsz && !srcp->cards[srcsz-ct].isHidden();
            if (cards.empty()) {
                if (game.pickedCard()->getRank() == Card::KING) {
                    for (auto i=ct; i>0; --i) {
                        cards.push_back(srcp->cards[srcsz-i]);
                    }
                    for (unsigned i=0; i<ct; ++i) {
                        srcp->cards.pop_back();
                    }
                    // ?need to flip top hidden card in src deck
                    if (srcp->cards.size()>0 && srcp->cards.back().isHidden()) {
                        srcp->cards.back() = srcp->cards.back().flip();
                    }
                    game.unpick();
                    updated = true;
                }
            } else {
                Card::Suit s1 = cards.back().getSuit();
                Card::Suit s2 = game.pickedCard()->getSuit();
                auto alt_color = [s1,s2]() {
                    bool thisIsRed = s1 == Card::DIAMONDS || s1 == Card::HEARTS;
                    bool thatIsRed = s2 == Card::DIAMONDS || s2 == Card::HEARTS;
                    return thisIsRed != thatIsRed;
                };
                if (visiblesrc&&-1 == cards.back().cmpAdjacency(*game.pickedCard(),alt_color)) { // valid move?
                    for (int i = ct; i > 0; --i) {
                        cards.push_back(srcp->cards[srcsz - i]);
                    }
                    for (unsigned i=0; i<ct; ++i) {
                        srcp->cards.pop_back();
                    }
                    // ?need to flip top hidden card in src deck
                    if (srcp->cards.size()>0 && srcp->cards.back().isHidden()) {
                        srcp->cards.back() = srcp->cards.back().flip();
                    }
                    game.unpick();
                    updated = true;
                }
            }
        }
    } else if (!cards.empty()) {
        if (ct > 0 && (unsigned)ct <= cards.size() && !cards[cards.size() - ct].isHidden()) {
            game.pick(this, &cards[cards.size() - ct], ct);
            updated = true;
        }
    }
    return updated;
}

Tableau::Tableau(Game&g, std::string id) :Pile(g, id)
{
}

Tableau::~Tableau()
{
}

std::string Tableau::toString()
{
    std::ostringstream msg;
    int facedown_ct = 0;
    if (cards.size() == 0) {
        msg << std::setw(4) << "[" << facedown_ct << "] ";
    }
    bool counting = true;
    for (unsigned j = 0; j < cards.size(); ++j) {
        if (counting) {
            if (cards[j].isHidden()) {
                ++facedown_ct;
            } else {
                counting = false;
                //print ct + first shown card
                msg << std::setw(4) << "[" << facedown_ct << "] " << std::setw(5) << cards[j].read() << " ";
            }
        } else {
            msg << std::setw(4) << cards[j].read() << " ";
        }
    }
    return msg.str();
}

bool Foundation::choose(int)
{
    bool updated = true;
    if (game.hasPick()) {
        Pile *p = game.pickedPile();
        Card src_card = p->cards.back();
        if (cards.empty()) {
            if (game.pickedCard()->getRank() == Card::ACE) {
                // take pickedCard
                cards.push_back(src_card);
                p->cards.pop_back();
                if (p->cards.size() > 0 && p->cards.back().isHidden()) {
                    p->cards.back() = p->cards.back().flip();
                }
                game.unpick();
                updated = true;
            }
        } else if (0==game.pickedCard()->shortname().compare(cards.back().shortname())) {
            game.unpick();
            updated = true;
        } else {
            Card::Suit s1 = cards.back().getSuit();
            Card::Suit s2 = game.pickedCard()->getSuit();
            auto same_suit = [s1,s2]() {
                return s1==s2;
            };
            if (1==game.pickedCount() && 1==cards.back().cmpAdjacency(*game.pickedCard(),same_suit)) { // valid move?
                // move card
                cards.push_back(*game.pickedCard());
                game.pickedPile()->cards.pop_back();
                if (p->cards.size() > 0 && p->cards.back().isHidden()) {
                    p->cards.back() = p->cards.back().flip();
                }
                game.unpick();
                updated = true;
            }
        }
    } else if (!cards.empty()) {
        // pick top card
        game.pick(this, &cards.back());
        updated = true;
    }
    return updated;
}

Foundation::Foundation(Game&g, std::string id) :Pile(g, id)
{
}

Foundation::~Foundation()
{
}

std::string Foundation::toString()
{
    int cardct = cards.size();
    std::ostringstream msg;
    msg << "[" << (cardct>1?cardct-1:0) << "] ";
    if (cardct > 0) {
        msg << cards.back().shortname();
    }
    return msg.str();
}

Command::Command() : id('Q'), p(nullptr), count(0)
{
}

Command::Command(char ID, Pile *pile, int ct) :id(ID), p(pile), count(ct)
{
}

Game::Selection::Selection(Pile *p, Card* c, int ct) :pRef(p), cRef(c), count(ct)
{
}

Game::Selection::Selection() :pRef(nullptr), cRef(nullptr), count(0)
{
}

bool Game::Selection::isSelected() const
{
    return pRef != nullptr && cRef != nullptr && count > 0;
}

void Game::unpick()
{
    currentPick = Selection();
}

void Game::pick(Pile*p, Card*c, int n)
{
    currentPick = Selection(p, c, n);
}

bool Game::hasPick() const
{
    return currentPick.isSelected();
}

/**
 get a pointer to the currently selected source Pile, if any. If no source Pile is currently selected, then
 pickedPile(isSelected() returns false.

 @return a pointer to the currently selected source pile, or nullptr if none selected.
 */
Pile* Game::pickedPile()
{
    return currentPick.pRef;
}

Card* Game::pickedCard()
{
    return currentPick.cRef;
}

int Game::pickedCount() const
{
    return currentPick.count;
}

bool Game::isWon()
{
    bool won = true;
    for (int i = 0; i < TABLEAU_CT; ++i) {
        if (tableau[i].cards.size() >0 && tableau[i].cards[0].isHidden()) {
            won = false;
            break;
        }
    }
    return won;
}

Game::Game() : currentPick()
{
    // initialize empty Tab piles.
    std::stringstream id;
    for (int i = 0; i < TABLEAU_CT; ++i) {
        id.str("");
        id << "T" << i;
        tableau.push_back(Tableau(*this, id.str()));
    }
    for (int i = 0; i < FOUNDATION_CT; ++i) {
        id.str("");
        id << "F" << i;
        foundation.push_back(Foundation(*this, id.str()));
    }
    id.str("");
    id << "D";
    for (int i = 0; i < 1; ++i) {
        discards.push_back(Discards(*this, id.str()));
    }
    id.str("");
    id << "S";
    for (int i = 0; i < 1; ++i) {
        stock.push_back(Stock(*this, id.str(), discards[0]));
    }
}

void Game::start(Deck&d)
{
    // initialize game context
    //  tableau populate
    for (int i = TABLEAU_CT - 1; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            tableau[j].cards.push_back(i == j ? d.deal().flip() : d.deal());
        }
    }
    //  stock populate
    while (d.card_count() > 0) {
        stock[0].cards.push_back(d.deal());
    }

    show();
    bool done = false;
    while (!done) {
        try {
            std::vector<Command> c = get_cmd();
            for (int i=0; i<c.size(); ++i) {
                switch (c[i].id) {
                case 's':
                case 'd':
                case 'f':
                    c[i].p->choose();
                    break;
                case 't':
                    c[i].p->choose(c[i].count);
                    break;
                case 'Q':
                    done = true;
                    break;
                }
                if (!done) {
                    // only explicitly show src pick when results from last command in current list
                    if (!hasPick() || i==c.size()-1) {
                        show('s'==c[i].id);
                    }
                }
            }
        } catch (std::invalid_argument& ex) {
            std::cerr << std::endl << ex.what() << std::endl;
        }
    }
}

std::vector<Command> Game::get_cmd()
{
    std::cout << std::endl;
    if (hasPick()) {
        std::ostringstream msg;
        msg << "{" << currentPick.pRef->getid() << "(" << currentPick.cRef->shortname();
        if (currentPick.count > 1) {
            msg << "+" << (currentPick.count - 1);
        }
        msg << ")} ";
        std::cout << msg.str();
    } else if (isWon()) {
        std::cout << std::endl << "WINNER! " << std::endl;
    }
    std::cout << "Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: ";
    std::string c;
    std::cin >> c;

    std::vector<Command> cmds;
    std::istringstream f(c);
    std::string s;
    while (getline(f, s, ';')) {
        if (s.length()==0) {
            // ignore
        } else if (s.at(0) == 's' && s.length() == 1) {
            cmds.push_back(Command('s', &stock[0]));
        } else if (s.at(0) == 'd' && s.length() == 1) {
            cmds.push_back(Command('d', &discards[0]));
        } else if (s.at(0) == 'f' && s.length() == 2 && isdigit(s[1]) && (s[1] - '0')<FOUNDATION_CT) {
            cmds.push_back(Command('f', &foundation[s[1] - '0']));
        } else if (s.at(0) == 't' &&
                   s.length() > 1 &&
                   isdigit(s[1]) &&
                   (s[1] - '0')<TABLEAU_CT &&
                   (s.length()==2||s[2]==',')) {
            if (s.length() > 3) {
                cmds.push_back(Command('t', &tableau[s[1] - '0'], std::stoi(s.substr(3))));
            } else {
                cmds.push_back(Command('t', &tableau[s[1] - '0']));
            }
        } else if (s.at(0) == 'Q' && s.length() == 1) {
            cmds.push_back(Command()); // quit command
        } else {
            std::ostringstream msg;
            msg << "unrecognized command. Try again: [" << s << "]";
            throw std::invalid_argument(std::string(msg.str()));
        }
    }
    return cmds;
}

void Game::show(bool minimal)
{
    if (!minimal) {
        std::cout << std::endl;
        for (int i = 0; i<TABLEAU_CT; ++i) {
            std::cout << std::endl << "t" << i << ": " << tableau[i].toString();
        }
        std::cout << std::endl << std::endl;
        for (int i = 0; i < FOUNDATION_CT; ++i) {
            std::cout << "f" << i << ": " << foundation[i].toString() << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "s: " << stock[0].toString() << "   d: " << discards[0].toString() << std::endl;
}
