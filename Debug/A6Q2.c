#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 37
#define PRIME     13

enum Marker {EMPTY,USED,DELETED};

typedef struct _slot{
    int key;
    enum Marker indicator;
} HashSlot;

int HashInsert(int key, HashSlot hashTable[]);
int HashDelete(int key, HashSlot hashTable[]);


int hash1(int key);
int hash2(int key);

int main()
{
    int opt;
    int i;
    int key;
    int comparison;
    HashSlot hashTable[TABLESIZE];

    for(i=0;i<TABLESIZE;i++){
        hashTable[i].indicator = EMPTY;
        hashTable[i].key = 0;
    }

    printf("============= Hash Table ============\n");
    printf("|1. Insert a key to the hash table  |\n");
    printf("|2. Delete a key from the hash table|\n");
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
            comparison = HashInsert(key,hashTable);
            if(comparison <0)
                printf("Duplicate key\n");
            else if(comparison < TABLESIZE)
                printf("Insert: %d Key Comparisons: %d\n",key, comparison);
            else
                printf("Key Comparisons: %d. Table is full.\n",comparison);
            break;
        case 2:
            printf("Enter a key to be deleted:\n");
            scanf("%d",&key);
            comparison = HashDelete(key,hashTable);
            if(comparison <0)
                printf("%d does not exist.\n", key);
            else if(comparison <= TABLESIZE)
                printf("Delete: %d Key Comparisons: %d\n",key, comparison);
            else
                printf("Error\n");
            break;
        case 3:
            for(i=0;i<TABLESIZE;i++) printf("%d: %d %c\n",i, hashTable[i].key,hashTable[i].indicator==DELETED?'*':' ');
            break;
        }
        printf("Enter selection: ");
        scanf("%d",&opt);
    }
    return 0;
}

int hash1(int key)
{
    return (key % TABLESIZE);
}

int hash2(int key)
{
    return (key % PRIME) + 1;
}

int HashInsert(int key, HashSlot hashTable[])
{
    int index;
    int newindex;
    int comp=0;
    int count = 0;
    int newkey;
    int firstdelpos = -100;

    // index = hash1(key);
    newindex = hash2(key);
    while (count != TABLESIZE)
    {
        newkey = key + (count*newindex);
        index = hash1(newkey);

        if (hashTable[index].indicator == USED) // entered used
        {
            comp++; // increase comp

            if (hashTable[index].key == key)
            {
                return -1;
            }
        }

        else
        {
            if (hashTable[index].indicator == EMPTY)
            {
                if (firstdelpos == -100) // no previous and first empty
                {
                    hashTable[index].key = key;
                    hashTable[index].indicator = USED;
                    return comp;
                }

                else //else - if have previous deleted, but the key in that deleted address at the end
                {// have previous deleted slot, put it in that instead
                    // printf("entered\n");
                    hashTable[firstdelpos].key = key;
                    hashTable[firstdelpos].indicator = USED;
                    return comp;
                    // break;
                }
            }

            else // indicator == DELETED
            { // only change firstdepos for the first time - put key in first del pos
                if (firstdelpos == -100)
                {
                    firstdelpos = index;
                }

            }   
            
        }
        count++;
    }

    if (firstdelpos != -100) 
    { // IF OUT OF BOUNDS OR FIRST EMPTY!!!!! MOVING THIS IN TO FIRST EMPTY MISSES THE OUT OF BOUNDS
        hashTable[firstdelpos].key = key;
        hashTable[firstdelpos].indicator = USED;
        return comp;
    }

    return count;
}

// int HashInsert(int key, HashSlot hashTable[])
// {
//     int index;
//     int newindex;
//     int comp=0;
//     int count = 0;
//     int newkey;
//     int firstdelpos = -100;

//     // index = hash1(key);
//     newindex = hash2(key);
//     while (count != TABLESIZE)
//     {
//         newkey = key + (count*newindex);
//         index = hash1(newkey);

//         if (hashTable[index].indicator == USED) // entered used
//         {
//             comp++; // increase comp

//             if (hashTable[index].key == key)
//             {
//                 return -1;
//             }
//         }

//         else if (hashTable[index].indicator == EMPTY) {
//             if (firstdelpos == -100) { // no previous and first empty
//                     hashTable[index].key = key;
//                     hashTable[index].indicator = USED;
//                     return comp;
//                 }

//             else //else - if have previous deleted, but the key in that deleted address at the end
//             {// have previous deleted slot, put it in that instead
//                     hashTable[firstdelpos].key = key;
//                     hashTable[firstdelpos].indicator = USED;
//                     return comp;
//                     // break; // moved above to down by why does this work??????
//                 }
//         } else { // deleted
//             if (firstdelpos == -100) {
//                 firstdelpos = index;
//             }

//         }
       
//         count++;
//     }

//     // if (firstdelpos != -100) {
//     //     hashTable[firstdelpos].key = key;
//     //     hashTable[firstdelpos].indicator = USED;
//     //     return comp;
//     // }
//     return count;
// }


int HashDelete(int key, HashSlot hashTable[])
{
    int index;
    int newindex;
    int count = 0;
    int checked=0;
    int newkey;
    
    // while (count<TABLESIZE)
    // {
    //     if (hashTable[count].key != key)
    //     {
    //         checked++;
    //     }
    //     count++;
    // }

    index = hash1(key);
    newindex = hash2(key);
    while (count<= TABLESIZE-1)
    {
        newkey = key + (count*newindex);
        index = hash1(newkey);
        
        if(hashTable[index].key == key && hashTable[index].indicator == USED)
        {
            hashTable[index].indicator = DELETED;
            return ++count;
        }

        // else if (hashTable[index].key == key && hashTable[index].indicator == DELETED)
        // {
        //     return -1;
        // }
        
        count++;
    }

    return -1;

}
