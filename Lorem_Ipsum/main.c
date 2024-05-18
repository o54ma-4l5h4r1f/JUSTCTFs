#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

char lorem[500] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec egestas malesuada feugiat. Etiam dapibus rutrum felis, vitae congue urna suscipit faucibus. Nam vel lectus blandit, consectetur purus id, gravida felis. Aenean sit amet odio nec lectus facilisis tempor. Curabitur ornare consectetur ligula, ut fermentum dolor faucibus sed. In et enim ligula. Integer consequat nisi id rhoncus pretium. Vestibulum erat eros, tempus sit amet ante ut, hendrerit commodo nisl.";

void sigAlarm(int sig) {
    printf("\ntimeout .. enough time editing ^^\n");
    exit(1);
}

void win(){
    system("cat flag.txt");
}

void init(){
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    signal(SIGALRM, sigAlarm);
    alarm(60);
}

void main(){
    init();
    int index;
    char substring[100];

    while (true) {
        printf("%s\n\n", lorem);
        printf("Enter the index: ");
        scanf("%d", &index);
        if(index == -1)
            break;
        printf("Enter the value: ");
        ssize_t size = read(0, substring, sizeof(substring));
        strncpy(&lorem[index], substring, size - 1);
    }
    return;
}