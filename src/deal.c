//
// Created by Zalet on 2018/2/2.
//
#include "../inc/deal.h"

void deal(int key,int* martrix)
{
    int df,k=0,i,j,at[4][4];
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        {
            at[i][j]=*(martrix+k);
            k++;
        }

    switch(key)
    {
        case 75:
        {
            /** \brief 左移
             *
             * \param k=0;k<4;k++
             *
             */
            for(k=0;k<4;k++)
            {
                for(i=0;i<4;i++)
                {
                    df=0;
                    for(j=i+1;j<4;j++)
                    {
                        if(at[k][j]==at[k][i]&&at[k][i]!=0&&df!=1)
                        {
                            at[k][i]=at[k][i]+at[k][j];
                            at[k][j]=0;
                            df=1;
                        }
                        else break;
                    }
                }


                for(i=0;i<3;i++)
                {
                    df=0;
                    if(at[k][i]==0)
                        for(j=i+1;j<4;j++)
                        {
                            if(at[k][j]!=0&&df==0)
                            {
                                at[k][i]=at[k][j];
                                at[k][j]=0;
                                df=1;
                            }
                        }
                }
            }
        }break;


        case 77:
        {

            /** \brief  右移
             *
             * \param
             * \param
             * \return
             *
             */
            for(k=3;k>=0;k--)
            {
                for(i=3;i>=0;i--)
                {
                    df=0;
                    for(j=i-1;j>=0;j--)
                    {
                        if(at[k][j]==at[k][i]&&at[k][i]!=0&&df!=1)
                        {
                            at[k][i]=at[k][i]+at[k][j];
                            at[k][j]=0;
                            df=1;
                        }
                        else break;
                    }
                }


                for(i=3;i>=0;i--)
                {
                    df=0;
                    if(at[k][i]==0)
                        for(j=i-1;j>=0;j--)
                        {
                            if(at[k][j]!=0&&df==0)
                            {
                                at[k][i]=at[k][j];
                                at[k][j]=0;
                                df=1;
                            }
                        }
                }
            }
        }break;


        case 72:
        {
            /** \brief 上移
             *
             * \param
             * \param
             * \return
             *
             */
            for(k=0;k<4;k++)
            {
                for(i=0;i<4;i++)
                {
                    df=0;
                    for(j=i+1;j<4;j++)
                    {
                        if(at[j][k]==at[i][k]&&at[i][k]!=0&&df!=1)
                        {
                            at[i][k]=at[i][k]+at[j][k];
                            at[j][k]=0;
                            df=1;
                        }
                        else break;
                    }
                }


                for(i=0;i<3;i++)
                {
                    df=0;
                    if(at[i][k]==0)
                        for(j=i+1;j<4;j++)
                        {
                            if(at[j][k]!=0&&df==0)
                            {
                                at[i][k]=at[j][k];
                                at[j][k]=0;
                                df=1;
                            }
                        }
                }
            }
        }break;

        case 80:
        {
            /** \brief 下移
             *
             * \param
             * \param
             * \return
             *
             */


            for(k=3;k>=0;k--)
            {
                for(i=3;i>=0;i--)
                {
                    df=0;
                    for(j=i-1;j>=0;j--)
                    {
                        if(at[j][k]==at[i][k]&&at[i][k]!=0&&df!=1)
                        {
                            at[i][k]=at[i][k]+at[j][k];
                            at[j][k]=0;
                            df=1;
                        }
                        else break;
                    }
                }


                for(i=3;i>=0;i--)
                {
                    df=0;
                    if(at[i][k]==0)
                        for(j=i-1;j>=0;j--)
                        {
                            if(at[j][k]!=0&&df==0)
                            {
                                at[i][k]=at[j][k];
                                at[j][k]=0;
                                df=1;
                            }
                        }
                }
            }
        }
    }

    k=0;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        {
            *(martrix+k)=at[i][j];
            k++;
        }



}
