#include "river.h" 

sem_t *sem_counter = NULL,        
    *sem_log = NULL,            
    *sem_hacker_wait = NULL,  
    *sem_serf_wait = NULL,       
    *sem_molo = NULL,           
    *sem_board = NULL,          
    *sem_ride = NULL,           
    *sem_landing = NULL,        
    *sem_finish = NULL;

param* parse_arguments(int argc, char **argv){
    char* mod;
    param *par = malloc(sizeof(param));

    if(argc != 7)
        fprintf(stderr, "Incorrect quantity of arguments\n"), exit(1);

    if((par->p = strtol(argv[1], &mod, 10))%2 || *mod || par->p < 2)
        fprintf(stderr, "Incorrect argument of P: must be integer, P => 2 && P%%2=0\n"), exit(1);

    if((par->h = strtol(argv[2], &mod, 10)) >2000 || par->h < 0 || *mod)
        fprintf(stderr, "Incorrect argument of H: must be integer, H => 0 && H <= 2000\n"), exit(1);    

    if((par->s = strtol(argv[3], &mod, 10)) >2000 || par->s < 0 || *mod)
        fprintf(stderr, "Incorrect argument of S: must be integer, S => 0 && S <= 2000\n"), exit(1);

    if((par->r = strtol(argv[4], &mod, 10)) >2000 || par->r < 0 || *mod)
        fprintf(stderr, "Incorrect argument of R: must be integer, R => 0 && R <= 2000\n"), exit(1);

    if((par->w = strtol(argv[5], &mod, 10)) >2000 || par->w < 20 || *mod)
        fprintf(stderr, "Incorrect argument of W: must be integer,  W=> 20 && W <= 2000\n"), exit(1);

    if((par->c = strtol(argv[6], &mod, 10)) < 5 || *mod)
        fprintf(stderr, "Incorrect argument of C: must be integer && C => 5\n"), exit(1);    

    return par;
}

