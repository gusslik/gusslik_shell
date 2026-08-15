#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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

int checkPipe(char **args){
    for(int i = 0; args[i] != NULL; i++){
        if(strcmp(args[i], "|") == 0)
            return 1;
    }

    return 0;
}

void seperatePipeCommands(char **args, char **args1, char **args2){
    int i, j;

    for(i = 0; strcmp(args[i], "|") != 0; i++){
        for(j = 0; args[i][j]; j++){
            args1[i][j] = args[i][j];
        }
        args1[i][strcspn(args1[i], "\n")] = '\0';
    }

    for(i = i + 1; args[i]; i++){
        for(j = 0; args[i][j]; j++){
            args2[i][j] = args[i][j];
        }
        args2[i][strcspn(args2[i], "\n")] = '\0';
    }
}

int executePipe(char **args){
    char **args1, **args2;

    seperatePipeCommands(args, args1, args2);
    
    return 0;
}

int executeCustomCommand(char **args){
    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        if(execvp(args[0], args) < 0){
            perror("execution failed");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    default:
        wait(NULL);
    }

}



int main(){
    char    input[MAX_INPUT];
    char    *tokens[MAX_TOKENS];

    while(1){
        printDir();
        readInput(input);
        tokenize(input, tokens);
        if(checkPipe(tokens)){
            executePipe(tokens);
        }
        else{
            if(!executeBuiltInCommand(tokens))
                executeCustomCommand(tokens);
        }

    }

    return 0;
}