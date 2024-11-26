#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "functions.h"

// ANSI Color Codes
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"

// Global hash table
MedicineNode *hashTable[TABLE_SIZE];

// Hash function (djb2)
unsigned int djb2Hash(const char *medicineName) {
    unsigned int hash = 5381;
    int c;
    while ((c = *medicineName++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

// Initialize the hash table
bool initializeHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
    return true;
}

// Insert a medicine into the hash table
int insertMedicine(const Medicine *medicine) {
    if (!medicine) return -1; // Invalid input
    
    unsigned int index = djb2Hash(medicine->name);
    unsigned int originalIndex = index;

    // Linear probing for collision resolution
    while (hashTable[index] != NULL) {
        if (strcmp(hashTable[index]->data.name, medicine->name) == 0) {
            // If medicine with same name exists, add to linked list
            MedicineNode *current = hashTable[index];
            while (current->next != NULL) {
                current = current->next;
            }
            MedicineNode *newNode = (MedicineNode *)malloc(sizeof(MedicineNode));
            if (!newNode) return -2; // Memory allocation failure
            
            newNode->data = *medicine;
            newNode->next = NULL;
            current->next = newNode;
            return 1; // Successfully added to linked list
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == originalIndex) {
            return -3; // Hash table is full
        }
    }

    // Insert new medicine
    MedicineNode *newNode = (MedicineNode *)malloc(sizeof(MedicineNode));
    if (!newNode) return -2; // Memory allocation failure
    
    newNode->data = *medicine;
    newNode->next = NULL;
    hashTable[index] = newNode;
    return 0; // Successfully inserted new entry
}

// Find a medicine in the hash table
MedicineSearchResult findMedicine(const char *name) {
    MedicineSearchResult result = {0};
    result.found = false;
    result.count = 0;
    
    if (!name) {
        result.error = -1; // Invalid input
        return result;
    }

    unsigned int index = djb2Hash(name);
    unsigned int originalIndex = index;

    // Linear probing to locate the index
    while (hashTable[index] != NULL && strcmp(hashTable[index]->data.name, name) != 0) {
        index = (index + 1) % TABLE_SIZE;
        if (index == originalIndex) {
            result.error = -2; // Not found after full table scan
            return result;
        }
    }

    if (hashTable[index] == NULL) {
        result.error = -2; // Not found
        return result;
    }

    // Found medicines - collect all matches in linked list
    result.found = true;
    MedicineNode *current = hashTable[index];
    while (current != NULL && result.count < MAX_RESULTS) {
        result.medicines[result.count++] = current->data;
        current = current->next;
    }

    return result;
}

// Delete a medicine from the hash table
int deleteMedicine(const char *name) {
    if (!name) return -1; // Invalid input
    
    unsigned int index = djb2Hash(name);
    unsigned int originalIndex = index;

    // Linear probing to locate the index
    while (hashTable[index] != NULL && strcmp(hashTable[index]->data.name, name) != 0) {
        index = (index + 1) % TABLE_SIZE;
        if (index == originalIndex) {
            return -2; // Not found after full table scan
        }
    }

    if (hashTable[index] == NULL) {
        return -2; // Not found
    }

    // Handle deletion
    MedicineNode *current = hashTable[index];
    MedicineNode *prev = NULL;
    bool deleted = false;

    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            deleted = true;
            break;
        }
        prev = current;
        current = current->next;
    }

    return deleted ? 0 : -2; // 0 for success, -2 for not found
}

// Load existing data from the file into the hash table
int loadFromFile() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        return -1; // File not found or error opening
    }

    int recordsLoaded = 0;
    Medicine medicine;
    
    while (fread(&medicine, sizeof(Medicine), 1, file)) {
        if (medicine.isOccupied) {
            int result = insertMedicine(&medicine);
            if (result >= 0) { // Successfully inserted
                recordsLoaded++;
            }
        }
    }

    fclose(file);
    return recordsLoaded; // Returns number of records loaded
}

// Save the current hash table data to a file
int saveToFile() {
    FILE *file = fopen(FILE_NAME, "wb");
    if (file == NULL) {
        return -1; // Error opening file
    }

    int recordsSaved = 0;
    MedicineNode *current;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        current = hashTable[i];
        while (current != NULL) {
            if (fwrite(&current->data, sizeof(Medicine), 1, file) == 1) {
                recordsSaved++;
            }
            current = current->next;
        }
    }

    fclose(file);
    return recordsSaved; // Returns number of records saved
}

// Get all records in the hash table
MedicineList getAllRecords() {
    MedicineList list = {0};
    list.count = 0;
    
    for (int i = 0; i < TABLE_SIZE && list.count < MAX_RESULTS; i++) {
        MedicineNode *current = hashTable[i];
        while (current != NULL && list.count < MAX_RESULTS) {
            list.medicines[list.count++] = current->data;
            current = current->next;
        }
    }
    
    return list;
}

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Clear screen (works on most systems)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Print colorful header
void printHeader() {
    clearScreen();
    printf(CYAN "========================================================\n" RESET);
    printf(GREEN "   🏥 Medicine Inventory Management System 💊\n" RESET);
    printf(CYAN "========================================================\n" RESET);
}

