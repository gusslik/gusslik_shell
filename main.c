#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_INPUT   1024  
#define MAX_TOKENS  32

void readInput(char *input){
    printf("\n> ");
    if(fgets(input, MAX_INPUT, stdin) == NULL){
        exit(0);
    }
    
    input[strcspn(input, "\n")] = 0;
}

void tokenize(char *input, char **tokens){
    int i = 0;

    tokens[i] = strtok(input, " ");

    while(i < MAX_TOKENS && tokens[i] != NULL){
        i++;
        tokens[i] = strtok(NULL, " ");
    }
}

void printDir(){
    char cwd[1024];

    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        return;
    }

    printf("%s", cwd);
}

int executeBuiltInCommand(char **args){
    char *command = args[0];
    
    if(command == NULL){
        return 0;
    }
        

    if(strcmp(command, "cd") == 0){
        if(args[1] == NULL){
            printf("Usage: cd <argument>\n");
        }
        else{
            if(chdir(args[1]) == -1){
                perror("cd error");
            }
        }

        return 1;
    }

    if(strcmp(command, "exit") == 0){
        printf("Exiting shell...\n");
        exit(0);
    }
    

    return 0;
}




int main(){
    char    input[MAX_INPUT];
    char    *tokens[MAX_TOKENS];

    while(1){
        printDir();
        readInput(input);
        tokenize(input, tokens);
        executeBuiltInCommand(tokens);
    }

    return 0;
}