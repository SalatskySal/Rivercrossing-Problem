#include "river.h"

int main(int argc, char **argv){
    pid_t hacker, serf;
    
    FILE *file = fopen("river.out", "w");

    setbuf(file, NULL); 

    int *sharedM[4];
    int shared[4];
    
    param *parameter = parse_arguments(argc, argv);
    if(init_sem(sharedM, shared)){
        close_sem(sharedM, shared);
    }

    *sharedM[0] = 0;
    *sharedM[1] = 0;
    *sharedM[2] = 0;
    *sharedM[3] = 0;

    if(!(hacker = fork())){
        create_subprocess(file, parameter, sharedM, HACKER);
    }
    else if(hacker < 0){
        
    }

    srand(time(NULL) * getpid());

    if(!(serf = fork())){
        create_subprocess(file, parameter, sharedM, SERF);
    }
    else if(serf < 0){
        
    }

    waitpid(-1, NULL, 0);    // Level 2 master sayian assurance.
    
    close_sem(sharedM, shared);

    fclose(file);

    free(parameter);
}