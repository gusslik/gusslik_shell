#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COMMAND_BUF_MAX_LEN 32
#define COMMAND_MAX_LEN     256     

// Parses the command entered by the user
ssize_t parse_command(int fd, char *buf, size_t max_buf_size){
    size_t i = 0;
    ssize_t res;
    char c;

    while(i < max_buf_size - 1){
        res = read(fd, &c, 1);
        
        if(res < 0)
            return -1;
        else if(res == 0 || c == '\n')
            break;

        buf[i] = c;
        i++;
    }

    buf[i] = '\0';
    return i; 
}

int main(){
    char    command_buf[COMMAND_BUF_MAX_LEN][COMMAND_MAX_LEN];
    int     command_buf_len = 0;     


    printf(
        "Welcome to Gusslik Shell\n"
        "------------------------\n");

    while(1){
        parse_command(STDIN_FILENO, command_buf[command_buf_len], COMMAND_MAX_LEN);
        printf("You entered: %s\n", command_buf[0]);
        break;
    }


    return 0;
}