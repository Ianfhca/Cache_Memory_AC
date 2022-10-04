/*
## TITLE: SIMPLE CACHE MEMORY SIMULATOR
#
#
## AUTORS: Ian Fernadez Hermida, Iker Goñi Perez.
#
## ENTITY: UPV/EHU.
## CAREER: Informatics Engineer.
## COURSE: Computer Architecture.
## PROFESSOR: Iñigo Perona Balda.
#
## DATE: 04/10/2022.
*/




#include <stdio.h>
#include <string.h>

int cmSize = 32; // Default wordSize*blockSize
int blockSize = 8; // Default 8 words
int wordSize = 0; // Minimun 4 bytes
int setSize = 1; // (Default) 1 = Direct-Map,  8 = Fully Associative, 2 or 4 = Set Associative
int repPolicy = 0; // (Default) 0 = FIFO and 1 = LRU

int direction = 0; // MM direction (@)
int op = 0; // 0 = LD and 1 = ST

int hits = 0; // Number of hits.
int misses = 0; // Number of misses.
float hitrate = 0; // hits/hits+misses
int accessTime = 0; // Program total access time 

int Tcm = 2, Tmm = 21, Tbuff = 1; // Access times

int error = 1; // error code


void printCache();
void createCache();
void modifyBlock();
void LRUPolicy();
void FIFOPolicy();
void FIFOPolicySet();
void LRUPolicySet();