// Print menu header
void printMenuHeader() {
    printf(YELLOW "\n--- MAIN MENU ---\n" RESET);
}

// Validate and get integer input
int getValidInteger(const char *prompt) {
    int value;
    char buffer[100];
    
    while (1) {
        printf(BLUE "%s" RESET, prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf(RED "Invalid input. Please try again.\n" RESET);
            continue;
        }
        
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Validate input
        char *endptr;
        long parsed = strtol(buffer, &endptr, 10);
        
        if (*endptr != '\0') {
            printf(RED "Please enter a valid integer.\n" RESET);
            continue;
        }
        
        if (parsed < 0) {
            printf(RED "Please enter a non-negative number.\n" RESET);
            continue;
        }
        
        value = (int)parsed;
        break;
    }
    
    return value;
}

// Validate and get float input
float getValidFloat(const char *prompt) {
    float value;
    char buffer[100];
    
    while (1) {
        printf(BLUE "%s" RESET, prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf(RED "Invalid input. Please try again.\n" RESET);
            continue;
        }
        
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Validate input
        char *endptr;
        float parsed = strtof(buffer, &endptr);
        
        if (*endptr != '\0') {
            printf(RED "Please enter a valid number.\n" RESET);
            continue;
        }
        
        if (parsed < 0) {
            printf(RED "Please enter a non-negative number.\n" RESET);
            continue;
        }
        
        value = parsed;
        break;
    }
    
    return value;
}

// Validate date format
int validateDate(const char *date) {
    int day, month, year;
    
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0;
    }
    
    // Basic date validation
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Leap year check
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    if (day < 1 || day > daysInMonth[month]) return 0;
    
    return 1;
}

// Validate and get string input
void validateInput(char *input, int maxLength) {
    // Remove leading/trailing whitespace
    char *start = input, *end = input + strlen(input) - 1;
    while (isspace(*start)) start++;
    while (end > start && isspace(*end)) end--;
    *(end + 1) = '\0';
    
    // Check if input is empty
    if (strlen(start) == 0) {
        printf(RED "Input cannot be empty.\n" RESET);
        input[0] = '\0';
        return;
    }
    
    // Truncate if too long
    if (strlen(start) > maxLength - 1) {
        start[maxLength - 1] = '\0';
        printf(YELLOW "Input truncated to %d characters.\n" RESET, maxLength - 1);
    }
    
    // Copy back to original input
    strcpy(input, start);
}

// Pause and wait for user to press a key
void pressAnyKeyToContinue() {
    printf(YELLOW "\nPress Enter to continue..." RESET);
    clearInputBuffer();
}

// Input medicine details
Medicine inputMedicineDetails() {
    Medicine medicine;
    
    printHeader();
    printf(MAGENTA "--- ADD NEW MEDICINE ---\n" RESET);
    
    // Medicine Name
    while (1) {
        printf(BLUE "Enter Medicine Name: " RESET);
        if (fgets(medicine.name, sizeof(medicine.name), stdin) == NULL) {
            printf(RED "Input error. Please try again.\n" RESET);
            continue;
        }
        medicine.name[strcspn(medicine.name, "\n")] = 0;
        validateInput(medicine.name, sizeof(medicine.name));
        if (strlen(medicine.name) > 0) break;
    }
    
    // Batch Number
    while (1) {
        printf(BLUE "Enter Batch Number: " RESET);
        if (fgets(medicine.batchnumber, sizeof(medicine.batchnumber), stdin) == NULL) {
            printf(RED "Input error. Please try again.\n" RESET);
            continue;
        }
        medicine.batchnumber[strcspn(medicine.batchnumber, "\n")] = 0;
        validateInput(medicine.batchnumber, sizeof(medicine.batchnumber));
        if (strlen(medicine.batchnumber) > 0) break;
    }
    
    // Quantity
    medicine.quantity = getValidInteger("Enter Quantity: ");
    
    // Price
    medicine.price = getValidFloat("Enter Price: ");
    
    // Expiry Date
    while (1) {
        printf(BLUE "Enter Expiry Date (DD/MM/YYYY): " RESET);
        if (fgets(medicine.expiryDate, sizeof(medicine.expiryDate), stdin) == NULL) {
            printf(RED "Input error. Please try again.\n" RESET);
            continue;
        }
        medicine.expiryDate[strcspn(medicine.expiryDate, "\n")] = 0;
        
        if (validateDate(medicine.expiryDate)) {
            break;
        }
        
        printf(RED "Invalid date format. Use DD/MM/YYYY.\n" RESET);
    }
    
    medicine.isOccupied = 1; // Mark as occupied
    return medicine;
}

// Display medicine details
void displayMedicine(Medicine medicine) {
    printf(CYAN "----------------------------------------\n" RESET);
    printf(GREEN "Medicine Name    : " RESET "%s\n", medicine.name);
    printf(GREEN "Batch Number    : " RESET "%s\n", medicine.batchnumber);
    printf(GREEN "Quantity        : " RESET "%d\n", medicine.quantity);
    printf(GREEN "Price           : " RESET "₹%.2f\n", medicine.price);
    printf(GREEN "Expiry Date     : " RESET "%s\n", medicine.expiryDate);
    printf(CYAN "----------------------------------------\n" RESET);
}