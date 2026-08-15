#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 32

void readInput(char *input)
{
    printf("\n> ");
    if (fgets(input, MAX_INPUT, stdin) == NULL)
    {
        exit(0);
    }

    input[strcspn(input, "\n")] = 0;
}

void tokenize(char *input, char **tokens)
{
    int i = 0;

    tokens[i] = strtok(input, " ");

    while (i < MAX_TOKENS && tokens[i] != NULL)
    {
        i++;
        tokens[i] = strtok(NULL, " ");
    }
}

void printDir()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return;
    }

    printf("%s", cwd);
}

int executeBuiltInCommand(char **args)
{
    char *command = args[0];

    if (command == NULL)
    {
        return 0;
    }

    if (strcmp(command, "cd") == 0)
    {
        if (args[1] == NULL)
        {
            printf("Usage: cd <argument>\n");
        }
        else
        {
            if (chdir(args[1]) == -1)
            {
                perror("cd error");
            }
        }

        return 1;
    }

    if (strcmp(command, "exit") == 0)
    {
        printf("Exiting shell...\n");
        exit(EXIT_SUCCESS);
    }

    return 0;
}

int checkPipe(char **args)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
            return 1;
    }

    return 0;
}

void seperatePipeCommands(char **args, char **args1, char **args2)
{
    int pipe_pos;

    for (pipe_pos = 0; strcmp(args[pipe_pos], "|") != 0; pipe_pos++)
        ;

    int i;
    if (pipe_pos == -1)
    {
        for (i = 0; args[i]; i++)
        {
            args1[i] = args[i];
        }

        args1[i] = NULL;
        args2[0] = NULL;
    }
    else
    {
        for (i = 0; strcmp(args[i], "|") != 0; i++)
        {
            args1[i] = args[i];
        }
        args1[i] = NULL;

        int j;
        for (j = 0; args[pipe_pos + j + 1]; j++)
        {
            args2[j] = args[pipe_pos + j + 1];
        }
        args2[j] = NULL;
    }
}

int executePipe(char **args)
{
    char *args1[MAX_TOKENS], *args2[MAX_TOKENS];

    seperatePipeCommands(args, args1, args2);

    int pipefd[2];
    pipe(pipefd);

    pid_t p1 = fork();

    if (p1 == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        if (execvp(args1[0], args1) < 0)
        {
            perror("execution failed");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    pid_t p2 = fork();

    if (p2 == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        if (execvp(args2[0], args2) < 0)
        {
            perror("execution failed");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    pid_t pid;
    while ((pid = wait(NULL)) > 0)
        ;

    return 0;
}

int executeCustomCommand(char **args)
{
    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        if (execvp(args[0], args) < 0)
        {
            perror("execution failed");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    default:
        wait(NULL);
    }
}

int main()
{
    char input[MAX_INPUT];
    char *tokens[MAX_TOKENS];

    while (1)
    {
        printDir();
        readInput(input);
        tokenize(input, tokens);
        if (checkPipe(tokens))
        {
            executePipe(tokens);
        }
        else
        {
            if (!executeBuiltInCommand(tokens))
                executeCustomCommand(tokens);
        }
    }

    return 0;
}