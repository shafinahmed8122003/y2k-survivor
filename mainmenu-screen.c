#include <stdio.h>
#include "constants.h"
#include "database.h"

int MainmenuScreen(User user)
{
    char options[][50] =
    {
        "News Feed",
        "Create Post",
        "Profile",
        "Search",
        "Notification",
        "Settings",
    };

    int total_options = 6;
    int current_option = 0;

    while (1)
    {
        printf(CLEAR_SCREEN);

        printf("Peach Be Upon You, %s!\n", user.name);
        printf("------------------------\n");

        for (int i = 0; i < total_options; i++)
        {
            if (current_option == i)
            {
                printf("\033[94m> %s\033[90m\n", options[i]);
            }
            else
            {
                printf("  %s\n", options[i]);
            }
        }

        int keys = getch();
        if (keys == 224)
        {
            int direction = getch();
            if (direction == 72) // Up arrow
            {
                current_option = current_option > 0 ? current_option - 1 : total_options - 1;
            }
            else if (direction == 80) // Down arrow
            {
                current_option = current_option < total_options - 1 ? current_option + 1 : 0;
            }
        }
        else if (keys == 13) // Enter key
        {
            return current_option;
        }
    }
}
