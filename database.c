#include <stdio.h>
#include "cJSON.h"

typedef struct
{
    char name[50];
    char username[50];
    char password[50];
} User;

typedef struct
{
    User *users;
    size_t user_count;
} Database;

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (!data)
    {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);
    return data;
}

Database load_database(const char *filename)
{
    Database db = {NULL, 0};

    // Read the file
    char *json_string = read_file(filename);
    if (!json_string) return db;

    // Parse JSON
    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    if (!root) return db;

    // Get the "users" array
    cJSON *users = cJSON_GetObjectItem(root, "users");
    if (!cJSON_IsArray(users))
    {
        cJSON_Delete(root);
        return db;
    }

    // Allocate memory for users
    db.user_count = cJSON_GetArraySize(users);
    db.users = (User *)malloc(db.user_count * sizeof(User));

    // Populate the structure
    cJSON *user;
    size_t i = 0;
    cJSON_ArrayForEach(user, users)
    {
        cJSON *name = cJSON_GetObjectItem(user, "name");
        cJSON *username = cJSON_GetObjectItem(user, "username");
        cJSON *password = cJSON_GetObjectItem(user, "password");

        if (cJSON_IsString(name) && cJSON_IsString(username) && cJSON_IsString(password))
        {
            strncpy(db.users[i].name, name->valuestring, sizeof(db.users[i].name));
            strncpy(db.users[i].username, username->valuestring, sizeof(db.users[i].username));
            strncpy(db.users[i].password, password->valuestring, sizeof(db.users[i].password));
            i++;
        }
    }

    cJSON_Delete(root);
    return db;
}

void save_database(const char *filename, Database *db)
{
    // Create JSON root
    cJSON *root = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();

    for (size_t i = 0; i < db->user_count; i++)
    {
        cJSON *user = cJSON_CreateObject();
        cJSON_AddStringToObject(user, "name", db->users[i].name);
        cJSON_AddStringToObject(user, "username", db->users[i].username);
        cJSON_AddStringToObject(user, "password", db->users[i].password);
        cJSON_AddItemToArray(users, user);
    }

    cJSON_AddItemToObject(root, "users", users);

    // Write JSON to file
    char *json_string = cJSON_Print(root);
    FILE *file = fopen(filename, "w");
    if (file)
    {
        fputs(json_string, file);
        fclose(file);
    }

    free(json_string);
    cJSON_Delete(root);
}
