//
// Created by Zalet on 2018/2/2.
//

#include "../inc/random.h"

int randn()
{
    int m;
    srand((unsigned)time(NULL));
    m=rand()%10;
    if(m<5)
        return 2;
    else if(m>5)return 4;
    else return 0;
}

int randlx()
{
    int lx;
    srand((unsigned)time(NULL));
    lx=rand()%4;
    return lx;
}

int randly()
{
    int ly;
    srand((unsigned)time(NULL));
    ly=rand()%4;
    return ly;
}
