#include "functions.h"

// File operations
int loadUserData(const char *filename, UserRecord users[]) {
    FILE *file = openFile(filename, "r");
    int numUsers = 0;
    int totalRecords = readTotalRecords(file);
    processFileLines(file, users, &numUsers);
    fclose(file);
    return numUsers;
} 

FILE* openFile(const char* filename, const char* mode) {
    FILE *file = fopen(filename, mode);
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }
    return file;
}

void processFileLines(FILE* file, UserRecord users[], int* numUsers) {
    char line[MAX_LINE_LENGTH];
    int lineNum = 1;
    
    // Skip the first line with total records
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        processLine(line, lineNum, users, numUsers);
    }
}

void processLine(const char* line, int lineNum, UserRecord users[], int* numUsers) {
    char name[MAX_NAME_LENGTH];
    int amount;
    char currency[10];
    
    if (!parseLine(line, name, &amount, currency)) {
        printFormatError(lineNum);
        return;
    }
    
    if (!validateAmount(amount, lineNum)) {
        return;
    }
    
    if (!validateCurrency(currency)) {
        printCurrencyError(currency, lineNum);
        return;
    }
    
    updateUserRecord(users, numUsers, name, amount, currency);
}

int readTotalRecords(FILE *file) {
    int total;
    fscanf(file, "%d\n", &total);
    return total;
}

// Parsing and validation
int parseLine(const char *line, char *name, int *amount, char *currency) {
    char tempLine[MAX_LINE_LENGTH];
    char extraWord[10];  // for "cents"
    strcpy(tempLine, line);

    // Format expected: "<name> <amount> cents in <currency>"
    int result = sscanf(tempLine, "%[^0-9] %d %s in %s", name, amount, extraWord, currency);
    
    // Remove trailing spaces from name
    int len = strlen(name);
    while (len > 0 && isspace(name[len-1])) {
        name[len-1] = '\0';
        len--;
    }
    
    // Remove trailing newline from currency if present
    len = strlen(currency);
    if (len > 0 && currency[len-1] == '\n') {
        currency[len-1] = '\0';
    }
    
    return (result == 4);  // I expect 4 parts: name, amount, "cents", and currency
}

int validateAmount(int amount, int lineNum) {
    if (amount < 1 || amount > 99) {
        printf("Error: Invalid amount (%d cents) in line %d - amount must be between 1 and 99 cents\n", 
               amount, lineNum);
        return 0;
    }
    return 1;
}

int validateCurrency(const char *currency) {
    char cleanCurrency[10];
    strcpy(cleanCurrency, currency);
    
    int len = strlen(cleanCurrency);
    while (len > 0 && (cleanCurrency[len-1] == '\n' || cleanCurrency[len-1] == ' ')) {
        cleanCurrency[len-1] = '\0';
        len--;
    }
    
    return (strcmp(cleanCurrency, "US$") == 0 || 
            strcmp(cleanCurrency, "AU$") == 0 || 
            strcmp(cleanCurrency, "Euro") == 0);
}

// Error messages
void printFormatError(int lineNum) {
    printf("Error: Invalid format in line %d - expected format: '<name> <amount> cents in <currency>'\n", 
           lineNum);
}

void printCurrencyError(const char* currency, int lineNum) {
    printf("Error: Invalid currency '%s' in line %d - must be US$, AU$, or Euro\n", 
           currency, lineNum);
}

// User record operations
void updateUserRecord(UserRecord users[], int* numUsers, const char* name, int amount, const char* currency) {
    int userIndex = findUser(users, *numUsers, name);
    if (userIndex >= 0) {
        updateUserAmount(&users[userIndex], amount, currency);
    } else {
        addNewUser(users, numUsers, name, amount, currency);
    }
}

int findUser(UserRecord users[], int numUsers, const char *name) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void updateUserAmount(UserRecord *user, int amount, const char *currency) {
    if (strcmp(currency, "US$") == 0) {
        user->amountUS += amount;
    } else if (strcmp(currency, "AU$") == 0) {
        user->amountAU += amount;
    } else if (strcmp(currency, "Euro") == 0) {
        user->amountEuro += amount;
    }
}

void addNewUser(UserRecord users[], int *numUsers, const char *name, int amount, const char *currency) {
    strcpy(users[*numUsers].name, name);
    users[*numUsers].amountUS = 0;
    users[*numUsers].amountAU = 0;
    users[*numUsers].amountEuro = 0;
    updateUserAmount(&users[*numUsers], amount, currency);
    (*numUsers)++;
}

int findExactUser(UserRecord users[], int numUsers, const char* name) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Menu handling
void processUserMenu(UserRecord users[], int numUsers) {
    int choice;
    do {
        displayMenu();
        choice = getUserChoice();
        processMenuChoice(choice, users, numUsers);
    } while (choice != 2);
}

