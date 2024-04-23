#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "map.h"
#include "sub.h"
int main()
{

    // make two process which run same
    // program after this instruction
    pid_t p = fork();
    if(p<0){
        perror("fork fail");
    }else if(p > 0){

        printf("Hello Main!, process_id(pid) = %d \n",getpid());
        //declare global because this will be shared mem in the future.
        Map *map = map_create();
        create_key_value_socket(map);

    }
    else{
        printf("Hello Child!, process_id(pid) = %d \n",getpid());
    }
}