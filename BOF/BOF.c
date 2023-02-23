#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void flag(){
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

void setup(){
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void Hi(){
    char buf1[128];
    size_t n = read(0, &buf1, 128);
    return;
}

void Bye(){
    char buf2[64];
    if (!strncmp(buf2 + (rand() % 5) * 8, "Good Bye", 8)) {
        flag();
    }
}

int main(){
    
    setup();
    Hi();
    Bye();

    return 0;
}