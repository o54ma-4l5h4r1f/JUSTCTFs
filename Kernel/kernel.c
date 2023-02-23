#include<stdio.h>
#include <string.h>
#define MAX_FILES 3
#define DEF_PERM "-rw-"
#define USERNAME "user"
#define USERID 1
#define ROOTID 0


struct File {
		int userID;
		int size;
		char perm[5];
		char name[20];
		char content[100];
} Files[MAX_FILES];

int filescount = 0;
int CurrentUserID = USERID;

// #======================= HELPER FUNCTIONS =========================# //

char opts[1]; 		// max number of options
char *filename[1];

char *getopts(char *cmd){
	opts[0] = '\0';
	filename[0] = '\0';

	if(strlen(cmd) > 0){
		for(int i = 0, j = 0; ; i++){
			if(cmd[i+1] != '\0'){
				if(cmd[i] == '-'){
					// printf("%c\n", cmd[i]);
					opts[j++] = cmd[i+1];
				}
			}
			else if (opts[0] != '\0'){
				// printf("%s\n", opts);
				return opts;
			}
			else {
				break;
			}
		}
		if(opts[0] == '\0'){
			for(int i = 0, j = 0; ; i++){
				if(cmd[i+1] != '\0'){
					if(cmd[i] == ' ' && cmd[i+1] != ' '){
						filename[j++] = cmd+i+1;
					}
				}
				else if (filename[0]){
					// printf("%s\n", filename[0]);
					return filename[0];
				}
				else {
					break;
				}
			}
		}
	}
	return opts;
}

void input(char str[], int nchars) {
  	int i = 0;
  	int ch;
  	while((ch = getchar()) != '\n' && ch != EOF) {
    	if (i < nchars) {
       		str[i++] = ch;
    	}
  	}
  	str[i] = '\0';
}

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

// #===================== END HELPER FUNCTIONS =======================# //

void ls(char *opts){

	if(opts[0] == 'l'){
		// -rwx o54ma size a.out
		for(int i = 0; i < MAX_FILES; i++){
			if(*Files[i].name){
				printf("%s %s:0x%08x\t%d %s\n", Files[i].perm, 
					// etcpasswd[Files[i].userID], 
					(Files[i].userID == ROOTID) ? "root" : (Files[i].userID == USERID) ? USERNAME : "????",
					Files[i].userID,
					Files[i].size, 
					Files[i].name);
			}
		}
	}
	else if(opts[0] != '\0'){
		printf("ls: invalid option -- '%c'\n", opts[0]);
	}
	else{
		// print names of files in one line !!
		for(int i = 0; i < MAX_FILES; i++){
			if(*Files[i].name){
				printf("%s  ", Files[i].name);
			}
		}
		printf("\n");
	}
}

void read(char *filename){

	if(*filename == '\0'){
		printf("Usage: read <FileName>\n");
		return;
	}

	for(int i = 0; i < MAX_FILES; i++){
		if(!strcmp(Files[i].name, filename)){
			if(CurrentUserID == Files[i].userID){
				if(!strcmp(Files[i].name, "flag.txt")){
					flag();
					return;
				}
				else{
					printf("%s\n", Files[i].content);
					return;
				}
			}
			else {
				printf("read: %s: Permission denied, non matching IDs\n", filename);
				return;
			}
		} 
	}
	printf("read: %s: No such file or directory\n", filename);
	return;
}

void write(char *filename){

	if(filescount >= MAX_FILES){
		printf("sorry, we are out of memory );\n", filename);
		return;
	}

	if(*filename == '\0'){
		printf("Usage: write <FileName>\n");
		return;
	}

	for(int i = 0; i < MAX_FILES; i++){
		if(!strcmp(Files[i].name, filename)){
			printf("write: %s: File already exists, you can't edit with 'write'\n", filename);
			return;
		}
	}
	printf("editor>> ");
	char cont[100];
	input(cont, 100);

	strcpy(Files[filescount].perm, DEF_PERM);
	strcpy(Files[filescount].name, filename);
	strcpy(Files[filescount].content, cont);
	// strcpy(Files[filescount].owner, USERNAME);
	Files[filescount].userID = CurrentUserID;
	Files[filescount].size = strlen(Files[filescount].content);

	filescount++;
}

void help(){
	printf("The available commands :\n");
	printf("ls\tUsage: ls [-l]\t\t information about the FILEs (permission  username:id  size  filename)\n");
	printf("read\tUsage: read <Filename>\t print files content\n");
	printf("write\tUsage: write <FileName>\t create a file and write into it (MAX 100 bytes)\n");
	printf("help\tUsage: help\t\t display this help\n");
}

void prompt(){
	printf("> ");
	
	char cmd[256];
	input(cmd, 256);

	if (!strncmp(cmd, "ls", 2)){
		ls(getopts(cmd));
	}
	else if (!strncmp(cmd, "read", 4)){
		read(getopts(cmd));
	}
	else if (!strncmp(cmd, "write", 5)){
		write(getopts(cmd));
	}
	else if (!strncmp(cmd, "help", 4) || !strncmp(cmd, "?", 1)){
		help(cmd);
	}
	else if(!strncmp(cmd, "", 1)){
		return;
	}
	else{
		printf("%s: command not found\n", cmd);
		printf("Try 'help' for more information.\n");
	}
}

void bootup(){
	setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

 //    Users[0].id = ROOTID;
	// strcpy(Users[0].name, "root");    

	strcpy(Files[MAX_FILES -1].perm, "-r--");
	strcpy(Files[MAX_FILES -1].name, "flag.txt");
	// strcpy(Files[MAX_FILES -1].owner, "root");
	strcpy(Files[MAX_FILES -1].content, "JUST{xxxxxxxxxxxxxx}");
	Files[MAX_FILES -1].userID = ROOTID;
	Files[MAX_FILES -1].size = strlen(Files[MAX_FILES -1].content);		
}


int main(int argc, char **argv, char **envp){
	bootup();

	while(1){
		prompt();
	}
	return 0;
}