int main () { 
  

    while (error == 1) {
        printf("Insert word size (On Bytes)  (4 or 8): ");
        scanf("%d", &wordSize);
        while(getchar() != '\n'); // will consume the charater
        
        if(wordSize == 4 || wordSize == 8) {
            error = 0;
        } else {
            printf("!! WRONG NUMBER INTRODUCED !!\n");
        }
    }
    error = 1;
    
   
    

    while (error == 1) {
        printf("\nInsert set size (1 = Direct-Map)   (2 or 4 = Set Associative)   (8 = Fully Associative): ");
        scanf("%d", &setSize);
        while(getchar() != '\n'); // will consume the charater
        if(setSize == 1 || setSize == 8 || setSize == 2 || setSize == 4) {
            error = 0;
        } else {
            printf("!! WRONG NUMBER INTRODUCED !!\n");
        }
    }
    error = 1;
    
    if (setSize !=1)
    {
        while (error == 1) {
            printf("\nInsert replacement policy (0 = FIFO)   (1 = LRU): ");
            scanf("%d", &repPolicy);
            while(getchar() != '\n'); // will consume the charater
            if(repPolicy == 0 || repPolicy == 1) {
                error = 0;
            } else {
                printf("!! WRONG NUMBER INTRODUCED !!\n");
            }
        }
    error = 1;
    }
    
    

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
    if (setSize != 1) {
        if (repPolicy == 0) {
            printf("Policy: FIFO\n\n");
        } else if (repPolicy == 1) {
            printf("Policy: LRU\n\n");
        }
    } else {
        printf("Policy: No Replacement Policy\n\n");
    }
    
    
    int word = 0;
    int blockMP = 0;
    int tag = 0;
    int blockMC = 0;
    int set = 0;

    int cm[blockSize][5];
    createCache(cm);

    while(direction != -1) {        
        printf("\nIntroduce direction (-1 exit): ");
        scanf("%d", &direction);
        while(getchar() != '\n'); // will consume the charater
        if (direction == -1) {
            break;
        } else {
            while (error == 1){
                printf("Select read(0) or write(1) operation: ");
                scanf("%d", &op);
                while(getchar() != '\n'); // will consume the charater
                if(op == 0 || op == 1){
                    error = 0;
                } else {
                    printf("!! WRONG NUMBER INTRODUCED !!\n");
                }
            }error = 1;

            modifyBlock(cm, direction, op);
            printCache(cm);
        
            printf("\nTotal entries: %d - Hits: %d - Misses: %d \nHit rate: %.2f - Access time: %d cycles\n", hits+misses, hits, misses, hitrate, accessTime);
        }
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
    } else if (setSize == blockSize) { // Fully Associative -----------------------------------------------------
        int i = 0;
        int empty = -1, hit = 0;
        int max = -1, firstOut = 0;
        int rem = setSize -1;
        tag = direction/cmSize;

        while (hit == 0 && i < blockSize) { // Looks if there is some gap in CM
            if (cm[i][0] == 1) {
                if (cm[i][2] == tag) {
                    hit = 1;
                    rem = cm[i][3];
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
           
            if (repPolicy == 0) {  // FIFO Policy
                FIFOPolicy(cm, hit);
            } else { // LRU Policy
                LRUPolicy(cm, hit, rem, blockMP, tag);
            }

        } else if (empty != -1) { // There is a gap
            if (repPolicy == 0) { // FIFO Policy
                FIFOPolicy(cm, hit);
            } else { // LRU Policy
                LRUPolicy(cm, hit, rem, blockMP, tag);
            }
            
            accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            cm[empty][0] = 1;
            cm[empty][2] = tag;
            cm[empty][4] = blockMP;
            misses++;

        } else { // CM is full
            if (repPolicy == 0) { // FIFO Policy
                FIFOPolicy(cm, hit);
            } else { // LRU Policy
                LRUPolicy(cm, hit, rem, blockMP, tag);
            }
            
            if (cm[firstOut][1] == 1) {
                cm[firstOut][1] = 0; // Dirty Clean
                accessTime += Tcm + 2*(Tmm+(blockSize-1)*Tbuff);
            } else {
                accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            }
            cm[firstOut][0] = 1;
            cm[firstOut][2] = tag;
            cm[firstOut][4] = blockMP;
            misses++;
        }
    } else { // Set Associative ------------------------------------------------------------------------- 
        int mSize = 0; // blockSize/setSize
        int i = 0; 
        int empty = -1, hit = 0;
        int max = -1, firstOut = 0;
        int rem = blockSize -1;

        tag = blockMP/setSize;
        mSize = blockSize / setSize;
        set = (blockMP%blockSize) % mSize; // Line of set
        i = set;

       while (hit == 0 && i < mSize*setSize){ // mSize*setSize = 8  
        if (cm[i][0] == 1) {
            if (cm[i][2] == tag) { //hit primer conjunto
            hit = 1;
            rem = cm[i][3];
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
        i = i + mSize; 
    }
        
        if (hit == 1) {
            if (op == 0) { // Hit (reading current block)
                accessTime += Tcm;
                hits++;
            } else { // Hit + Dirty (writing current block)
                cm[i- mSize][1] = 1; // Dirty
                accessTime += Tcm;
                hits++;    
            }
           
            if (repPolicy == 0) {  // FIFO Policy
                FIFOPolicySet(cm, hit, mSize, set, tag);
            } else { // LRU Policy
                LRUPolicySet(cm, hit, rem, blockMP, mSize, set, tag);
            }

        } else if (empty != -1) { // There is a gap
            if (repPolicy == 0) { // FIFO Policy
                FIFOPolicySet(cm, hit, mSize, set, tag);
            } else { // LRU Policy
                LRUPolicySet(cm, hit, rem, blockMP, mSize, set, tag);
            }
            
            accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            cm[empty][0] = 1;
            cm[empty][2] = tag;
            cm[empty][4] = blockMP;
            misses++;
        } else { // CM is full
            if (repPolicy == 0) { // FIFO Policy
                FIFOPolicySet(cm, hit, mSize, set, tag);
            } else { // LRU Policy
                LRUPolicySet(cm, hit, rem, blockMP, mSize, set, tag);
            }
            
            if (cm[firstOut][1] == 1) {
                cm[firstOut][1] = 0; // Dirty Clean
                accessTime += Tcm + 2*(Tmm+(blockSize-1)*Tbuff);
            } else {
                accessTime += Tcm + (Tmm+(blockSize-1)*Tbuff);
            }
            cm[firstOut][0] = 1;
            cm[firstOut][2] = tag;
            cm[firstOut][4] = blockMP;
            misses++;
        }

    }

    hitrate = (float)hits/(float)(hits+misses);
}


void LRUPolicy(int cm[blockSize][5], int hit, int rem, int blockMP, int tag) {
    if (hit == 1) { // Hit
        int j = 0;
        while (j < blockSize) {
            if (cm[j][0] == 1) { // Hit in thats position
                if (cm[j][3] < rem) {
                    cm[j][3] = (cm[j][3]+1) % blockSize;
                } else if (cm[j][2] == tag) { 
                    cm[j][3] = 0;
                }                
            }
            j++;
        }  
            
    } else { //miss
        int j = 0;
        while(j < blockSize) {
            if (cm[j][0] == 1) {
                if (cm[j][2] == tag) { 
                    cm[j][3] = 0;
                } else {
                    cm[j][3] = (cm[j][3]+1) % blockSize;
                }
            }
            j++;
        }
    }
}


void FIFOPolicy(int cm[blockSize][5], int hit) {
    if (hit == 0) { //miss
        int j = 0;
        while (j < blockSize) {
            if (cm[j][0] == 1) {
                cm[j][3] = (cm[j][3]+1) % blockSize;
            }
            j++;
        }
    }
 
}

void LRUPolicySet(int cm[blockSize][5], int hit, int rem, int blockMP, int mSize, int set, int tag) {
    if (hit == 1) { // Hit
        int j = set;
        while (j < mSize*setSize) {
            if (cm[j][0] == 1) { // Hit in thats position
                if (cm[j][3] < rem) {
                    cm[j][3] = (cm[j][3]+1) % setSize;
                } else if (cm[j][2] == tag) { 
                    cm[j][3] = 0;
                }                
            }
            j = j + mSize;
        }  
            
    } else { //miss
        int j = set;
        while(j < mSize*setSize) {
            if (cm[j][0] == 1) {
                if (cm[j][2] == tag) { 
                    cm[j][3] = 0;
                } else {
                    cm[j][3] = (cm[j][3]+1) % setSize;
                }
            }
            j = j + mSize;
        }
    }
}

void FIFOPolicySet(int cm[blockSize][5], int hit, int mSize, int set, int tag) {
    if (hit == 0) { //miss
        int j = set;
        while (j < mSize*setSize) {
            if (cm[j][0] == 1) {
                cm[j][3] = (cm[j][3]+1) % setSize;
            }
            j = j + mSize;
        }
    }
 
}

