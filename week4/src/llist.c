#include <stdio.h>
#include <stdlib.h>
#include <modbus-tcp.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>                            //Inluding Neccissary Header Files

int runs=0;
size_t rs;

struct list_object_s {                         //Structure with 3 members
    char *string;                   
    int strlen;                    
    struct list_object_s *next;                //Dynamic Pointer to Next List Item
};

struct list_object_s *list_head;               //Pointer to the Linked List Head

int compare_func(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);                   //return -1, 0, 1
	
} 

void add_to_list(char *input) {
 
    struct list_object_s *last_item;           //Pointer to the Last Item
    struct list_object_s *new_item = malloc(sizeof(struct list_object_s));
    if (!new_item) {
        fprintf(stderr, "Memory Alocation has Failed, Aborting\n");
        exit(1);
    }                                          //Error Message, No Memory Could Be alocated
                                               //-> is member of a structure
    new_item->string = strdup(input);          //new_item.string is the string iput value 
    new_item->strlen = strlen(input);          //new_item.strlen is the string length
    new_item->next = NULL;                     //new_item.next is a null value

    if (list_head == NULL) {                   //If no list head exists (first run)
       
        list_head = new_item;                  //create a list head
    } else {
        
        last_item = list_head;                 //Make the last item the list head
        while (last_item->next) last_item = last_item->next;
        last_item->next = new_item;            //point to the new item
    }
}

void print_and_free(void) {
    struct list_object_s *old_object, *cur_object;
    char *arr[runs];
    int i=0;
    size_t ii=0;

    cur_object = list_head;
    printf("Loop Took %d Input(s)\n",runs);

    printf("\nUnsorted Inputs Are: ");
    do {
        old_object = cur_object;               //make the current object old
        cur_object = cur_object->next;         //move to the next object  
	arr[i]=old_object->string;             //copy old-object.string into array
	printf(" %s ",arr[i]);                 //print in input order
	i++;                                   //move to next array cell
    } while (cur_object);                      //Keep Doing while a current object Exists

    printf("\nSorted Inputs Are:   "); 
    rs=sizeof(arr) / sizeof(char *);           //find string length size for qsort 
    qsort(arr, rs, sizeof(char *), compare_func);

    for(ii=0; ii<rs; ii++)                     //while ii is less then amount of inputs
    {
        printf(" %s ", arr[ii]);               //print sorted values
    }
    printf("\nFreeing Memory...\n");

    cur_object = list_head;  
    do {
	old_object = cur_object;               //make the current object old
        cur_object = cur_object->next;         //move to the next object  
	free(old_object->string);              //Free Memory
        free(old_object);                      //Free Memory
    } while (cur_object);

    printf("Memory Success Released\n");
}

int main(int argc, char **argv) {              //Main Function, Will be run First
    modbus_t *ctx;                             //Create a Modbus Pointer
    int option_index, c, counter, counter_given = 0; //Initialise Integers to 0
    char input[256]; 

    struct option long_options[] = {
        { "count",      required_argument,  0, 'c' },  //Count Argument
        { "directive",  no_argument,        0, 'd' },  //Directive Argument
        { 0 }
    };

    while (1) {                                //Infinite Loop
        c = getopt_long(argc, argv, "c:d", long_options, &option_index);

        if (c == -1) break;                    //Break Infinite Loop if C is -1
//This Break is for Part 1 of Homework, if no option is given run until EOF

        switch (c) {
            case 'c':                          //If c Exists 
                //printf("Got count argument with value %s\n", optarg);
                counter = atoi(optarg);        //Print and convert Count to interger 
                counter_given = 1;             //Flag Counter Value
		fprintf(stderr, "stderr: Count is: %d\n", counter ); //HW Part 2
                break;
            case 'd':
                printf("Got directive argument\n");
                break;
        }
    }

    while (scanf("%256s", input) != EOF) {     //While the input is not EOF

       
        add_to_list(input);                    //Run Function add_to_list with input as input
        if (counter_given) {                   //If a counter flag exists
            counter--;                         //subtract a value from the counter
            if (!counter) 
	    {
		runs++;
		break;               //if the counter is 0 break the input loop
	    }
        }
	runs++;
    }

    
    print_and_free();                          //print out the Values and Free Memory
   

   // printf("Linked list object is %li bytes long\n",
   //                 sizeof(struct list_object_s));

    return 0;
}

