/// @author Will Serface
/// @professor Prof. N. Torosdagli
/// @class COP3502C.0025
/// @date 11 April 2024
/// @file lab_9.c
/// @lab 9
/// @include input.txt

#include <stdio.h> // Standard Input/Output Library
#include <stdlib.h> // Standard Utilities Library

#define HASH_MOD 26 // Modulo for Hash Function

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
    return x % HASH_MOD; // Returns x mod HASH_MOD
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

int iSqRt(int x) {
    switch (x) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 4:
            return 2;
        case 9:
            return 3;
        case 16:
            return 4;
        case 25:
            return 5;
        case 36:
            return 6;
        case 49:
            return 7;
        case 64:
            return 8;
        case 81:
            return 9;
        case 100:
            return 10;
        case 121:
            return 11;
        case 144:
            return 12;
        case 169:
            return 13;
        default:
            return -1;
    }
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
    int i;
    printf("Hash Table:\n%5s | %5s | %5s | %5s | %5s | %5s\n%s\n", "INDEX", "HASH", "DELTA", "ID", "CHAR", "ORDER",
           "------+-------+-------+-------+-------+------"); // Print Header
    for (i = 0; i < hashSz; ++i) {
        if (pHashArray->records[i] != NULL) { // Check if Entry is NULL
            struct RecordType entry = *pHashArray->records[i]; // Current Entry
            printf("%5d | %5d | %3d^2 | %5d | %5c | %5d\n", i, hash(entry.id), iSqRt(i - hash(entry.id)),
                   entry.id, entry.name, entry.order); // Print Hash Index and Entry
        } else printf("%5d |       |       |       | %5s |\n", i, "NULL"); // Print Hash Index and NULL
    }
}

void insertRecord(struct HashType *, struct RecordType *); // Insert a new Record into Hash Table

int main(void) {
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Will Serface's Hash Table Implementation

    // Main Table for Hashes
    struct HashType hashTable = {0, calloc(0, 0)};

    // Add all Records to Hash Table
    for (int i = 0; i < recordSz; ++i) insertRecord(&hashTable, &pRecords[i]);

    // Display Hash Table
    displayRecordsInHash(&hashTable, hashTable.slots);

    // Free Dynamically Allocated Memory
    free(hashTable.records);
    free(pRecords);

    // Successful Exit
    return 0;
}

void insertRecord(struct HashType *table, struct RecordType *record) {
    int recordHash = hash(record->id), // Hash of current Record
    index, // Index to insert Record into Hash Table
    i = 0; // Loop Counter
    do {
        index = recordHash + i * i;
        i++;
    } while (index < table->slots && table->records[index] != NULL);
    if (index + 1 >= table->slots) {
        int copy = table->slots; // Index to copy table to
        table->slots = index + 1; // Update number of Slots in Hash Table
        table->records = realloc(table->records, (table->slots) * sizeof(struct RecordType *));
        for (i = copy; i < table->slots; ++i) table->records[i] = NULL;
    }
    table->records[index] = record;
}