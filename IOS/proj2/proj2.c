/** MAIN *****************
	main.c
	Reseni IOS-proj2, 2.5.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9
****************************/


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>


#define printerr(s, ...) fprintf(stderr, s"\n", __VA_ARGS__);
#define printerr0(s) fprintf(stderr, s"\n");


typedef struct {
    long p;       //Pocet procesu (pasazeru)
    long c;       //Kapacita voziku
    long pt;      //max ms, po které je generován nový proces pro pasažéra; PT >= 0 && PT < 5001
    long rt;      //max ms průjezdu trati; RT >= 0 && RT < 5001
} options;

typedef struct {
	unsigned line;
	unsigned order;
} share;



/***************
 Cte argumenty
 Naplni strukturu options daty
 ******************/
bool read_args(int *argc, char **argv, options *options) {
    if(*argc != 5) {
        printerr("Bad count of arguments (%i). Requiered 4!", *argc);
        return false;
    }

    long tmp = 0;
    char *err = 0;

    for(unsigned i = 1 ; i < 5 ; i++) {
        tmp = strtoul(argv[i], &err, 10);
        if(err == argv[i] || *err != '\0' || tmp < 0) {
            printerr("Invalid %i. argument %s, unsigned integer expected!", i, argv[i]);
            return false;
        }

        switch(i) {
            case 1:
                if(tmp <= 0) {
                    printerr0("P is must be bigger than 0!");
                    return false;
                }
                options->p = tmp;
                break;

            case 2:
                if(tmp <= 0 || tmp > options->p || options->p % tmp) {
                    printerr0("C is must be bigger than 0 or it's must be smaller than P or P %% C must be 0!");
                    return false;
                }
                options->c = tmp;
                break;

            case 3:
                if(tmp < 0 || tmp >= 5000) {
                    printerr0("PT >= 0 && PT < 5001!");
                    return false;
                }
                options->pt = tmp;
                break;

            case 4:
                if(tmp < 0 || tmp >= 5000) {
                    printerr0("RT >= 0 && RT < 5001!");
                    return false;
                }
                options->rt = tmp;
                break;

            default:
                printerr0("Some other error...");
                return false;
        }
    }

    return true;
}



/**************
 **** MAIN ****
 *************/
