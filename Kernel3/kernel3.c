#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILES 10
#define DEF_PERM "-rw-"
#define USERNAME "user"
#define USERID 0xffffffff
#define ROOTID 0x13371337
#define FILE_NAME_LEN 40

struct File {
		int userID;
		int size;
		char perm[5];
		char name[FILE_NAME_LEN];
		char content[100];
};


struct File *Files[MAX_FILES];			// we increased the size of the memory !! (1)

int8_t memmap[MAX_FILES] = {0};
int8_t Index[MAX_FILES] = {0};


int filescount = 0;
// int CurrentUserID = USERID;

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
					if(strlen(filename[0]) <= FILE_NAME_LEN){
						return filename[0];
					}else{
						// filename[0] = '\0';
						printf("error: passed argument too long\n");
						break;
					}
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
  	while((ch = getchar()) != '\n' && ch != EOF ) {
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


void set_next_valid_index(int8_t idx){
	for(int i = MAX_FILES-1; i >= 0; i--){
		if(Index[i] == 0){
			Index[i] = idx;
			return;
		}
	}
}

int8_t get_next_valid_index(){
	int tmp; 
	for(int i = 0; i < MAX_FILES; i++){
		if(Index[i] != 0){
			tmp = Index[i];
			Index[i] = 0;
			return tmp;
		}
	}
	return filescount;

}

// #===================== END HELPER FUNCTIONS =======================# //

void ls(char *opts){

	if(opts[0] == 'l'){
		// -rwx o54ma:id size a.out
		for(int i = 0; i < MAX_FILES; i++){
			if(memmap[i]){
				printf("%s %s:0x%08x\t%d %s\n", Files[i]->perm, 
					// etcpasswd[Files[i].userID], 
					(Files[i]->userID == ROOTID) ? "root" : (Files[i]->userID == USERID) ? USERNAME : "????",
					Files[i]->userID,
					Files[i]->size, 
					Files[i]->name);
			}
		}
	}
	else if(opts[0] != '\0'){
		printf("ls: invalid option -- '%c'\n", opts[0]);
	}
	else{
		// print names of files in one line !!
		for(int i = 0; i < MAX_FILES; i++){
			if(memmap[i]){
				printf("%s  ", Files[i]->name);
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
		if(memmap[i]){
			if(!strcmp(Files[i]->name, filename)){
				if(USERID == Files[i]->userID){
					if(!strcmp(Files[i]->name, "flag.txt")){
						flag();
						return;
					}
					else{
						printf("%s\n", Files[i]->content);
						return;
					}
				}
				else {
					printf("read: %s: Permission denied, non matching IDs\n", filename);
					return;
				}
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
		if(memmap[i]){
			if(!strcmp(Files[i]->name, filename)){
				printf("write: %s: File already exists, you can't edit with 'write'\n", filename);
				return;
			}
		}
	}


	int idx = get_next_valid_index();

	Files[idx] = (struct File*) malloc(sizeof(struct File));

	printf("editor>> ");
	input(&Files[idx]->content, 100);

	strcpy(Files[idx]->perm, DEF_PERM);
	strcpy(Files[idx]->name, filename);
	// strcpy(Files[filescount]->content, cont);
	Files[idx]->userID = USERID;
	Files[idx]->size = strlen(Files[idx]->content);

	memmap[idx] = 1;
	filescount++;
}

void echo(char *string){
	printf("%s\n", string);
}

void edit(char *filename){
	if(*filename == '\0'){
		printf("Usage: edit <FileName>\n");
		return;
	}

	for(int i = 0; i < MAX_FILES; i++){
		if(memmap[i]){
			if(!strcmp(Files[i]->name, filename)){
				if(USERID == Files[i]->userID){
					printf("editor>> ");
					input(&Files[i]->content, 1000);	// BUG !!! 
					Files[i]->size = strlen(Files[i]->content);
					return;
				}
				else {
					printf("edit: %s: Permission denied, non matching IDs\n", filename);
					return;
				}
			}
		}
	}
	printf("edit: %s: No such file or directory\n", filename);
	return;
}

void rm(char *filename){
	if(*filename == '\0'){
		printf("Usage: read <FileName>\n");
		return;
	}

	for(int i = 0; i < MAX_FILES; i++){
		if(memmap[i]){
			if(!strcmp(Files[i]->name, filename)){
				if(USERID == Files[i]->userID){
					if(!strcmp(Files[i]->name, "flag.txt")){
						printf("really !!\n");
						return;
					}
					else{
						// printf("%s\n", Files[i]->content);
						free(Files[i]);
						set_next_valid_index(i);
						memmap[i] = 0;
						filescount--;
						return;
					}
				}
				else {
					printf("rm: %s: Permission denied, non matching IDs\n", filename);
					return;
				}
			}
		}
	}

	printf("rm: %s: No such file or directory\n", filename);
	return;
}

void leak(char *filename){
	if(*filename == '\0'){
		printf("Usage: leak <FileName>\n");
		return;
	}

	for(int i = 0; i < MAX_FILES; i++){
		if(memmap[i]){
			if(!strcmp(Files[i]->name, filename)){
				if(USERID == Files[i]->userID){
					if(!strcmp(Files[i]->name, "flag.txt")){
						printf("really !!\n");
						return;
					}
					else{
						printf("%p\n", Files[i]);
						return;
					}
				}
				else {
					printf("leak: %s: Permission denied, non matching IDs\n", filename);
					return;
				}
			}
		}
	}

	printf("leak: %s: No such file or directory\n", filename);
	return;
}

void help(){
	printf("The available commands :\n");
	printf("ls\tUsage: ls [-l]\t\t list information about the FILEs (permission  username:id  size  filename)\n");
	printf("read\tUsage: read <Filename>\t print files content\n");
	printf("write\tUsage: write <FileName>\t create a file and write into it\n");
	printf("echo\tUsage: echo [string]\t display a line of text\n");
	printf("edit\tUsage: edit <Filename>\t overwrite files content\n");
	printf("leak\tUsage: leak <Filename>\t print files memory location\n");
	printf("rm\tUsage: rm <Filename>\t remove files\n");
	printf("help\tUsage: help or ?\t display this help\n");
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
	else if (!strncmp(cmd, "echo", 4)){
		echo(getopts(cmd));
	}
	else if (!strncmp(cmd, "edit", 4)){
		edit(getopts(cmd));
	}
	else if (!strncmp(cmd, "leak", 4)){
		leak(getopts(cmd));
	}
	else if (!strncmp(cmd, "rm", 2)){
		rm(getopts(cmd));
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

    Files[0] = (struct File*) malloc(sizeof(struct File));

	strcpy(Files[0]->perm, "-r--");
	strcpy(Files[0]->name, "flag.txt");
	// strcpy(Files[MAX_FILES -1].owner, "root");
	strcpy(Files[0]->content, "JUST{xxxxxxxxxxxxxx}");
	Files[0]->userID = ROOTID;
	Files[0]->size = strlen(Files[0]->content);	

	filescount++;
	memmap[0] = 1;
}


int main(int argc, char **argv, char **envp){
	bootup();

	while(1){
		prompt();
	}
	return 0;
}