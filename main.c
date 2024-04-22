#include "main.h"

int main() {
    //declare global because this will be shared mem in the future.
    Map *map = map_create();
    create_key_value_socket(map);
}