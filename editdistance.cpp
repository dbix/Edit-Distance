/*
 * Prog Name:  Edit Distance Algorithm
 * Summary:    Uses dynamic programming techniques to compare the similarity of
 *             two strings.
 * Created on: Apr 28, 2014
 * Author:     Derek Bixler
 */

/* Header and implementation in the same file */
#pragma header "editdistance.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* The algorithm doesn't use more than MIN_3 but I found it funny that you can
   make the syntax easier to read the more complex the comparisons get. */
#define MIN_6(a,b,c,d,e,f) (MIN_3(MIN_2(a,b), MIN_2(c,d), MIN_2(e,f)))
#define MIN_5(a,b,c,d,e)   (MIN_2(MIN_3(a,b,c), MIN_2(d,e)))
#define MIN_4(a,b,c,d)     (MIN_2(MIN_2(a,b), MIN_2(c,d)))
#define MIN_3(a,b,c)       ((a)<(b) ? ((a)<(c) ? (a):(c)):((b)<(c) ? (b):(c)))
#define MIN_2(a,b)         ((a)<(b) ? (a):(b))

/* For basic humans who can't read spaghetti */
typedef enum {
	COPY, TWIDDLE, REPLACE, INSERT, DELETE, KILL
} Costs;

/* Contains a matrix of characters used throughout the comparisons */
int **c;

/* Input:  Two strings x,y, their lengths m,n, and a variable array
           containing the various costs of each edit.
   Output: The least number of edits to x required for x == y. */
int EditDist(char *x, char *y, int m, int n, double cost[6]);


/* Header and implementation in the same file */
#pragma implementation "editdistance.h"

/* 
 * c[i-1][j] is a deletion of a letter,
 * c[i][j-1] is an insertion of a new letter,
 * c[i-1][j-1] is a copy of a letter from x to y,
 * c[i-1][j-1] is a replacement of a letter if x[i] != y[j], and
 * c[i-2][j-2] is a twiddly, change-y, swap-y thing.
 */
int EditDist(char *x, char *y, int m, int n, double cost[6])
{
    int i, j;
    char c[m+1][n+1];
    int seq[n];
    c[0][0] = 0;
    for (i = 1; i < m+1; i++) {
        c[i][0] = i;
    }
    for (j = 1; j < n+1; j++) {
        c[0][j] = j * cost[INSERT];
    }
    
    /* Sigh */
    for (i = 1; i <= n + 1; i++) {
        for (j = 1; j <= m + 1; j++) {
            if (x[i-1] == y[j-1]) {
                c[i][j] = c[i-1][j-1] + cost[COPY];
            } else if(i > 2 && j > 2) {
                c[i][j] = MIN_3(c[i-2][j-2] + cost[TWIDDLE],
                                c[i-1][j-1] + cost[REPLACE],
                                c[i-1][j]   + cost[DELETE]);
            } else {
                c[i][j] = MIN_2(c[i-1][j-1] + cost[REPLACE],
                                c[i-1][j] + cost[DELETE]);
            }
        }
    }
    
    /* This mess prints the edit distance, other results of mediocre
       importance, and a fancy little table to stdout. */
    printf("   \\\n    \\\n     \\ y[j]  ");
	for (int i = 0; i < n; i++) printf("%c   ", *(y+i));
	printf("\n      \\");
	for (int i = 0; i < m+1; i++) {
	    printf("____");
	}
	printf("\n x[i] |  0   ");
	for (int i = 1; i < n+1; i++) {
	    printf("%i   ", c[i][0]);
	}
	for (int i = 1; i < m +1; i++) {
	    printf("\n      |\n   %c  |  %i   ", *(x+i-1), i);
	    for (int j = 1; j < n + 1; j++) {
	        printf("%i   ", c[i][j]);
	    }
	}
    i = 0;
    j = 0;

    /* Sigh, still printing */
    printf("\n\n\nEdit Distance:\t%i\nSequence:\t\t ", c[m][n]);
    while (i < m) {
        if (x[i] == y[i]) {
            printf("Copy ");
            seq[i] = COPY;
            i++; j++;
        } else if(x[i] == y[j+1] && x[i+1] == y[j]) {
            printf("Twiddle ");
            seq[i] = TWIDDLE;
            i+=2;
            j+=2;
        } else if(i > 2 && j > 2) {
            int min = MIN_3(c[i+1][j+1], c[i][j+1], c[i+1][j]);
            if (min == c[i+1][j+1]) {
                printf("Replace ");
                seq[i] = REPLACE;
                j++;
                i++;
            } else if(min == c[i][j+1]) {
                printf("Insert ");
                seq[i] = INSERT;
                j++;
            } else if (min == c[i+1][j]) {
                printf("Delete ");
                seq[i] = DELETE;
                i++;
            }
        } else {
            fflush(stdout);
            i = m + 1;
            break;
        }
    }
    /* What you really came here for */
    return c[m][n];
}

int main(int argc, char **argv)
{
	char *x, *y;
    int distance, m, n;
    double C[] = {0,1,1,1,1,1};
    x = "gambol";
    y = "gmaco";
    m = 6;
    n = 5;
	distance = EditDist(x, y, m, n, C);
	return 0;
}
