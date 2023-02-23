#include<stdio.h>
#include <float.h>
#include <limits.h> 
#define DIGS DECIMAL_DIG

void flag(){
	puts("------------------------------------------");
	puts("Well Done, the guardian wasn't that strong after all !!");
    
    char c;
    FILE *fp=fopen("./flag.txt","r");
    if(fp){
        while( (c=getc(fp)) != EOF)
            putchar(c);
        fclose(fp);
    } else{
        puts("Flag file not found!");
    }
}


int main(){
	setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

	puts("------------------------------------------");
   	puts("X is the remaining years for the guardian to live !");
   	puts("Can you make X = 0 ??");

	double N;
	double X = 1337.1337;
	printf("X = %f\n", X);

	while(X != 0){
		puts("------------------------------------------");
		printf("> ");
		
		if (scanf("%lf", &N) != 1){
		  fprintf(stderr,"Input not recognised as an integer, please try again.\n");
		  return 0;
		}
		else if(N <= 0){
			puts("you power doesn't work this way );");
			N = 1.0;
		}
		else{

			X = X * N;

			if(X >= DBL_MAX){
				puts("You've lost, now the guardian is undefeatable !!");
				return 0;
			}
		}

		printf("%.*e\n", DIGS, X);

		if (X == 0){
			flag();
			return 0;
		}
    }
    return 0;
}