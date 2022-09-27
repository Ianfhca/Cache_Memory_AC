#include <stdio.h>
#include <string.h>

int cmSize = 32; // Default wordSize*blockSize
int blockSize = 8; // Default 8 words
int wordSize = 4; // Default 4 bytes
int setSize = 1; // Default Direct-Map
int repPolicy = 0; // 0 = FIFO and 1 = LRU

int direction = 0; // Default first @
int op = 0; // 0 = LD and 1 = ST



void printCache();
void createCache();



int main () {
    printf("Insert word size: ");
    //scanf("%d", &wordSize);
    printf("%d\n", wordSize);
    cmSize = wordSize * blockSize;

    printf("Insert set size: ");
    //scanf("%d", &setSize);
    printf("%d\n", setSize);

    printf("Insert replacement policy: ");
    //scanf("%d", &repPolicy);
    if (repPolicy == 0) {
        printf("FIFO policy\n");
    } else if (repPolicy == 1) {
        printf("LRU policy\n");
    }
    printf("\n");
    


    int cm[blockSize][5];
    createCache(cm);

    int word = 0;
    int blockMP = 0;
    int tag = 0;
    int blockMC = 0;
    int set = 0;

    printf("Introduce direction: ");
    scanf("%d", &direction);

    printf("Introduce read (0) or write (1) operation: ");
    scanf("%d", &op);

    word = direction/wordSize;
    blockMP = direction/cmSize;

    blockMC = blockMP%blockSize;



    cm[blockMC][0] = 1;
    printCache(cm);
    

    return 0;
}



/*
Function that creates a memory cache empty.
*/
void createCache(int cm[blockSize][5]){

    int i = 0, j = 0;
    printf("B D T R ||  B\n");
    printf("---------------\n");
    for (i; i < blockSize; i++) {
        for (j; j < 5; j++) {
            cm[i][j] = 0;
            if (j == 3) {
                printf("%d ||  ", cm[i][j]);
            } else {
                printf("%d ", cm[i][j]);
            }
        }
        printf("\n");
        j = 0;
    }
}

/*
Function that print the memory cache.
*/
void printCache(int cm[blockSize][5]){

        int i = 0, j = 0;
    printf("B D T R ||  B\n");
    printf("---------------\n");
    for (i; i < blockSize; i++) {
        for (j; j < 5; j++) {
            if (j == 3) {
                printf("%d ||  ", cm[i][j]);
            } else {
                printf("%d ", cm[i][j]);
            }
        }
        printf("\n");
        j = 0;
    }
}