#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"

char** splitByWhitespace(char longArray[1024], int* numSubarrays) {
    char** subarrays = (char**)malloc(1024 * sizeof(char*)); // Allocate memory for subarrays
    if (!subarrays) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    char *token;
    char *rest = longArray;
    int count = 0; // Counter for subarrays

    // Tokenize the input string and store subarrays
    while ((token = strtok_r(rest, ":", &rest))) {
        subarrays[count] = strdup(token);
        if (!subarrays[count]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        count++;
    }

    *numSubarrays = count; // Update numSubarrays with the count
    return subarrays;
}

int main() {
    //set up map
    Map *map = map_create();

    //take first command
    printf("Welcome to the server please enter a command!\n");
    char command[1024];
    scanf(" %[^\n]", command);

    //split the command
    int numSubarrays;
    char** subarrays = splitByWhitespace(command, &numSubarrays);
    const char* method = subarrays[0];

    //loop until "QUIT" is entered
    while(strcmp(method,"QUIT") != 0){

        if(strcmp(method,"PUT") == 0){
            const char* key = subarrays[1];
            const char* value = subarrays[2];
            map_insert_element(map, key, value);
            printf("Inserted: %s and %s.\n",key,value);

        }else if(strcmp(method,"GET") == 0){
            const char* key = subarrays[1];
            const char* value = map_get_element(map, key);
            printf("Got: %s. The Value is: %s\n",key,value);

        }else if(strcmp(method,"DEL") == 0){
            const char* key = subarrays[1];
            map_delete_element(map, key);
            printf("Deleted: %s.\n",key);

        }else if(strcmp(method,"HLP") == 0){
            printf("<Help>\nPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HLP\", \"QUIT\" \n</Help>\n");
        }else{
            printf("Command not found, please enter a valid command. Enter \"HLP\" to so see possible commands.");
        }

        //get new command
        printf("Please enter a new command. \n");
        scanf("%s",&command);

        //split the new command
        subarrays = splitByWhitespace(command, &numSubarrays);
        method = subarrays[0];
    }

    //Show that the program has ended
    printf("Main terminated, because \"QUIT\" was entered.\n");
    return 0;
}
