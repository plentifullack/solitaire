# Solitaire

A simple solitaire game with text/command UI. 

## Terminology

Cards can exist in any of four different pile types: Stock (**s**), Discard (**d**), Tableau (**t**), Foundation (**f**).

* *Stock* is a single pile of face down cards not yet played.  

* *Discard* is a single pile of face up cards - initially empty and populated by taking the top stock card one at a time. The card atop discard can be moved to a Foundation or Tableau pile. When stock is depleted, it can be repopultated by flipping the discard pile (without changing order);
* *Tableau* (aka, layout) consists of 7 piles of 0 or more cards. Face up cards can be added to a Tableau pile (from discard or tableau or foundation) in order: decending rank (King, high, to Ace, low) with alternating color (red Diamond or Heart goes on black, black Spade or Club goes on red). Any addition to an empty Tableau pile must start with a King.
* *Foundation* consists of four piles, initially empty. The goal of the game is to populate each Foundation pile with all cards from the same suit in ascending order (Ace, low, to King, high). Each Foundation pile must start with an Ace.

## gameplay

Here's a typical game board at startup:

```
t0:    [6]   5_D 
t1:    [5]   3_C 
t2:    [4]   A_D 
t3:    [3]   4_C 
t4:    [2]  10_S 
t5:    [1]   5_C 
t6:    [0]   6_D 

f0: [0] 
f1: [0] 
f2: [0] 
f3: [0] 

s: [24]   d: [0]

Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: 
```

The seven Tableau piles are designated t0 through t6, the foundation piles are f0 through f3, stock is s and discards is d. In the above, we see that t0 has one face up card (five of Diamonds) over 6 face down cards. Foundation piles are empty, stock contains 24 face down cards and nothing is yet discarded. 

We can move cards by entering a source and destination. Above, we see an ace (A_H) in tableau pile t2. We can move the ace to an empty foundation pile by entering the source and destination, separated by a ';' (t2;f0). The board then updates as follows:

```
Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: t2;f0

t0:    [6]   5_D 
t1:    [5]   3_C 
t2:    [3]   7_H 
t3:    [3]   4_C 
t4:    [2]  10_S 
t5:    [1]   5_C 
t6:    [0]   6_D 

f0: [0] A_D
f1: [0] 
f2: [0] 
f3: [0] 

s: [24]   d: [0]
```

We can chain multiple commands in a single entry using ';'. In the following, we move the four of clubs onto the five of Diamonds (t3;t0), then move the five of Clubs onto the six of Diamonds (t5;t6) and finally, flip a stock card (s) onto the discard pile.

```
Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: t3;t0;t5;t6;s


t0:    [6]   5_D  4_C 
t1:    [5]   3_C 
t2:    [3]   7_H 
t3:    [2]   A_H 
t4:    [2]  10_S 
t5:    [1]   5_C 
t6:    [0]   6_D 

f0: [0] A_D
f1: [0] 
f2: [0] 
f3: [0] 

s: [24]   d: [0]


t0:    [6]   5_D  4_C 
t1:    [5]   3_C 
t2:    [3]   7_H 
t3:    [2]   A_H 
t4:    [2]  10_S 
t5:    [0]   5_S 
t6:    [0]   6_D  5_C 

f0: [0] A_D
f1: [0] 
f2: [0] 
f3: [0] 

s: [24]   d: [0]

s: [23]   d:  [0]  8_D

Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: t3;f1

```

An updated game board is shown at each move.

The top card in the discard pile can be moved to a Tableau or Foundation pile. To do so, use d as the source (e.g., d;t2).
Also, it's possible to move more than one card between Tableau piles. To designate more than one card as the source of the move, append ,n to the source command (where n is the desired number of cards). See below example (t0,2;t2)

```
t0:    [6]   5_D  4_C 
t1:    [5]   3_C 
t2:    [3]   7_H 
t3:    [1]   J_D 10_S  9_H 
t4:    [1]   4_S 
t5:    [0]   5_S 
t6:    [0]   6_D  5_C 

f0: [0] A_D
f1: [0] A_H
f2: [0] A_S
f3: [0] A_C

s: [18]   d:  [2]  Q_D

Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: s

s: [17]   d:  [3]  6_C

Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: d;t2


t0:    [6]   5_D  4_C 
t1:    [5]   3_C 
t2:    [3]   7_H  6_C 
t3:    [1]   J_D 10_S  9_H 
t4:    [1]   4_S 
t5:    [0]   5_S 
t6:    [0]   6_D  5_C 

f0: [0] A_D
f1: [0] A_H
f2: [0] A_S
f3: [0] A_C

s: [17]   d:  [2]  Q_D

Enter command (s|d|t{i}[,n]|f{i})[;..]|Q: t0,2;t2


t0:    [5]   7_D 
t1:    [5]   3_C 
t2:    [3]   7_H  6_C  5_D  4_C 
t3:    [1]   J_D 10_S  9_H 
t4:    [1]   4_S 
t5:    [0]   5_S 
t6:    [0]   6_D  5_C 

f0: [0] A_D
f1: [0] A_H
f2: [0] A_S
f3: [0] A_C

s: [17]   d:  [2]  Q_D

```

If you command a source without also providing the required destination, the next command will choose the destination. To cancel a selected source, re-enter the source as destination.

To restock an empty stock pile from the discards, use the **s** command.
