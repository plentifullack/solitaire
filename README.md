# solitaire

simple solitaire game with text/command UI. 

# terminology

cards can exist in any of four different pile types: Stock (s), Discard (d), Tableau (t), Foundation (f).

Stock is a single pile of face down cards not yet played.

Discard is a single pile of face up cards - initially empty and populated by taking the top stock card one at a time. The card atop discard can be moved to a Foundation or Tableau pile. When stock is depleted, it can be repopultated by flipping the discard pile (without changing order);

Tableau (aka, layout) consists of 7 piles of 0 or more cards. Face up cards can be added to a Tableau pile (from discard or tableau or foundation) in order: decending rank (king high to ace low) with alternating color (red goes on black, black goes on red). Any addition to an empty Tableau pile must start with a king.

Foundation consists of four piles, initially empty. The goal of the game is to populate each Foundation pile with all cards from the same suit in ascending order (ace low to king high). Each Foundation pile must start with an ace.
