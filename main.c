#include <stdio.h>
#include "map.h"

int main() {
    printf("Entered Main\n");

    //create
    struct Map* map = map_create();

    //insert
    map_insert_element(map, "apple", "red");

    //get
    printf("Color of apple: %s\n", map_get_element(map, "apple"));

    //delete
    map_delete_element(map, "apple");

    //get
    printf("Color of apple after deletion: %s\n", map_get_element(map, "apple"));

    // Destroy the map and free memory
    map_destroy(map);

    printf("Main terminated\n");
    return 0;
}
