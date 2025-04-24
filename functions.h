#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RECORDS 100
#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 100
#define MIN_AMOUNT 1
#define MAX_AMOUNT 95

typedef struct {
    char name[MAX_NAME_LENGTH];
    int amountUS;
    int amountAU;
    int amountEuro;
} UserRecord;


int loadUserData(const char *filename, UserRecord users[]);
FILE* openFile(const char* filename, const char* mode);
void processFileLines(FILE* file, UserRecord users[], int* numUsers);
void processLine(const char* line, int lineNum, UserRecord users[], int* numUsers);
int readTotalRecords(FILE *file);
int parseLine(const char *line, char *name, int *amount, char *currency);
int validateAmount(int amount, int lineNum);
int validateCurrency(const char *currency);
void printFormatError(int lineNum);
void printCurrencyError(const char* currency, int lineNum);
void updateUserRecord(UserRecord users[], int* numUsers, const char* name, int amount, const char* currency);
int findUser(UserRecord users[], int numUsers, const char *name);
void updateUserAmount(UserRecord *user, int amount, const char *currency);
void addNewUser(UserRecord users[], int *numUsers, const char *name, int amount, const char *currency);
int findExactUser(UserRecord users[], int numUsers, const char* name);
void processUserMenu(UserRecord users[], int numUsers);
void displayMenu(void);
int getUserChoice(void);
void clearInputBuffer(void);
void processMenuChoice(int choice, UserRecord users[], int numUsers);
void processNameEntry(UserRecord users[], int numUsers);
int getUserInput(char* input);
void findAndDisplayUser(const char* name, UserRecord users[], int numUsers);
void displayUserDetails(const UserRecord *user);
void displayUserAmountDetails(const UserRecord *user, const char* currency, int amount);
void displayChangeForAmount(int amount, const char* currency);
void displayChange(int amount, const char *currency, int change[]);
void calculateChange(int amount, const char *currency, int change[]);
void saveChangeToCSV(const char *filename, UserRecord users[], int numUsers);
void writeAllUsersToFile(FILE *file, UserRecord users[], int numUsers);
void writeUserDataToFile(FILE *file, const UserRecord *user);
void writeUserCurrencyData(FILE *file, const UserRecord *user, const char* currency, int amount);
void writeChangeToFile(FILE *file, const char *name, int amount, const char *currency, int change[]);

#endif