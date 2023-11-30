//Lucía Docampo Rodríguez

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <wordexp.h>
#include "shell.h"

#define ROJO   "\x1b[31m"
#define BRILLO "\x1b[1m"
#define RESET  "\x1b[0m"
#define MAX_TOKENS 100
#define MAX_PATHS 1000
#define LINESIZE 100
#define MAXLINES 100


int parseString(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;

    return i;
}
//------------------------------------------------------------------------------------
//	COMMANDS	
//------------------------------------------------------------------------------------

int exit_cmd(char *argv[], int argc) {
    return 1;
}

int help (char *argv[], int argc){
    
    if(argv[1]==NULL){
        printf("\n'help cmd' where cmd is one of the next commands:\n");
        printf(" exit\n ");printf("cd\n "); printf("tail\n "); printf("uniq\n "); 
        printf("version \n");
    }else if(!strcmp(argv[1], "exit")){
        printf("\nexit: ends the ejecution of the shell\n");
    } else if(!strcmp(argv[1], "cd")){
        printf("\ncd: hange the current working directory\n");
    } else if(!strcmp(argv[1], "tail")){
        printf("\ntail [-c | -n | -q | -v]: display the last 10 lines of the notes file.\n");
        printf("-c : display the notes file a page at a time, beginning with the bytes you want\n");
        printf("-n : specify the number of lines to start reading from the end of the notes file\n");
        printf("-q : if you do not want to print the headers, you can use quiet mode with -q\n");
        printf("-v : always print file name headers\n");
    } else if(!strcmp(argv[1], "uniq")){
        printf("\nuniq [-i | -d | -u]: helps to detect the adjacent duplicate lines and also deletes the duplicate lines\n");
        printf("-i : case insensitive comparisons can be made\n");
        printf("-d : it only prints the repeated lines and not the lines which aren’t repeated\n");
        printf("-u : it allows you to print only unique lines\n");
    } else if(!strcmp(argv[1], "help")){
        printf("\nhelp [cmd]:  Show help about commands\n");
    } else if(!strcmp(argv[1], "version")){
        printf("\nversion [-a | -d | -h]: Shows info about the autor [-a], the date [-d] and the hour [-h]\n");}

    return 0;
}

int version(char *argv[], int argc){

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char fecha[100];
    if(argv[1]==NULL){
        printf("\nOperating Systems proyect, by Lucía Docampo\n");
    }else{
        if (!strcmp(argv[1],"-a")) {
            printf("\nLucia Docampo Rodriguez\n");
        }
        if(!strcmp(argv[1],"-d")){
            strftime(fecha, 100, "%d/%m/%Y",tm);
            printf("\n%s\n", fecha);
        }
        if (!strcmp(argv[1],"-h")) {
        	  strftime(fecha, 100, "%H:%M:%S",tm);
        	  printf("\n%s\n", fecha);
        }
    }
    return 0;
}

int cd(char *argv[], int argc){
     char direccion [100];
     if (argv[1]==NULL){
          printf("\n%s\n", getcwd(direccion, 100));
     } else{
          if (chdir(argv[1])!=0){
               printf("\nImpossible to change directory: No such file or directory\n");
          }
     }
     return 0;
}


int tail(char *argv[], int argc){
    int pidd;

    if((pidd = fork()) == -1){
        perror("");
    } else {
        if (pidd > 0) {
            waitpid(pidd, WEXITED, NULL);
        } else {
            if (pidd == 0) {
                if (execvp("tail", argv) == -1) { //runs
                    perror("No ejecuted: ");
                }
                exit(0);
            }
        }
    }
    return 0;
}




int uniq(char *argv[], int argc){
    int pidd;

    if((pidd = fork()) == -1){
        perror("");
    } else {
        if (pidd > 0) {
            waitpid(pidd, WEXITED, NULL);
        } else {
            if (pidd == 0) {
                if (execvp("uniq", argv) == -1) {//runs
                    perror("No ejecuted: ");
                }
                exit(0);
            }
        }
    }
    return 0;
}


//------------------------------------------------------------------------------------
struct cmd {
    char *cmd_name;
    int (*cmd_fun)(char *argv[], int argc);
};


struct cmd cmds[] = {
    {"version", version},
    {"cd", cd},
    {"tail", tail},
    {"uniq", uniq},
    {"help", help},
    {"exit", exit_cmd},
    {NULL, NULL}
};

int processCmd(char *argv[], int argc) {
    int i;
    for(i=0; cmds[i].cmd_name != NULL; i++) {
        if(argv[0] == NULL){//if you dont put anything
            return 0;
        }
        if(strcmp(argv[0], cmds[i].cmd_name) == 0)//if they are the same as one of the commands from the other function
            return cmds[i].cmd_fun(argv, argc);
    }

    printf("\nOffending '%s'\n", argv[0]);
    return 0;
}

void printDir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n%s", cwd);
}


int main(int argc, char *argv[], char *env[]) {
    char *line = (char *)NULL;
    int end = 0;

    

    while(!end) {
    	   printDir();
    	   readline;
       
        if(line){
		free(line);
		line = (char *)NULL;
	   }
	   
	   line = readline(ROJO BRILLO "\n>> " RESET);
	   if(line && *line)
		add_history(line);
		
		wordexp_t input;
		wordexp(line, &input, WRDE_REUSE);
		
        
        end = processCmd(input.we_wordv, input.we_wordc);
        
    }
    
}
