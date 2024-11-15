#include <stdio.h>
#include "constants.h"
#include "database.h"
#include <string.h>

void LoginScreen(Database db, User *user)
{
    char username[50];
    char password[50];

    printf(CLEAR_SCREEN);
    printf("Welcome Back\n");
    printf("Enter your credentials please!\n");
    printf("User Name: ");
    scanf(" %s", username);
    printf("Password: ");
    scanf(" %s", password);

    int passed = 0;
    for (int i = 0; i < db.user_count; i++)
    {
        if (strcmp(username, db.users[i].username) == 0 && strcmp(password, db.users[i].password) == 0)
        {
            strcpy((*user).name, db.users[i].name);
            strcpy((*user).username, db.users[i].username);
            strcpy((*user).password, db.users[i].password);
        }
    }
}
