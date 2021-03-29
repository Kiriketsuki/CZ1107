#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 37
#define PRIME     13

enum Marker {EMPTY,USED};

typedef struct _slot{
    int key;
    enum Marker indicator;
    int next;
} HashSlot;

int HashInsert(int key, HashSlot hashTable[]);
int HashFind(int key, HashSlot hashTable[]);

int hash(int key)
{
    return (key % TABLESIZE);
}

int main()
{
    int opt;
    int i;
    int key;
    int index;
    HashSlot hashTable[TABLESIZE];

    for(i=0;i<TABLESIZE;i++){
        hashTable[i].next = -1;
        hashTable[i].key = 0;
        hashTable[i].indicator = EMPTY;
    }

    printf("============= Hash Table ============\n");
    printf("|1. Insert a key to the hash table  |\n");
    printf("|2. Search a key in the hash table  |\n");
    printf("|3. Print the hash table            |\n");
    printf("|4. Quit                            |\n");
    printf("=====================================\n");

    printf("Enter selection: ");
    scanf("%d",&opt);
    while(opt>=1 && opt <=3){
        switch(opt){
        case 1:
            printf("Enter a key to be inserted:\n");
            scanf("%d",&key);
            index = HashInsert(key,hashTable);
            if(index <0)
                printf("Duplicate key\n");
            else if(index < TABLESIZE)
                printf("Insert %d at index %d\n",key, index);
            else
                printf("Table is full.\n");
            break;
        case 2:
            printf("Enter a key for searching in the HashTable:\n");
            scanf("%d",&key);
            index = HashFind(key, hashTable);

            if(index!=-1)
                printf("%d is found at index %d.\n",key,index);
            else
                printf("%d is not found.\n",key);
            break;

        case 3:
            printf("index:\t key \t next\n");
            for(i=0;i<TABLESIZE;i++) printf("%d\t%d\t%d\n",i, hashTable[i].key,hashTable[i].next);
            break;
        }
        printf("Enter selection: ");
        scanf("%d",&opt);
    }
    return 0;
}

int HashInsert(int key, HashSlot hashTable[])
{
    int i = 0;
    int hash_to_update = hash(key);
    int prev;

    for (i; i < TABLESIZE; i++) {
        int hash_address = hash(key+i);

        if (hashTable[hash_address].indicator == EMPTY) { // insert
            hashTable[hash_address].key = key;
            hashTable[hash_address].indicator = USED;

            if (hash_to_update != hash_address) { // preventing pointing to itself
                while (hash_to_update != -1) { // goes through the chain of nexts
                    prev = hash_to_update;
                    hash_to_update = hashTable[hash_to_update].next;
                }

                hashTable[prev].next = hash_address;
            }

            return hash_address;
        } else if (hashTable[hash_address].key == key) {
            return -1;
        }
    }

    return i;
}

int HashFind(int key, HashSlot hashTable[])
{
    int hash_address = hash(key);

    while (hash_address != -1) {
        if (hashTable[hash_address].indicator == EMPTY) {
            return -1;
        } else if (hashTable[hash_address].key == key) {
            return hash_address;
        } else {
            hash_address = hashTable[hash_address].next;
        }
    }

    return -1;
}
