#include <stdio.h>
#include "database.h"
#include "constants.h"
#include <string.h>

void SignupScreen(Database db, User *user)
{
    char name[50];
    char username[50];
    char password[50];

    printf(CLEAR_SCREEN);
    printf("Start your Y2K Survivor journey!\n");
    printf("--------------------------------\n");
    printf("Name: ");
    scanf(" %s", name);
    printf("User Name: ");
    scanf(" %s", username);
    printf("Password: ");
    scanf(" %s", password);

    int username_unique = 1;
    for (int i = 0; i < db.user_count; i++)
    {
        if (strcmp(username, db.users[i].username) == 0)
        {
            username_unique = 0;
        }
    }

    if (username_unique)
    {
        add_user(&db, name, username, password);
        strcpy((*user).name, name);
        strcpy((*user).username, username);
        strcpy((*user).password, password);
    }

    // Save the updated database back to the file
    save_database("database.json", &db);
}
