#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void process_schedules()
{
    sqlite3 *connection;
    sqlite3_stmt *stmt;
    if(sqlite3_open("database.sqlite3", &connection))
    {
        printf("Failed to open database\n");
        exit(1);
    }

    char sql[] = "SELECT * FROM Schedules WHERE day = strftime('%w', 'now') AND start <= strftime('%H%M', 'now') <= end AND (last IS NULL OR last < date('now'))";
    if(sqlite3_prepare_v2(connection, sql, strlen(sql), &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to compile SQL statement\n");
        exit(1);
    }

    // Launch a recording backend for each scheduled program
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        char sql2[50];
        sprintf(sql2, "UPDATE Schedules SET last=date('now') WHERE id=%d", sqlite3_column_int(stmt, 0));
        if(sqlite3_exec(connection, sql2, NULL, NULL, NULL) != SQLITE_OK)
        {
            printf("Failed to update schedule\n");
            exit(1);
        }
        // launch backend
    }
}
