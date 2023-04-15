#include <stdio.h>
#include <stdlib.h>

// RecordType, contains pointer to collision records
struct RecordType
{
    int id;
    char name;
    int order;
    //for Linked List collision structure
	struct RecordType* next;
};

// Hash table
struct HashType
{
	struct RecordType* record;
};

// Hash function: x mod tableSize
int hash(int x, int tableSize)
{
	return x % tableSize;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
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
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
	int index = hash(record->id,tableSize);
	
	if (hashTable[index].record == NULL) //inserts record if hash table index is empty
	{
		hashTable[index].record = record;
		hashTable[index].record->next = NULL;
	}
	else //collision: inserts record into existing record's linked list
	{
		record->next = hashTable[index].record;
		hashTable[index].record = record;
	}
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
    // for each entry in the table
    // print the contents in that index
    // The output should look like this (for example): "Index 1 -> 21715, Q, 16 -> 28876, a, 26 -> NULL"
	struct RecordType* temp = NULL;
	for (int i = 0; i < tableSize; i++)
	{
		temp = hashTable[i].record;
		if (temp == NULL)
			continue;
		else
			printf("Index %d -> ", i);
		while (temp != NULL)
		{
			printf("%d, %c, %d -> ", temp->id, temp->name, temp->order);
			temp = temp->next;
		}
		printf("NULL\n");
	}
}


int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

	//Hash table initialization
	struct HashType* hashTable = (struct HashType*) calloc(recordSz, sizeof(struct HashType));
	int hashTableSz = 11;
	
	//hash table data input
	for (int i = 0; i < recordSz; i++)
	{
		insertRecord(hashTable, &(pRecords[i]), hashTableSz);
	}

	//hash table data display
	displayRecordsInHash(hashTable, hashTableSz);


    free(pRecords);
	free(hashTable);

    return 0;
}
