#include "map.h"
#include "sub.h"


int main() {
    Map *map = map_create();
    create_key_value_socket(map);
    return 0;
}
