#include <stdio.h>
#include <stdlib.h> // for malloc, free

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node structure for linked list
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Hash table structure
struct HashType
{
    struct Node** buckets; // Array of pointers to linked lists
    int size; // Size of the hash table
};

// Compute the hash function
int hash(int x, int size)
{
    // Ensure x is non-negative
    if (x < 0)
        x = -x;
    return x % size; // Simple mod hashing
}

// Initialize hash table
struct HashType* createHashTable(int size)
{
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType));
    hashTable->size = size;
    hashTable->buckets = (struct Node**)malloc(sizeof(struct Node*) * size);
    for (int i = 0; i < size; i++)
        hashTable->buckets[i] = NULL;
    return hashTable;
}

/// Insert a record into the hash table
void insert(struct HashType* hashTable, struct RecordType data)
{
    // Calculate the hash value
    int index = hash(data.id, hashTable->size);
    printf("Inserting record with id %d at index %d\n", data.id, index); // Debug print

    // Ensure the hash index is within the range of the hash table size
    if (index < 0 || index >= hashTable->size) {
        printf("Error: Invalid hash index %d\n", index);
        return;
    }

    // Verify if hashTable and buckets are properly initialized
    if (hashTable == NULL || hashTable->buckets == NULL) {
        printf("Error: Hash table or buckets are not properly initialized\n");
        return;
    }

    // Create a new node for the record
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    // Insert the new node into the appropriate bucket
    if (hashTable->buckets[index] == NULL)
        hashTable->buckets[index] = newNode;
    else
    {
        // If the bucket already has elements, insert at the head
        newNode->next = hashTable->buckets[index];
        hashTable->buckets[index] = newNode;
    }
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* hashTable)
{
    printf("\nHash Table:\n");
    for (int i = 0; i < hashTable->size; i++)
    {
        printf("Index %d -> ", i);
        struct Node* current = hashTable->buckets[i];
        while (current != NULL)
        {
            printf("(%d, %c, %d) -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

// Parse input file to an array of records
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Print the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    int hashSize = 10; // You can choose any appropriate size
    struct HashType* hashTable = createHashTable(hashSize);

    // Insert records into the hash table
   for (int i = 0; i < recordSz; i++)
    insert(hashTable, pRecords[i]);

    // Display the hash table
    displayRecordsInHash(hashTable);

    // Clean up
    for (int i = 0; i < hashSize; i++)
    {
        struct Node* current = hashTable->buckets[i];
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->buckets);
    free(hashTable);

    free(pRecords);

    return 0;
}
