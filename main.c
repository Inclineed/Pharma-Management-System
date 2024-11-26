#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main() {
    // Initialize hash table and load existing data
    if (!initializeHashTable()) {
        printf(RED "Failed to initialize hash table.\n" RESET);
        return 1;
    }

    // Load existing data from file
    int loadedRecords = loadFromFile();

    // Sample Data
    Medicine medicines[] = {
        {
            .name = "Paracetamol",
            .batchnumber = "PAR2024001",
            .quantity = 500,
            .price = 25.50,
            .expiryDate = "31/12/2024",
            .isOccupied = 1
        },
        {
            .name = "Amoxicillin",
            .batchnumber = "AMX2024002",
            .quantity = 250,
            .price = 175.75,
            .expiryDate = "30/06/2025",
            .isOccupied = 1
        },
        {
            .name = "Insulin Glargine",
            .batchnumber = "INS2024003",
            .quantity = 100,
            .price = 850.00,
            .expiryDate = "28/02/2025",
            .isOccupied = 1
        },
        {
            .name = "Ibuprofen",
            .batchnumber = "IBU2024004",
            .quantity = 750,
            .price = 45.25,
            .expiryDate = "15/11/2025",
            .isOccupied = 1
        },
        {
            .name = "Metformin",
            .batchnumber = "MET2024005",
            .quantity = 600,
            .price = 95.50,
            .expiryDate = "20/09/2025",
            .isOccupied = 1
        },
        {
            .name = "Ciprofloxacin",
            .batchnumber = "CIP2024006",
            .quantity = 300,
            .price = 125.75,
            .expiryDate = "10/07/2025",
            .isOccupied = 1
        },
        {
            .name = "Losartan",
            .batchnumber = "LOS2024007",
            .quantity = 450,
            .price = 55.25,
            .expiryDate = "25/12/2025",
            .isOccupied = 1
        },
        {
            .name = "Azithromycin",
            .batchnumber = "AZI2024008",
            .quantity = 200,
            .price = 210.50,
            .expiryDate = "15/08/2025",
            .isOccupied = 1
        },
        {
            .name = "Pantoprazole",
            .batchnumber = "PAN2024009",
            .quantity = 350,
            .price = 85.75,
            .expiryDate = "30/10/2025",
            .isOccupied = 1
        },
        {
            .name = "Levothyroxine",
            .batchnumber = "LEV2024010",
            .quantity = 275,
            .price = 125.00,
            .expiryDate = "20/06/2025",
            .isOccupied = 1
        }
    };

    // To add these to the hash table, you could use a loop:
    for (int i = 0; i < sizeof(medicines) / sizeof(medicines[0]); i++) {
        insertMedicine(&medicines[i]);
    }
    
    // Add this near the top of main(), after loading medicine records
    int loadedBills = loadBillsFromFile();
    printf(YELLOW "Loaded Bills: %d\n" RESET, loadedBills);
    int choice;
    while (1) {
        printHeader();
        printMenuHeader();
        
        // Enhanced menu with emojis
        printf(GREEN "1. Add New Medicine\n" RESET);
        printf(GREEN "2. Find Medicine\n" RESET);
        printf(GREEN "3. Delete Medicine\n" RESET);
        printf(GREEN "4. View All Medicines\n" RESET);
        printf(GREEN "5. Billing Menu\n" RESET);
        printf(GREEN "6. Check Inventory Alerts" RESET "\n");
        printf(GREEN "7. Save and Exit\n" RESET);
        printf(YELLOW "Loaded Records: %d\n" RESET, loadedRecords);
        
        choice = getValidInteger("Enter your choice: ");

        switch (choice) {
            case 1: { // Insert Medicine
                Medicine newMedicine = inputMedicineDetails();
                int result = insertMedicine(&newMedicine);
                
                printHeader();
                switch (result) {
                    case 0:
                        printf(GREEN "Medicine inserted successfully!\n" RESET);
                        break;
                    case 1:
                        printf(YELLOW "Medicine added to existing entry.\n" RESET);
                        break;
                    case -2:
                        printf(RED "Memory allocation failed.\n" RESET);
                        break;
                    case -3:
                        printf(RED "Hash table is full.\n" RESET);
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
                    printf(GREEN "Found %d matching medicine(s):\n" RESET, result.count);
                    for (int i = 0; i < result.count; i++) {
                        displayMedicine(result.medicines[i]);
                    }
                } else {
                    printf(RED "Medicine not found.\n" RESET);
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
                        printf(GREEN "Medicine deleted successfully!\n" RESET);
                        break;
                    case -2:
                        printf(RED "Medicine not found.\n" RESET);
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
                    printf(YELLOW "No medicines in the inventory.\n" RESET);
                } else {
                    printf(GREEN "Total Medicines: %d\n" RESET, allMedicines.count);
                    for (int i = 0; i < allMedicines.count; i++) {
                        displayMedicine(allMedicines.medicines[i]);
                    }
                }
                pressAnyKeyToContinue();
                break;
            }

            // In your main menu switch statement
            case 5: // Example billing menu
                printf(YELLOW "\n--- BILLING MENU ---\n" RESET);
                printf("1. Create New Bill\n");
                printf("2. View Specific Bill\n");
                printf("3. List All Bills\n");
                printf("4. Return to Main Menu\n");

                int billingChoice = getValidInteger("Enter your choice: ");
                switch (billingChoice) {
                    case 1: {
                        char customerName[100];
                        printf(BLUE "Enter Customer Name: " RESET);
                        fgets(customerName, sizeof(customerName), stdin);
                        customerName[strcspn(customerName, "\n")] = 0;
                        
                        Bill newBill = createBill(customerName);
                        if (newBill.isOccupied) {
                            // Add items to bill
                            while (1) {
                                char medicineName[50];
                                printf(BLUE "Enter Medicine Name (or 'done' to finish): " RESET);
                                fgets(medicineName, sizeof(medicineName), stdin);
                                medicineName[strcspn(medicineName, "\n")] = 0;
                                
                                if (strcmp(medicineName, "done") == 0) break;
                                
                                int quantity = getValidInteger("Enter Quantity: ");
                                addBillItem(&newBill, medicineName, quantity);
                            }
                            
                            // Display and save bill
                            displayBill(&newBill);
                            saveBillToFile(&newBill);
                        }
                        break;
                    }
                    case 2: {
                        int billId = getValidInteger("Enter Bill ID to view: ");
                        Bill *bill = findBillById(billId);
                        if (bill) {
                            displayBill(bill);
                        } else {
                            printf(RED "Bill not found.\n" RESET);
                        }
                        break;
                    }
                    case 3:
                        listAllBills();
                        break;
                }
                pressAnyKeyToContinue();
                break;

            case 6:
                checkInventoryAlerts();
                pressAnyKeyToContinue();
                break;

            case 7: { // Save and Exit
                int savedRecords = saveToFile();
                printHeader();
                printf(GREEN "Saved %d records to file.\n" RESET, savedRecords);
                printf(YELLOW "Exiting the program. Goodbye!\n" RESET);
                return 0;
            }
            

            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pressAnyKeyToContinue();
        }
    }

    

    return 0;
}