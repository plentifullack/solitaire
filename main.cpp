//
//  main.cpp
//  solitaire
//

#include <iostream>
#include "solitaire.h"
/*
 Example move sequence for a winning game (using game option 'x' on macOs):
 ----
 t3;t1;t4;t0;t0,2;t6;s;t3;f0;d;t2;t6;f0;s;t0;t2;t6,2;t3;t1,2;t6;t3,3;t0;t1;t5;s;s;s;d;t3;d;t3;d;t3;t1;t3;d;t1;s;s;d;t5;t1,2;t5;s;s;s;d;f1;s;s;s;d;f2;s;t1;f2;t4;f2;t4;t1;t5,5;t4;s;d;f3;s;t2;f3;t0;f3;t2;f0;t2;f2;t2;f1;t1;f2;s;d;t5;s;t0;f0;t2;t0;t2;t3;t2;t3;s;s;s;d;f1;t0;f1;t3;f1;t3;f2;t0;t1;t0;f1;t3;f1;s;d;t4;s;s;d;t6;s;s;d;t2;s;d;f3;t1;f3;s;t4;f1;t0;f1;t0;t3;t0;f0
 ----
 */
int main(int argc, const char * argv[]) {
	int shufflect = 1;
	bool randomize_shuffle = argc < 2 || 'x' != *argv[1];
	if (randomize_shuffle) {
		std::srand((unsigned int)std::time(0));
	}
	else if ('x'==*argv[1] && isdigit(argv[1][1])){
		shufflect = atoi(&argv[1][1]);
	}
	// std::cout << "randomize: " << randomize_shuffle << std::endl;
	Game g;
    
    Deck d2(randomize_shuffle);
    g.start(d2.shuffle().shuffle(shufflect));
    return 0;
}
