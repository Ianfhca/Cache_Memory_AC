#include <stdio.h>
#include <string.h>

int cmSize = 32; // Default wordSize*blockSize
int blockSize = 8; // Default 8 words
int wordSize = 4; // Default 4 bytes
int setSize = 1; // (Default) 1 = Direct-Map,  8 = fully associative, 2 or 4 = set associative
int repPolicy = 0; // 0 = FIFO and 1 = LRU



int direction = 0; // Default first @
int op = 0; // 0 = LD and 1 = ST



void printCache();
void createCache();
void modifyBlock();


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

        int word = 0;
        int blockMP = 0;
        int tag = 0;
        int blockMC = 0;
        int set = 0;


    int cm[blockSize][5];
    createCache(cm);


    while(1){

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
        tag = blockMP/blockSize;


        modifyBlock(cm, blockMP, blockMC , setSize, direction, tag);
        printCache(cm);
        

        
    }
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
            if (j < 4) {
                printf("%d ", cm[i][j]);
            } else {
                printf("||  %d ", cm[i][j]);
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

    int i = 0, j = 0, bussy = 0;
    printf("B D T R ||  B\n");
    printf("---------------\n");
    for (i; i < blockSize; i++) {
        if (cm[i][0] == 1){
            bussy = 1;
        }
        for (j; j < 5; j++) {
            if (j < 4) {
                printf("%d ", cm[i][j]);
            } else {
                if (bussy == 1){
                    printf("||  b%d", cm[i][j]);
                    bussy = 0;
                } else {
                printf("||  %d ", cm[i][j]);
                }
            }
        }
        printf("\n");
        j = 0;
    }
}


/*
Function that modificated the cache memory.
*/
void modifyBlock(int cm[blockSize][5], int blockMP, int blockMC , int setSize, int direction, int tag){
    if (setSize == 1) {
        if (cm[blockMC][0] == 0) { 
            cm[blockMC][0] = 1;
            cm[blockMC][2] = tag;
            cm[blockMC][4] = blockMP;
        } else {
            if (cm[blockMC][4] == blockMP) {
            //HIT MARKER
            } else {
            //MISS RUSSIA
            }
        } 





    } else if (setSize == 8) {
                    //fully associative
    } else {
                    //set associative
    }
}



