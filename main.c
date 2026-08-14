#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COMMAND_BUF_MAX_LEN 32
#define COMMAND_MAX_LEN     256     
#define TOKENS_MAX_NUM      32
#define TOKEN_MAX_LEN       256

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

size_t command_to_tokens(char tokens[][TOKEN_MAX_LEN], size_t max_num_tokens, size_t max_token_len, char *command){
    size_t i = 0, j = 0, n_token = 0;
    int isToken = 0;
    
    while(n_token < max_num_tokens && command[j]){
        for(j = i; command[j] != ' ' && command[j] != '\n' && command[j]; j++){
            isToken = 1;
            if(j - i + 1 < max_token_len){
                tokens[n_token][j - i] = command[j];
            }
        }
        
        if(isToken){
            isToken = 0;
            tokens[n_token][j - i + 1] = '\0';
            n_token++;
        }
        
        i = j + 1;
    }

    return n_token;
}

int main(){
    char    command_buf[COMMAND_BUF_MAX_LEN][COMMAND_MAX_LEN];
    int     command_buf_len = 0;     

    char    tokens[TOKENS_MAX_NUM][TOKEN_MAX_LEN];
    int     tokens_num = 0;
    
    printf(
        "Welcome to Gusslik Shell\n"
        "------------------------\n");

    while(1){
        parse_command(STDIN_FILENO, command_buf[command_buf_len], COMMAND_MAX_LEN);
        printf("You entered: %s\n", command_buf[0]);
        
        tokens_num = command_to_tokens(tokens, TOKENS_MAX_NUM, TOKEN_MAX_LEN, command_buf[0]);

        for(size_t i = 0; i < tokens_num; i++){
            printf("Token %ld: %s\n", i + 1, tokens[i]);
        }

        break;
    }


    return 0;
}