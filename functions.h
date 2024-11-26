#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>

// Existing constants
#define TABLE_SIZE 100
#define FILE_NAME "medicine_inventory.dat"
#define MAX_RESULTS 10

// New billing-related constants
#define MAX_BILL_ITEMS 50
#define MAX_BILLS 1000

// Stock and expiry alert thresholds
#define LOW_STOCK_THRESHOLD 50
#define NEAR_EXPIRY_DAYS 90

// Medicine Structure
typedef struct {
    char name[50];
    char batchnumber[50];
    int quantity;
    float price;
    char expiryDate[20];
    int isOccupied;
} Medicine;

// Existing Medicine Node Structure
typedef struct MedicineNode {
    Medicine data;
    struct MedicineNode *next;
} MedicineNode;

// Billing Item Structure
typedef struct {
    char medicineName[50];
    int quantity;
    float unitPrice;
    float totalPrice;
} BillItem;

// Bill Structure
typedef struct {
    int billId;
    char customerName[100];
    char billDate[20];
    BillItem items[MAX_BILL_ITEMS];
    int itemCount;
    float totalBillAmount;
    int isOccupied;
} Bill;

// Search Result Structures
typedef struct {
    Medicine medicines[MAX_RESULTS];
    bool found;
    int count;
    int error;
} MedicineSearchResult;

typedef struct {
    Medicine medicines[MAX_RESULTS];
    int count;
} MedicineList;

// Hash Table Function Prototypes
unsigned int djb2Hash(const char *medicineName);
bool initializeHashTable();
int insertMedicine(const Medicine *medicine);
MedicineSearchResult findMedicine(const char *name);
int deleteMedicine(const char *name);
int loadFromFile();
int saveToFile();
MedicineList getAllRecords();

// Input Validation Function Prototypes
void clearInputBuffer();
void clearScreen();
void printHeader();
void printMenuHeader();
int getValidInteger(const char *prompt);
float getValidFloat(const char *prompt);
int validateDate(const char *date);
void validateInput(char *input, int maxLength);
void pressAnyKeyToContinue();

// Medicine Input/Display Prototypes
Medicine inputMedicineDetails();
void displayMedicine(Medicine medicine);

// Billing Function Prototypes
Bill createBill(const char *customerName);
int addBillItem(Bill *bill, const char *medicineName, int quantity);
int saveBillToFile(const Bill *bill);
Bill* findBillById(int billId);
void displayBill(const Bill *bill);
void listAllBills();
int loadBillsFromFile();

// Function declarations for new inventory alert functionality
void getCurrentDate(char *currentDate);
int daysBetweenDates(const char *date1, const char *date2);
void checkInventoryAlerts();

#endif // FUNCTIONS_H