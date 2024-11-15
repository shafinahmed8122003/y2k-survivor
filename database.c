#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define INITIAL_CAPACITY 10

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

// Function to read a file into a string
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (!data) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);
    return data;
}

// Initialize the database with a certain capacity
Database init_database(size_t initial_capacity) {
    Database db;
    db.users = (User *)malloc(initial_capacity * sizeof(User));
    db.user_count = 0;
    db.capacity = initial_capacity;
    return db;
}

// Load the database from a file
Database load_database(const char *filename) {
    Database db = init_database(INITIAL_CAPACITY);

    char *json_string = read_file(filename);
    if (!json_string) return db;

    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    if (!root) return db;

    cJSON *users = cJSON_GetObjectItem(root, "users");
    if (cJSON_IsArray(users)) {
        db.user_count = cJSON_GetArraySize(users);
        db.capacity = db.user_count > INITIAL_CAPACITY ? db.user_count : INITIAL_CAPACITY;
        db.users = (User *)realloc(db.users, db.capacity * sizeof(User));

        cJSON *user;
        size_t i = 0;
        cJSON_ArrayForEach(user, users) {
            cJSON *name = cJSON_GetObjectItem(user, "name");
            cJSON *username = cJSON_GetObjectItem(user, "username");
            cJSON *password = cJSON_GetObjectItem(user, "password");

            if (cJSON_IsString(name) && cJSON_IsString(username) && cJSON_IsString(password)) {
                strncpy(db.users[i].name, name->valuestring, sizeof(db.users[i].name));
                strncpy(db.users[i].username, username->valuestring, sizeof(db.users[i].username));
                strncpy(db.users[i].password, password->valuestring, sizeof(db.users[i].password));
                i++;
            }
        }
    }

    cJSON_Delete(root);
    return db;
}

// Save the database to a file
void save_database(const char *filename, Database *db) {
    cJSON *root = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();

    for (size_t i = 0; i < db->user_count; i++) {
        cJSON *user = cJSON_CreateObject();
        cJSON_AddStringToObject(user, "name", db->users[i].name);
        cJSON_AddStringToObject(user, "username", db->users[i].username);
        cJSON_AddStringToObject(user, "password", db->users[i].password);
        cJSON_AddItemToArray(users, user);
    }

    cJSON_AddItemToObject(root, "users", users);

    char *json_string = cJSON_Print(root);
    FILE *file = fopen(filename, "w");
    if (file) {
        fputs(json_string, file);
        fclose(file);
    }

    free(json_string);
    cJSON_Delete(root);
}

// Add a new user to the database
void add_user(Database *db, const char *name, const char *username, const char *password) {
    // Resize if necessary
    if (db->user_count == db->capacity) {
        db->capacity *= 2;
        db->users = (User *)realloc(db->users, db->capacity * sizeof(User));
    }

    // Add the new user
    strncpy(db->users[db->user_count].name, name, sizeof(db->users[db->user_count].name));
    strncpy(db->users[db->user_count].username, username, sizeof(db->users[db->user_count].username));
    strncpy(db->users[db->user_count].password, password, sizeof(db->users[db->user_count].password));
    db->user_count++;
}

// Free the allocated memory for the database
void free_database(Database *db) {
    free(db->users);
    db->users = NULL;
    db->user_count = 0;
    db->capacity = 0;
}
