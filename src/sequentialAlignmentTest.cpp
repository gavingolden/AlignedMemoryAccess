/**
   
 Created by Gavin Golden on 4/16/15.
 Copyright (c) 2015 Gavin Golden. All rights reserved.
 
 A simple program to test the efficiency of sequential (un)aligned word
 array access patterns.

 A series of tests has shown that runtime does not differ even with non-word
 aligned memory access.

 I believe this is due to block CPU caching that effectively negates any
 "advantages" that aligned access presents because cache misses will occur
 at the same rate for all offsets but on different iterations of the loop.
 
*/

#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include "GTimer.h"


#define ARR_SIZE 100000
#define DESIRED_OFFSET 4

/** Data type to be used for the test. */
typedef unsigned ValType;

/** Hold random data to access aligned/unaligned memory locations. */
std::vector<ValType> data(ARR_SIZE);


/** Fill vector with random values. */
void initData() {
    std::for_each(data.begin(), data.end(), [](ValType& ui) {
        ui = rand() % 10;
    });
}



/** Driver method.
 */
int main(int argc, const char * argv[]) {
    srand(1);

    /** Repeat the summation loop on a smallish vector rather than
        creating an enormous vector */
    const unsigned REPS = (argc == 2 ? atoi(argv[1]) : 10000);
    /** Cannot offset by a size greater than the num bytes in #ValType. */
    const int MAX_OFFSET = std::min(DESIRED_OFFSET, (int)sizeof(typeid(ValType)));
    
    // Initialize array with random values
    initData();
    
    // Perform some work so that the compiler doesn't do away with loops.
    unsigned long long sum = 0;
    
    // Perform array addition for various offsets beginning with zero
    for (int offset = 0; (offset < MAX_OFFSET); offset++)
    {
        ValType* curr = reinterpret_cast<ValType*>(reinterpret_cast<char*>(data.data()) + offset);
        std::cout << "Start address for offset [" << offset << "]: " << curr << " -----" << std::endl;
        
        GUtil::Timer timer;
        timer.start();
        for (size_t rep = 0; (rep < REPS); rep++)
        {
            curr = reinterpret_cast<ValType*>(reinterpret_cast<char*>(data.data()) + offset);
            if (rep == 0) {
                
            }
            for (size_t i = 0; (i < ARR_SIZE - 1); i++)
            {
                sum += ((*curr) % (rep + 1) == 0 ? 1 : 0);
                curr++;
            }
        }
        
        timer.end();
        std::cout << "Time --> " << timer << "\n\n";
    }
    std::cout << "Sum : " << sum << std::endl;
    return 0;
}
