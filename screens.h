#ifndef SCREENS_H
#define SCREENS_H

#include "database.h"

int AuthScreen();
void LoginScreen(Database db, User *user);
int MainmenuScreen(User user);

#endif