int init_sem(int *sharedM[], int shared[]){
    int fail = 0;

    if (     ((sem_counter = sem_open("/semafor_counter", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
          || ((sem_log = sem_open("/semafor_log", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
          || ((sem_molo = sem_open("/semafor_molo", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
          || ((sem_board = sem_open("/semafor_board", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
          || ((sem_ride = sem_open("/semafor_ride", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
          || ((sem_hacker_wait = sem_open("/semafor_hacker_wait", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
          || ((sem_serf_wait = sem_open("/semafor_serf_wait", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
          || ((sem_landing = sem_open("/semafor_landing", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
          || ((sem_finish = sem_open("/semafor_finish", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)   )
    fprintf(stderr, "Can`t create semaphores\n"), fail++;       
    
    if ( ((shared[0] = shm_open("/sharedMemory0", O_CREAT | O_EXCL | O_RDWR, 0644)) == -1)
          || ((shared[1] = shm_open("/sharedMemory1", O_CREAT | O_EXCL | O_RDWR, 0644)) == -1)
          || ((shared[2] = shm_open("/sharedMemory2", O_CREAT | O_EXCL | O_RDWR, 0644)) == -1)
          || ((shared[3] = shm_open("/sharedMemory3", O_CREAT | O_EXCL | O_RDWR, 0644)) == -1))
    fprintf(stderr, "Cannot create shared memory\n"), fail++;

    if (    (ftruncate(shared[0], sizeof(int)) == -1)
         || (ftruncate(shared[1], sizeof(int)) == -1)
         || (ftruncate(shared[2], sizeof(int)) == -1)
         || (ftruncate(shared[3], sizeof(int)) == -1))
    fprintf(stderr, "Truncating file size has failed->\n"), fail++;

    if (  ((sharedM[0] = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared[0], 0)) == MAP_FAILED)
          || ((sharedM[1] = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared[1], 0)) == MAP_FAILED)
          || ((sharedM[2] = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared[2], 0)) == MAP_FAILED)
          || ((sharedM[3] = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared[3], 0)) == MAP_FAILED))
    fprintf(stderr, "Mapping file to memory has failed->\n"), fail++;

    return fail;

    return fail;
}

int close_sem(int *sharedM[], int shared[]){
    int fail = 0;

    if (    (sem_close(sem_counter) == -1)
         || (sem_close(sem_log) == -1)
         || (sem_close(sem_ride) == -1)
         || (sem_close(sem_hacker_wait) == -1)
         || (sem_close(sem_serf_wait) == -1)
         || (sem_close(sem_molo) == -1)
         || (sem_close(sem_board) == -1)
         || (sem_close(sem_landing) == -1)
         || (sem_close(sem_finish) == -1))
    fprintf(stderr, "Cannot close semaphores\n"), fail++;

    if (    (sem_unlink("/semafor_counter") == -1)
         || (sem_unlink("/semafor_log") == -1)
         || (sem_unlink("/semafor_ride") == -1)
         || (sem_unlink("/semafor_hacker_wait") == -1)
         || (sem_unlink("/semafor_serf_wait") == -1)
         || (sem_unlink("/semafor_molo") == -1)
         || (sem_unlink("/semafor_board") == -1)
         || (sem_unlink("/semafor_landing") == -1)
         || (sem_unlink("/semafor_finish") == -1)   )
    fprintf(stderr, "Removing shared memory has failed->\n"), fail++;

    if (    ((munmap(sharedM[0], sizeof(int))) != 0)
         || ((munmap(sharedM[1], sizeof(int))) != 0)
         || ((munmap(sharedM[2], sizeof(int))) != 0)
         || ((munmap(sharedM[3], sizeof(int))) != 0))
    fprintf(stderr, "Unmapping file from memory has failed->\n"), fail++;

    if (    (shm_unlink("/sharedMemory0") == -1)
         || (shm_unlink("/sharedMemory1") == -1)
         || (shm_unlink("/sharedMemory2") == -1)
         || (shm_unlink("/sharedMemory3") == -1))
    fprintf(stderr, "Removing shared memory has failed->\n"), fail++;
    

    if (    (close(shared[0]) == -1)
         || (close(shared[1]) == -1)
         || (close(shared[2]) == -1)
         || (close(shared[3]) == -1))
    fprintf(stderr, "Closing file of shared memory has failed->\n"), fail++;

    return fail;
}

int create_subprocess(FILE *fp, param *par, int *sharedM[], int category){

    pid_t pid_fork;
    int i = 0;
    int sleep = category ? par->h : par->s;
    
    if(!sleep)
        sleep++;                 // Prevents (random() % 0) to occur->

    while (i++ < par->p)
    {
        usleep((random() % (sleep)) * 1000);
                             // Forking will happen in random time form 0ms to maximum time to generate child-process of given category->
        pid_fork = fork();

        if (pid_fork == 0)
        {                    // Forked child-process of given category->
            create_child(fp, par, category, i, sharedM);
        }

        else if (pid_fork < 0)
        {                   // Forking child-process failed->
            fprintf(stderr, "%s", "Forking new child process has failed->\n");
            return 2;
        }

    }

    waitpid(-1, NULL, 0);
    exit(0);
}

int create_child(FILE *file, param *par, int category, int number, int *sharedM[]){
    int isCaptain = 0;
    char* name = category ? "HACK" : "SERF";
    
    sem_wait(sem_log);       // Child-process was created and reports it to the log->
    fprintf(file, "%d\t: %s %d\t: starts\n", counter(sharedM[3], 1), name, number);
    sem_post(sem_log);

    sem_wait(sem_board);     // To eneter the pier, processes must wait till current rivercrossing is over->

    sem_wait(sem_log);      // After entering the pier, process raise counter of waiting processes of its category and reports to the log->
    counter(sharedM[category], 1);
    fprintf(file, "%d\t: %s %d\t: waits\t\t: %d\t: %d\n", counter(sharedM[3], 1), name, number, *sharedM[1], *sharedM[0]);
    sem_post(sem_log);

    if(*sharedM[category] == 4){
        for(int i = 0; i < 4; i++)
            category ? sem_post(sem_hacker_wait) : sem_post(sem_serf_wait);
        counter(sharedM[category], -4);
        isCaptain = 1;

        sem_wait(sem_log);
        fprintf(file, "%d\t: %s %d\t: boards\t\t: %d\t: %d\n", counter(sharedM[3], 1), name, number, *sharedM[1], *sharedM[0]);
        sem_post(sem_log);
    }
    else if(*sharedM[category] == 2 && *sharedM[!category] >= 2){
        sem_post(sem_hacker_wait);
        sem_post(sem_hacker_wait);
        sem_post(sem_serf_wait);
        sem_post(sem_serf_wait);
        
        counter(sharedM[category], -2);
        counter(sharedM[!category], -2);

        isCaptain = 1;

        sem_wait(sem_log);
        fprintf(file, "%d\t: %s %d\t: boards\t\t: %d\t: %d\n", counter(sharedM[3], 1), name, number, *sharedM[1], *sharedM[0]);
        sem_post(sem_log);
    }
    else {
        sem_post(sem_board);
    }

    category ? sem_wait(sem_hacker_wait) : sem_wait(sem_serf_wait);
    
    board(sharedM[2], sem_landing);
                             // Ship members waits for its captain to land->
    sem_wait(sem_landing);

    if (!isCaptain)
    {
        sem_wait(sem_log);       // After crossing the river is finished, processes land and report it to the log->
        fprintf(file, "%d\t: %s %d\t: member exits\t\t: %d\t: %d\n", counter(sharedM[3], 1), name, number, *sharedM[1], *sharedM[0]);
        sem_post(sem_log);
        sem_post(sem_finish);
    }
    else{
        sem_wait(sem_finish);
        sem_wait(sem_finish);
        sem_wait(sem_finish);

        sem_wait(sem_log);       // After crossing the river is finished, processes land and report it to the log->
        fprintf(file, "%d\t: %s %d\t: captain exits\t\t: %d\t: %d\n", counter(sharedM[3], 1), name, number, *sharedM[1], *sharedM[0]);
        sem_post(sem_log);

        srand(time(NULL)*getpid());
                             // Randomize random number generator->
        usleep((random() % (par->r + 1)) * 1000);
        sem_post(sem_board);

                             // Captain is put to sleep for time needed to perform crossing the river->
    }
    exit(0);
}

int counter(int *x, int y)
{
    int cache = 0;

    sem_wait(sem_counter);
    (*x) = (*x) + y;
    cache = *x;              // New value is stored in local variable to return->
    sem_post(sem_counter);

    return cache;
}

void board(int *seat, sem_t *sem_target)
{

    counter(seat, 1);        // Raise (safely) shared value (seat counter) by 1->
    sem_wait(sem_counter);

    if (*seat == 4)
    {
        (*seat) = 0;
        sem_post(sem_target);
        sem_post(sem_target);
        sem_post(sem_target);
        sem_post(sem_target);
                             // Will unlock needed amount of given semaphores in loop->
    }

    sem_post(sem_counter);
}