int main(int argc, char *argv[]){
    bool all_OK = true;
    FILE *output_file = 0;
	share *share = 0;

   // int line = 0, order = 0;
    int err;

    options options = {0, 0, 0, 0};
    all_OK = read_args(&argc, argv, &options);

    if(!all_OK)
        return 1;


///Priprava semaforu
    srand(time(NULL));

    output_file = fopen("proj2.out","w+");
    if(!output_file) {
        printerr0("Problem with creating file!");
        return 1;
    }
    pid_t pass;
    pid_t coaster;
    pid_t pass_gen;
	
	share = mmap(NULL, sizeof(share), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	if(share == MAP_FAILED)
		return 2;
	share->line = 0;
	share->order = 0;
	
    sem_t *c_loading = sem_open("/c_loading", O_CREAT | O_EXCL, 0644, 0);
    sem_t *c_unloading = sem_open("/c_unloading", O_CREAT | O_EXCL, 0644, 0);
    sem_t *c_empty = sem_open("/c_empty", O_CREAT | O_EXCL, 0644, 0);
    sem_t *c_full = sem_open("/c_full", O_CREAT | O_EXCL, 0644, 0);

    sem_t *protect = sem_open("/protect", O_CREAT | O_EXCL, 0644, 1);
    sem_t *finished = sem_open("/finished", O_CREAT | O_EXCL, 0644, 0);

    if(c_loading == SEM_FAILED || c_unloading == SEM_FAILED ||
       c_empty == SEM_FAILED || c_full == SEM_FAILED ||
       protect == SEM_FAILED || finished == SEM_FAILED) {

        printerr0("Problem with creating semaphores!");
        return 2;
    }


//Vytvorim generator pasazeru
    pass_gen = fork();
    if(!pass_gen) {     //pasazery jeste nejsou - generuji
        for(int i = 1 ; i < options.p + 1 ; i++){
            if(options.pt)
                usleep(rand() % options.pt);       //uspim na nahodnou dobu
            pass = fork();      //Vytvorim pasazera
            if(!pass){


            //Pasazer inicializovan
                sem_wait(protect);
				share->line ++;
                fprintf(output_file, "%i\t: P %i\t: started\n", share->line, i);
                fflush(output_file);
				sem_post(protect);
				
            //Cekam na vozik
                sem_wait(c_loading);
				
            //Nastoupil
				sem_wait(protect);
                share->line ++;
                fprintf(output_file, "%i\t: P %i\t: board\n", share->line, i);
                fflush(output_file);
				sem_post(protect);
				
                sem_wait(protect);
                share->order ++;
 
			//Plny vozik
                if(share->order >= options.c) {
                    share->line ++;
                    fprintf(output_file, "%i\t: P %i\t: board last\n", share->line, i);
                    fflush(output_file);
                    sem_post(c_full);
                }
                else{
                    share->line ++;
                    fprintf(output_file, "%i\t: P %i\t: board order %i\n", share->line, i, share->order);
                    fflush(output_file);
                }
                sem_post(protect);
				
			//Vozik prijel
                sem_wait(c_unloading);
                sem_wait(protect);

                share->line ++;
                fprintf(output_file, "%i\t: P %i\t: unboard\n", share->line, i);
                fflush(output_file);
				sem_post(protect);
			
			//Vystupuji
                sem_post(c_unloading);
				
                sem_wait(protect);
				share->order --;
				
            //Vozik prazdny
                if(!share->order){
                    share->line ++;
                    fprintf(output_file, "%i\t: P %i\t: unboard last\n", share->line, i);
                    fflush(output_file);
                    sem_post(c_empty);
                }
                else{
					share->line ++;
                    fprintf(output_file, "%i\t: P %i\t: unboard order %i\n", share->line, i, share->order);
                    fflush(output_file);
                }
            //Cekam na konec
                sem_post(protect);
                sem_wait(finished);
               
				sem_post(finished);
                sem_wait(protect);

                share->line ++;
                fprintf(output_file, "%i\t: P %i\t: finished\n", share->line, i);
                fflush(output_file);

                sem_post(protect);


                goto close_all;
            }

        }
    //Cekam na konec
        sem_wait(finished);
        sem_post(finished);

        goto close_all;
    }

//Inicializuji vozik
    else{
        coaster = fork();
        if(!coaster){       //Jeste neni - vytvorim
            sem_wait(protect);
            share->line ++;
            fprintf(output_file, "%i\t: C 1\t: started\n", share->line);
            fflush(output_file);
            sem_post(protect);
			
        //Cekam na pasazery
            for(int i = options.p / options.c ; i > 0; i--) {
                sem_wait(protect);
                share->line ++;
                fprintf(output_file, "%i\t: C 1\t: load\n", share->line);
                fflush(output_file);
                sem_post(protect);
				
        //Cekam na pasazery
                for(int i = 0 ; i < options.c ; i++) {
                    sem_post(c_loading);
                }
                sem_wait(c_full);
                sem_wait(protect);
				
        //Odjizdim
                share->line ++;
                fprintf(output_file, "%i\t: C 1\t: run\n", share->line);
                fflush(output_file);
                sem_post(protect);
				
                if(options.rt)
                    usleep(rand() % options.rt);
                sem_wait(protect);
				
        //Prijizdim a vyloduji
                share->line ++;
                fprintf(output_file, "%i\t: C 1\t: unload\n", share->line);
                fflush(output_file);

                sem_post(protect);
                sem_post(c_unloading);
				
                sem_wait(c_empty);
                sem_wait(c_unloading);
            }
            sem_wait(protect);
			
        //Dojezdil jsem
            share->line ++;
            fprintf(output_file,"%i\t: C 1\t: finished\n", share->line);
            fflush(output_file);

            sem_post(protect);
            sem_post(finished);

            goto close_all;
        }
    }

    //Cekám na ukonceni vseho
    for(int i = 0 ; i < options.p ; i++){
        wait(NULL);
    }


//Uklizim
	munmap(share, sizeof(share));

    sem_unlink("/c_loading");
    sem_unlink("/c_unloading");
    sem_unlink("/c_empty");
    sem_unlink("/c_full");
    sem_unlink("/protect");
    sem_unlink("/finished");

    err = fclose(output_file);
    if(err)
        fprintf(stderr, "Some problem with cloasing file!\n");

close_all:
    sem_close(c_loading);
    sem_close(c_unloading);
    sem_close(c_empty);
    sem_close(c_full);
    sem_close(protect);
    sem_close(finished);

    return 0;
}