void displayMenu() {
    printf("\n1. Enter name\n2. Exit\n");
    printf("Choose an option: ");
}

int getUserChoice() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter 1 or 2.\n");
        return 0;
    }
    clearInputBuffer();
    return choice;
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void processMenuChoice(int choice, UserRecord users[], int numUsers) {
    switch (choice) {
        case 1:
            processNameEntry(users, numUsers);
            break;
        case 2:
            saveChangeToCSV("change.csv", users, numUsers);
            printf("Data stored in change.csv file \n");
            break;
        default:
            printf("Invalid option. Please enter 1 or 2.\n");
    }
}

void processNameEntry(UserRecord users[], int numUsers) {
    char input[MAX_NAME_LENGTH];
    printf("Enter name: ");
    if (getUserInput(input)) {
        findAndDisplayUser(input, users, numUsers);
    }
}

int getUserInput(char* input) {
    if (fgets(input, MAX_NAME_LENGTH, stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        return 1;
    }
    return 0;
}

void findAndDisplayUser(const char* name, UserRecord users[], int numUsers) {
    int userIndex = findExactUser(users, numUsers, name);
    if (userIndex >= 0) {
        displayUserDetails(&users[userIndex]);
    } else {
        printf("Error: No exact match found for '%s'\n", name);
    }
}

// Display operations
void displayUserDetails(const UserRecord *user) {
    displayUserAmountDetails(user, "US$", user->amountUS);
    displayUserAmountDetails(user, "AU$", user->amountAU);
    displayUserAmountDetails(user, "Euro", user->amountEuro);
}

void displayUserAmountDetails(const UserRecord *user, const char* currency, int amount) {
    if (amount > 0) {
        printf("\nCustomer:\n");
        printf("%s %d cents in %s\n", user->name, amount, currency);
        printf("Change:\n");
        displayChangeForAmount(amount, currency);
        printf("\n");
    }
}

void displayChangeForAmount(int amount, const char* currency) {
    int change[4] = {0};
    calculateChange(amount, currency, change);
    displayChange(amount, currency, change);
}

void displayChange(int amount, const char *currency, int change[]) {
    int denominations[4];
    
    if (strcmp(currency, "US$") == 0) {
        denominations[0] = 50;
        denominations[1] = 25;
        denominations[2] = 10;
        denominations[3] = 1;
    } else if (strcmp(currency, "AU$") == 0) {
        denominations[0] = 50;
        denominations[1] = 20;
        denominations[2] = 10;
        denominations[3] = 5;
    } else { // Euro
        denominations[0] = 20;
        denominations[1] = 10;
        denominations[2] = 5;
        denominations[3] = 1;
    }
    
    for (int i = 0; i < 4; i++) {
        if (change[i] > 0) {
            printf("%d cents: %d\n", denominations[i], change[i]);
        }
    }
}

// Change calculation
void calculateChange(int amount, const char *currency, int change[]) {
    int denominations[4];
    
    if (strcmp(currency, "US$") == 0) {
        denominations[0] = 50;
        denominations[1] = 25;
        denominations[2] = 10;
        denominations[3] = 1;
    } else if (strcmp(currency, "AU$") == 0) {
        denominations[0] = 50;
        denominations[1] = 20;
        denominations[2] = 10;
        denominations[3] = 5;
    } else { // Euro
        denominations[0] = 20;
        denominations[1] = 10;
        denominations[2] = 5;
        denominations[3] = 1;
    }
    
    for (int i = 0; i < 4; i++) {
        change[i] = amount / denominations[i];
        amount %= denominations[i];
    }
}

// File saving operations
void saveChangeToCSV(const char *filename, UserRecord users[], int numUsers) {
    FILE *file = openFile(filename, "w");
    writeAllUsersToFile(file, users, numUsers);
    fclose(file);
}

void writeAllUsersToFile(FILE *file, UserRecord users[], int numUsers) {
    for (int i = 0; i < numUsers; i++) {
        writeUserDataToFile(file, &users[i]);
    }
}

void writeUserDataToFile(FILE *file, const UserRecord *user) {
    writeUserCurrencyData(file, user, "US$", user->amountUS);
    writeUserCurrencyData(file, user, "AU$", user->amountAU);
    writeUserCurrencyData(file, user, "Euro", user->amountEuro);
}


void writeUserCurrencyData(FILE *file, const UserRecord *user, const char* currency, int amount) {
    if (amount > 0) {
        int change[4];
        calculateChange(amount, currency, change);
        writeChangeToFile(file, user->name, amount, currency, change);
    }
}

void writeChangeToFile(FILE *file, const char *name, int amount, const char *currency, int change[]) {
    fprintf(file, "%s, the change for %d cents in %s is %d,%d,%d,%d\n",
            name, amount, currency, change[0], change[1], change[2], change[3]);
}