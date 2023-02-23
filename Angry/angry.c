#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input(char str[]) {
    int i = 0;
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF) {
        str[i++] = ch;
    }
    str[i] = '\0';
}

void helper(void) {
  __asm__(
    "pop %r14\n"
    "xchg %rdi, %r14\n"
    "ret\n"
  );
}

int main(){
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    
    char s[64];
    printf("What do you want ? •`_´• %p\n", puts);

    input(s);

    if(strlen(s) < 64){
        puts("NO");
        return 0;
    }
    else{
        exit(0);
    }
}