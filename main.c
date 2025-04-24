// main.c
#include "functions.h"

int main() {
    UserRecord users[MAX_RECORDS];
    int numUsers = 0;
    
    // Load user data from coins.txt
    numUsers = loadUserData("coins.txt", users);
    
    // Show menu and process user choices
    processUserMenu(users, numUsers);
    
    return 0;
}