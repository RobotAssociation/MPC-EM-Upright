#include <stdio.h>
#include <time.h>
#include <conio.h>
#include "inc/deal.h"

int main()
{
    int i,j,rf,rx,ry,key;

    int a[4][4]={0,0,0,0,
                 0,0,0,0,
                 0,0,0,0,
                 0,0,0,0};

    printmatrix(a);

    while((key=getkey())!=27)
    {
        deal(key, a);

        rf=0;
        for(i=0;i<4;i++)
            for(j=0;j<4;j++)
            {
                if(a[i][j]==0)
                    rf=1;break;
            }

        if(rf==1)
        {
            while(rf==1)
            {
                rx=randlx(),ry=randly();
                if(a[rx][ry]==0)
                {
                    a[rx][ry]=randn();
                    rf=0;
                }
            }
        }

        system("cls");

        printmatrix(a);
        printf("\n");
    }




}


void printaline(int a,int b,int c,int d)
{
    printf("//  "); printf("%d   ",a); printf("//  ");
    printf("//  "); printf("%d   ",b); printf("//  ");
    printf("//  "); printf("%d   ",c); printf("//  ");
    printf("//  "); printf("%d   ",d); printf("//  \n");
}

void printmatrix(int* martrix)
{
    //system("cls");
    printf("////////////////////////////////////////////////\n");
    printaline(*martrix,*(martrix+1),*(martrix+2),*(martrix+3));
    printaline(*(martrix+4),*(martrix+5),*(martrix+6),*(martrix+7));
    printaline(*(martrix+8),*(martrix+9),*(martrix+10),*(martrix+11));
    printaline(*(martrix+12),*(martrix+13),*(martrix+14),*(martrix+15));
    printf("////////////////////////////////////////////////\n");


}

int getkey()
{
    /**键值

    up 72  down 80  left 75   right 77

    w 119 W 87   s 115 S83   a 97 A65   d 100 D68

    *
    */

    if(getch()==224)
        return getch();

}

