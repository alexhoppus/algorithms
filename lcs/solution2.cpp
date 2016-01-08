/**
 * @author  Quickgrid ( Asif Ahmed )
 * @link    https://quickgrid.wordpress.com
 * Problem: UVA 10405 Longest Common Subsequence ( LCS )
 * Method:  Memorized Recursive / Top Down Solution
 */
 
#include<stdio.h>;
#include<string.h>;
#define SIZE 1024
 
static char x[SIZE], y[SIZE];
static int lcs[SIZE][SIZE];
 
int maxval(int a, int b){
    return a > b ? a : b;
}
 
/*
 * If the value is not -1 then it means the value of that sub problem is
 * already calculated. Just return that calculated value
 * If both of the characters in string are same then we can reduce both string size by 1 length and calculate rest
 * Else among sub problems by reducing one string and keeping the other one same find the one with the max length
 */
int LCS(int i, int j){
    if(lcs[i][j] != -1)
        return lcs[i][j];
 
    if(x[i-1] == y[j-1])
        lcs[i][j] = LCS(i-1, j-1) + 1;
    else
        lcs[i][j] = maxval( LCS(i-1, j), LCS(i, j-1) );
 
    return lcs[i][j];
}
 
int main(){
    register unsigned i, j;
    while(gets(x) && gets(y)){
 
        int xlen = strlen(x);
        int ylen = strlen(y);
 
        /*
         * Set -1 to all positions to indicate there are no calculated value
         */
        for(i = 1; i <= xlen; ++i)
            for(j = 1; j <= ylen; ++j)
                lcs[i][j] = -1;
 
        printf("%d\n", LCS(xlen,ylen) );
 
    }
    return 0;
}
