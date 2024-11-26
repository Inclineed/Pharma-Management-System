#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

// ANSI Color Codes
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"

int main() {
    // Initialize hash table and load existing data
    if (!initializeHashTable()) {
        printf(RED "Failed to initialize hash table.\n" RESET);
        return 1;
    }

    // Load existing data from file
    int loadedRecords = loadFromFile();
    
    int choice;
    while (1) {
        printHeader();
        printMenuHeader();
        
        // Enhanced menu with emojis
        printf(GREEN "1. 💊 Add New Medicine\n" RESET);
        printf(GREEN "2. 🔍 Find Medicine\n" RESET);
        printf(GREEN "3. ❌ Delete Medicine\n" RESET);
        printf(GREEN "4. 📋 View All Medicines\n" RESET);
        printf(GREEN "5. 💾 Save and Exit\n" RESET);
        printf(YELLOW "Loaded Records: %d\n" RESET, loadedRecords);
        
        choice = getValidInteger("Enter your choice: ");

        switch (choice) {
            case 1: { // Insert Medicine
                Medicine newMedicine = inputMedicineDetails();
                int result = insertMedicine(&newMedicine);
                
                printHeader();
                switch (result) {
                    case 0:
                        printf(GREEN "✅ Medicine inserted successfully!\n" RESET);
                        break;
                    case 1:
                        printf(YELLOW "⚠️ Medicine added to existing entry.\n" RESET);
                        break;
                    case -2:
                        printf(RED "❌ Memory allocation failed.\n" RESET);
                        break;
                    case -3:
                        printf(RED "❌ Hash table is full.\n" RESET);
                        break;
                }
                pressAnyKeyToContinue();
                break;
            }

            case 2: { // Find Medicine
                char searchName[100];
                printHeader();
                printf(MAGENTA "--- FIND MEDICINE ---\n" RESET);
                
                printf(BLUE "Enter Medicine Name to search: " RESET);
                if (fgets(searchName, sizeof(searchName), stdin) == NULL) {
                    printf(RED "Input error.\n" RESET);
                    pressAnyKeyToContinue();
                    break;
                }
                searchName[strcspn(searchName, "\n")] = 0;

                MedicineSearchResult result = findMedicine(searchName);
                
                printHeader();
                if (result.found) {
                    printf(GREEN "✅ Found %d matching medicine(s):\n" RESET, result.count);
                    for (int i = 0; i < result.count; i++) {
                        displayMedicine(result.medicines[i]);
                    }
                } else {
                    printf(RED "❌ Medicine not found.\n" RESET);
                }
                pressAnyKeyToContinue();
                break;
            }

            case 3: { // Delete Medicine
                char deleteName[100];
                printHeader();
                printf(MAGENTA "--- DELETE MEDICINE ---\n" RESET);
                
                printf(BLUE "Enter Medicine Name to delete: " RESET);
                if (fgets(deleteName, sizeof(deleteName), stdin) == NULL) {
                    printf(RED "Input error.\n" RESET);
                    pressAnyKeyToContinue();
                    break;
                }
                deleteName[strcspn(deleteName, "\n")] = 0;

                int result = deleteMedicine(deleteName);
                
                printHeader();
                switch (result) {
                    case 0:
                        printf(GREEN "✅ Medicine deleted successfully!\n" RESET);
                        break;
                    case -2:
                        printf(RED "❌ Medicine not found.\n" RESET);
                        break;
                }
                pressAnyKeyToContinue();
                break;
            }

            case 4: { // View All Medicines
                printHeader();
                printf(MAGENTA "--- ALL MEDICINES ---\n" RESET);
                
                MedicineList allMedicines = getAllRecords();
                
                if (allMedicines.count == 0) {
                    printf(YELLOW "📭 No medicines in the inventory.\n" RESET);
                } else {
                    printf(GREEN "📦 Total Medicines: %d\n" RESET, allMedicines.count);
                    for (int i = 0; i < allMedicines.count; i++) {
                        displayMedicine(allMedicines.medicines[i]);
                    }
                }
                pressAnyKeyToContinue();
                break;
            }

            case 5: { // Save and Exit
                int savedRecords = saveToFile();
                printHeader();
                printf(GREEN "✅ Saved %d records to file.\n" RESET, savedRecords);
                printf(YELLOW "👋 Exiting the program. Goodbye!\n" RESET);
                return 0;
            }

            default:
                printf(RED "❌ Invalid choice. Please try again.\n" RESET);
                pressAnyKeyToContinue();
        }
    }

    return 0;
}