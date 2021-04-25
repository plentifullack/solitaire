/**
 main.cpp

 Presents a simple console-based game of solitaire.

 see https://github.com/plentifullack/solitaire
 (steve hardy <plentifullackofwit@hotmail.com>)
 */

#include <iostream>
#include "solitaire.h"
/*
 Example move sequence for a winning game (using game option 'x' in my macOs environment):
 ----
 s;s;d;t1;t3;t0;t6;t3;s;s;s;d;f0;s;s;s;s;d;t5;t0,2;t5;t0;t6;t2;t6;t5,4;t1;t1,6;t4;d;t1;t0;t1;t5;f0;t0;t5
    s;s;s;s;d;f1;d;d;d;t6;t4,7;t6;s;s;s;d;t0;t1,3;t0;t1;t3;t1;f2;d;f2;s;d;f3;s;s;s;s;s;d;t0;s;d;t0;t3,3;t0
    t1;f3;t1;f0;t0,10;t1;t2;f0;s;s;s;d;f2;t6;f2;s;d;t0;t3;t0;t4;t3;s;d;t5;t0,3;t4;t0;f0;s;t6;t2;t6;f0;t2,2;t6
    d;t5;s;d;f3;s;d;f1;s;d;t0;s;t1;f3;t6;f3;t1;f2;t1;f3;t1;f0;d;f3;s;d;f0;t2;f0;s
 ----
 */
int main(int argc, const char * argv[])
{
    // To bypass random seeding, use command line argument 'x'.
    //
    // To select a number of initial shuffles (e.g., 3), presumably for a winnable ordering, use argument 'x3'.
    //
    if (argc==2 && 0==strcmp(argv[1], "-h")){
        std::cout << "\nSolitaire game\n\nusage:\n\tsolitaire [xN|-h]\n"
        <<"  where\n \t x derandomizes initial shuffle. (N is a small positive number mapping to a repeatable state on current platform)"
        <<"\n\t-h shows this help and exits\n"
        << std::endl;
    }
    else{
        int shufflect = 1;
        bool randomize_shuffle = argc < 2 || 'x' != *argv[1];
        if (randomize_shuffle) {
            std::srand((unsigned int)std::time(0));
        } else if ('x'==*argv[1] && isdigit(argv[1][1])) {
            shufflect = atoi(&argv[1][1]);
        }
        Game g;
        Deck d2(randomize_shuffle);
        g.start(d2.shuffle().shuffle(shufflect));
    }
    return 0;
}
