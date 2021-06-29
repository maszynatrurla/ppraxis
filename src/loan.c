/**
 * @file loan.c
 * @author maszynatrurla
 * @brief Programming exercise: Loan amortization calculator
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/12/loan-amortization/
 */

#include <stdio.h>
#include <math.h>

#define P       10000
#define RATE    0.07
#define T       36



void printLoanTable(int p, double rate, int months) {
    double i = rate / 12.;
    /* Annuity formula */
    int A = (double) p * (i + i / (pow(1. + i, months) - 1.));
    int wholes;
    int j;
    
    printf("%2s %9s %9s %9s\n", "m", "principal", "intrest", "balance");
    wholes = p / 100;
    printf(" 0                     %6d.%2d\n", wholes, p - 100*wholes);
    
    for (j = 1; j <= months; ++j) {
        int intrest = (p * i);
        int wholeInt = intrest / 100;
        int principal = A - intrest;
        int wholePri = principal / 100;
        int wholeBal;
        p -= principal;
        wholeBal = p / 100;
        printf("%2d %6d.%2d %6d.%2d %6d.%2d\n", j,
               wholePri, principal - 100*wholePri,
               wholeInt, intrest - 100*wholeInt,
               wholeBal, p - 100*wholeBal);
    }
}


int main(int argc, char ** argv) {
    
    printLoanTable(P * 100, RATE, T);
    
    return 0;
}
