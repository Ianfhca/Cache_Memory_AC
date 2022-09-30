#include <stdio.h>
#include <string.h>

int cmSize = 32; // Default wordSize*blockSize
int blockSize = 8; // Default 8 words
int wordSize = 4; // Default 4 bytes
int setSize = 1; // (Default) 1 = Direct-Map,  8 = Fully Associative, 2 or 4 = Set Associative
int repPolicy = 0; // (Default) 0 = FIFO and 1 = LRU

int direction = 0; // MM direction (@)
int op = 0; // 0 = LD and 1 = ST

int hits = 0; // Number of hits.
int misses = 0; // Number of misses.
float hitrate = 0; // hits/hits+misses
int accessTime = 0; // Program total access time 

int Tcm = 2, Tmm = 21, Tbuff = 1; // Access times

void printCache();
void createCache();
void modifyBlock();
// void updateFIFO();
// void updateLRU();

int main () {
    printf("Insert word size: ");
    //scanf("%d", &wordSize);

    printf("Insert set size: ");
    scanf("%d", &setSize);

    printf("Insert replacement policy: ");
    //scanf("%d", &repPolicy);

    cmSize = wordSize * blockSize;
    printf("\n\nSIMULATED CACHE MEMORY:");
    printf("\nTotal size: %d B - Words: %d - Blocks: %d - ", cmSize, wordSize, blockSize);

    if (setSize == 1) {
        printf("Type: Direct-Map - ");
    } else if (setSize == 8) {
        printf("Type: Fully Associative - ");
    } else {
        printf("Type: Set Associative of %d sets - ", setSize);
    }

    if (repPolicy == 0) {
        printf("Policy: FIFO\n\n");
    } else if (repPolicy == 1) {
        printf("Policy: LRU.\n\n");
    }
    
    int word = 0;
    int blockMP = 0;
    int tag = 0;
    int blockMC = 0;
    int set = 0;

    int cm[blockSize][5];
    createCache(cm);

    while(1) {        
        printf("\nIntroduce direction: ");
        scanf("%d", &direction);

        printf("Select read(0) or write(1) operation: ");
        scanf("%d", &op);

        modifyBlock(cm, direction, op);
        printCache(cm);
        
        printf("\nTotal entries: %d - Hits: %d - Misses: %d \nHit rate: %.2f - Access time: %d cycles\n", hits+misses, hits, misses, hitrate, accessTime);
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
void modifyBlock(int cm[blockSize][5], int direction, int op) {
    int word = 0;
    int blockMP = 0;
    int tag = 0;
    int blockMC = 0;
    int set = 0;

    word = direction/wordSize;
    blockMP = direction/cmSize;

    if (setSize == 1) { // Direct-Map -----------------------------------------------------------------
        tag = blockMP/blockSize;
        blockMC = blockMP%blockSize;

        if (cm[blockMC][0] == 0) { // Miss (block transfer from MM)
            cm[blockMC][0] = 1;
            cm[blockMC][2] = tag;
            cm[blockMC][4] = blockMP;
            accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            misses++;
        } else {
            if (cm[blockMC][2] == tag && op == 0) { // Hit (reading current block)
                accessTime += Tcm;
                hits++;
            } else if (cm[blockMC][2] == tag && op == 1) { // Hit + Dirty (writing current block)
                cm[blockMC][1] = 1; // Dirty
                accessTime += Tcm;
                hits++;
            } else {  // Miss 
                if (cm[blockMC][1] == 1) { // Block Replacement
                    cm[blockMC][1] = 0; // Dirty Clean
                    accessTime += Tcm + 2*(Tmm+(blockSize-1)*Tbuff);
                } else { // Block Transfer
                    accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
                }

                cm[blockMC][0] = 1;
                cm[blockMC][2] = tag;
                cm[blockMC][4] = blockMP;
                misses++;
            }
        }
    } else if (setSize == 8) { // Fully Associative -----------------------------------------------------
        int i = 0;
        int empty = -1, hit = 0;
        int max = -1, firstOut = 0;

        tag = direction/cmSize;

        while (hit == 0 && i < blockSize) { // Looks if there is some gap in CM
            if (cm[i][0] == 1) {
                if (cm[i][2] == tag) {
                    hit = 1;
                } else {
                    if (cm[i][3] > max) {
                        firstOut = i;
                        max = cm[i][3];
                    }
                }
            } else {
                if (empty == -1) {
                    empty = i;
                }
            }
            i++;
        }
        printf("bucle terminado\n");
        if (hit == 1) {
            if (op == 0) { // Hit (reading current block)
                accessTime += Tcm;
                hits++;
            } else { // Hit + Dirty (writing current block)
                cm[i-1][1] = 1; // Dirty
                accessTime += Tcm;
                hits++;    
            }
        } else if (empty != -1) { // There is a gap
            int j = 0;
            if (repPolicy == 0) { // FIFO Policy
                //cm[empty][3] = 0;
                while (j < blockSize) {
                    if (cm[j][0] == 1) {
                        cm[j][3] = (cm[j][3]+1) % blockSize;
                    }
                    j++;
                }
            } else { // LRU Policy

            }
            cm[empty][0] = 1;
            cm[empty][2] = tag;
            //cm[empty][3] = 0;
            cm[empty][4] = blockMP;
            accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            misses++;
        } else { // CM is full
            int j = 0;
            if (repPolicy == 0) { // FIFO Policy
                //cm[firstOut][3] = 0;
                while (j < blockSize) {
                    if (cm[j][0] == 1) {
                        cm[j][3] = (cm[j][3]+1) % blockSize;
                    }
                    j++;
                }
            } else { // LRU Policy

            }
            
            if (cm[firstOut][1] == 1) {
                cm[firstOut][1] = 0; // Dirty Clean
                accessTime += Tcm + 2*(Tmm+(blockSize-1)*Tbuff);
            } else {
                accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            }
            cm[firstOut][0] = 1;
            cm[firstOut][2] = tag;
            //cm[firstOut][3] = 0;
            cm[firstOut][4] = blockMP;
            misses++;
        }
    } else { // Set Associative -------------------------------------------------------------------------

    }

    hitrate = (float)hits/(float)(hits+misses);
}