#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "screens.h"
#include "constants.h"
#include "database.h"

int main()
{
    Database db = load_database("database.json");
    User user = {"", "", ""};
    printf(TERMINAL_SETUP);
    printf(CLEAR_SCREEN);

auth_screen:
    {
        int choice = AuthScreen();
        if (choice == 0)
        {
            goto login_screen;
        }
        else
        {
            goto end;
        }
    }

login_screen:
    {
        LoginScreen(db, &user);
        if (strcmp(user.name, "") != 0) {
            printf("Welcome!\n");
        }
        else
        {
            printf("\nWrong information!\n");
            printf("Press enter!\n");
            getch();
            goto auth_screen;
        }
    }

    // signup_screen:
    // SignupScreen();

mainmenu_screen:
    {
        MainmenuScreen(user);
    }

end:
    printf(RESET);
    return 0;
}
