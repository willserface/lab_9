/// @author Will Serface
/// @professor Prof. N. Torosdagli
/// @class COP3502C.0025
/// @date 11 April 2024
/// @file lab_9.c
/// @lab 9
/// @include input.txt

#include <stdio.h> // Standard Input/Output Library
#include <stdlib.h> // Standard Utilities Library

#define HASH_SIZE 9 // Modulo for Hash Function

// RecordType
struct RecordType {
    int id;
    char name;
    int order;
};

// Fill out this structure
struct HashType {
    int slots; // Stores number of Slots allocated for Records in Hash Table
    struct RecordType **records; // Array of Record Pointers, indexed by Hash
};

// Compute the hash function
int hash(int x) {
    return x % HASH_SIZE; // Returns x mod HASH_SIZE
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData) {
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
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

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
    int i;
    printf("Hash Table:\n"); // Print Header
    for (i = 0; i < hashSz; ++i) {
        printf("Hash %d: {", i);
        for (int j = 0; j < pHashArray[i].slots; ++j) {
            struct RecordType entry = *pHashArray[i].records[j];
            printf("[%d %c %d]%s", entry.id, entry.name, entry.order, (j + 1 == pHashArray[i].slots) ? "" : ", ");
        }
        printf("}\n");
    }
}

void insertRecord(struct HashType *, struct RecordType *); // Insert a new Record into Hash Table

int main(void) {
    struct RecordType *pRecords;
    int recordSz;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Will Serface's Hash Table Implementation

    // Main Table for Hashes
    struct HashType hashTable[HASH_SIZE] = {};

    // Add all Records to Hash Table
    for (int i = 0; i < recordSz; ++i) insertRecord(hashTable, &pRecords[i]);

    // Display Hash Table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free Dynamically Allocated Memory
    for (int i = 0; i < HASH_SIZE; ++i) free(hashTable[i].records);
    free(pRecords);

    // Successful Exit
    return 0;
}

void insertRecord(struct HashType *table, struct RecordType *record) {
    int index = hash(record->id);
    table[index].records = realloc(table[index].records, ((table[index].slots) + 1) * sizeof(struct RecordType *));
    table[index].records[table[index].slots++] = record;
}