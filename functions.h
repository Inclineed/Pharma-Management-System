#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define TABLE_SIZE 100
#define FILE_NAME "medicine_inventory.dat"
#define MAX_RESULTS 50

// Medicine structure definition
typedef struct Medicine {
    char name[100];
    char batchnumber[50];
    int quantity;
    float price;
    char expiryDate[20];
    int isOccupied;
} Medicine;

// Medicine Node for linked list in hash table
typedef struct MedicineNode {
    Medicine data;
    struct MedicineNode *next;
} MedicineNode;

// Search Result structure
typedef struct {
    Medicine medicines[MAX_RESULTS];
    int count;
    bool found;
    int error;
} MedicineSearchResult;

// Medicine List structure
typedef struct {
    Medicine medicines[MAX_RESULTS];
    int count;
} MedicineList;

// Function Prototypes for Utility Functions
void clearScreen();
void printHeader();
void printMenuHeader();
int getValidInteger(const char *prompt);
float getValidFloat(const char *prompt);
void pressAnyKeyToContinue();
void validateInput(char *input, int maxLength);
int validateDate(const char *date);
void clearInputBuffer();
Medicine inputMedicineDetails();
void displayMedicine(Medicine medicine);

// Function Prototypes for Hash Table Operations
unsigned int djb2Hash(const char *medicineName);
bool initializeHashTable();
int insertMedicine(const Medicine *medicine);
MedicineSearchResult findMedicine(const char *name);
int deleteMedicine(const char *name);
int loadFromFile();
int saveToFile();
MedicineList getAllRecords();

#endif // FUNCTIONS_H