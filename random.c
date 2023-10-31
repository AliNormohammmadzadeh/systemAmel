#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc , char *argv[]){
    if(argc != 2){
        printf("Usage: %s <number_of_people>\n",argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    srand(time(NULL));

    int random = rand() % (n+1) +1 ;

    printf("The selected volunteer is person %d.\n", random);

    return 0;
}