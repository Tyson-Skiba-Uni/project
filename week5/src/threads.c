#include <stdio.h>
#include <stdlib.h>
#include <modbus-tcp.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>

struct list_object_s {
    char *string;                   /* 8 bytes */
    int strlen;                     /* 4 bytes */
    struct list_object_s *next;     /* 8 bytes */
};


static pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t list_data_ready=PTHREAD_COND_INITIALIZER;
static pthread_cond_t list_data_flush=PTHREAD_COND_INITIALIZER;
static struct list_object_s *list_head;

static void add_to_list(char *input) {     //static gives only local access
   
    struct list_object_s *last_item;
    struct list_object_s *new_item = malloc(sizeof(struct list_object_s));
    if (!new_item) {
        fprintf(stderr, "Malloc failed\nAborting!");
        exit(1);
    }

  
    new_item->string = strdup(input);
    new_item->strlen = strlen(input);
    new_item->next = NULL;

    //list head is shared between threads, lock to prevent data corruption
    pthread_mutex_lock(&list_lock);
    

    if (list_head == NULL) {
       
        list_head = new_item;
    } else {
       
        last_item = list_head;
        while (last_item->next) last_item = last_item->next;
        last_item->next = new_item;
    }

//Release the lock and notify that data is avaliable
pthread_cond_signal(&list_data_ready);
pthread_mutex_unlock(&list_lock);
}

static struct list_object_s *list_get_first(void) {
    struct list_object_s *first_item;
    
    first_item=list_head;
    list_head=list_head->next;
    return first_item;
}

static void *print_and_free(void *arg) {
    struct list_object_s *cur_object;
    printf("Thread Begins\n");

    

    while(1) {
    //wait for data to be avaliable
    
    pthread_mutex_lock(&list_lock);

    while(!list_head){
	pthread_cond_wait(&list_data_ready,&list_lock); ///Data may not actually be avaliabe so we check for empty file
    }

    //take lock due to shared data however the above when returned sucesfully will already hold lock
    cur_object=list_get_first();
    pthread_mutex_unlock(&list_lock);

    printf("T2: String is: %s\n", cur_object->string);
    printf("T2: String length is %i\n", cur_object->strlen);
    free(cur_object->string);
    free(cur_object);
    	    
    //work has been done post signal 
    pthread_cond_signal(&list_data_flush);
    }
   
}

static void list_flush(void) {
    pthread_mutex_lock(&list_lock);
    while(list_head){
	pthread_cond_signal(&list_data_flush);
	pthread_cond_wait(&list_data_flush, &list_lock);
    }
    pthread_mutex_unlock(&list_lock);
}

int main(int argc, char **argv) {
    modbus_t *ctx;
    int option_index, c, counter, counter_given = 0;
    pthread_t print_thread;
    char input[256];
    

    struct option long_options[] = {
        { "count",      required_argument,  0, 'c' },
        { "directive",  no_argument,        0, 'd' },
        { 0 }
    };

    printf("Week 5: Threads\n\n\n");

    while (1) {
        c = getopt_long(argc, argv, "c:d", long_options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 'c':
                printf("Got count argument with value %s\n", optarg);
                counter = atoi(optarg);
                counter_given = 1;
                break;
            case 'd':
                printf("Got directive argument\n");
                break;
        }
    }

    //Create Thread 1 and Call it print_thread
    pthread_create(&print_thread, NULL, print_and_free, NULL);	

    while (scanf("%256s", input) != EOF) {
     
        add_to_list(input);
        if (counter_given) {
            counter--;
            if (!counter) break;
        }
    }


    printf("Linked list object is %li bytes long\n",
                    sizeof(struct list_object_s));

    //block here untill all objects are printed
    list_flush();

    return 0;
}
