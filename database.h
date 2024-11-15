#ifndef DATABASE_H
#define DATABASE_H

typedef struct {
    char name[50];
    char username[50];
    char password[50];
} User;

typedef struct {
    User *users;
    size_t user_count;
    size_t capacity;
} Database;

char *read_file(const char *filename);
Database load_database(const char *filename);
void save_database(const char *filename, Database *db);
void free_database(Database *db);
void add_user(Database *db, const char *name, const char *username, const char *password);

#endif



