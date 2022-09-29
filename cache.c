#include <stdio.h>
#include <string.h>

int cmSize = 32; // Default wordSize*blockSize
int blockSize = 8; // Default 8 words
int wordSize = 4; // Default 4 bytes
int setSize = 1; // (Default) 1 = Direct-Map,  8 = fully associative, 2 or 4 = set associative
int repPolicy = 0; // 0 = FIFO and 1 = LRU
//Goñi
int hits = 0; // Number of hits.
int misses = 0; // Number of misses.
float hitrate = 0; // hits/hits+misses
int accessTime = 0; // Program total access time 
//
int direction = 0; // Default first @
int op = 0; // 0 = LD and 1 = ST

// Here are the access times:
// Tcm = 2c; Tmm = 21; Tbuff = 1c;
int Tcm = 2; int Tmm = 21; int Tbuff = 1;

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

        
        printf("\nIntroduce direction: ");
        scanf("%d", &direction);

        printf("Select read(0) or write(1) operation: ");
        scanf("%d", &op);

        word = direction/wordSize;
        blockMP = direction/cmSize;

        blockMC = blockMP%blockSize;
        tag = blockMP/blockSize;


        modifyBlock(cm, blockMP, blockMC , setSize, tag, direction, op, accessTime);
        printCache(cm);
        
        printf("\nTotal entries: %d - Hits: %d - Misses: %d \nHit rate: %.2f - Access time: %d", hits+misses, hits, misses, hitrate, accessTime);
        //printf("\nNumber of hits: %d \n", hits);
        //printf("Number of misses: %d \n", misses); 
    }

    return 0;
}



//Function that creates a memory cache empty.
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

//Function that print the memory cache.
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


//Function that modificated the cache memory.
void modifyBlock(int cm[blockSize][5], int blockMP, int blockMC , int setSize, int tag, int direction, int op, int accessTime) {
    if (setSize == 1) { // Direct-Map
        if (cm[blockMC][0] == 0) { // Miss (block transfer from MM)
            cm[blockMC][0] = 1;
            cm[blockMC][2] = tag;
            cm[blockMC][4] = blockMP;
            //accessTime += Tmm + (-1) * Tbuff;
            misses++;
        } else {
            if (cm[blockMC][4] == blockMP && op == 0) { // Hit (reading current block)
                accessTime += Tcm;
                hits++;
            } else if (cm[blockMC][4] == blockMP && op == 1) { // Hit + Dirty (writing current block)
                cm[blockMC][1] = 1;//dirty
                accessTime += Tcm;
                hits++;
            } else {  // Miss (block replacement)
                cm[blockMC][0] = 1;
                cm[blockMC][1] = 0;//dirty clean
                cm[blockMC][2] = tag;
                cm[blockMC][4] = blockMP;
                accessTime += Tmm + (-1) * Tbuff;
                misses++;
            }//Falta trasferir los datos a la MP
        }





    } else if (setSize == 8) {
                    //fully associative
    } else {
                    //set associative
    }
}



