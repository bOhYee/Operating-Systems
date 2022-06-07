#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#define STR_SIZE 256
#define MAX_ARGS_EXEC 5+1
#define MAX_COMMANDS 20

const char endCommand[10] = "end";

typedef enum{
    EXEC, FILENAME, VERSION
} params;

typedef enum{
    SYSTEM, EXECL
} exe_mode;

typedef struct Cmd{
    int num_params;
    char *c;
    char **params;
} Command;

int convertStrToInt(char *str);
void modCharStr(char *str, char find, char sub);

void initCommands(Command **cmd_struct);
void freeCommands(Command *cmd_struct);
int readFile(int file, Command *cmd_str);

int executeCommand(Command *cmd_struct, int id_cmd, int mode);
int sysCommand(Command *cmd_struct, int id);
int execCommand(Command *cmd_struct, int id);

int main (int argc, char **argv) {

    int fd;                     // File descriptor
    int mode;                   // Program execution mode
    int num_cmd = 0;
    Command *cmd_struct = NULL;

    // Checking the arguments
    if(argc < 3){
        printf("Error: not enough arguments!\n");
        return -1;
    }

    fd = open(argv[FILENAME], O_RDONLY);
    if(fd == -1){
        printf("Error: the file cannot be opened!\n");
        return -1;
    }
    mode = convertStrToInt(argv[VERSION]);
    if(mode == -1){
        printf("Error while processing the mode parameter!\n");
        exit(1);
    }

    mode = mode - 1;
    initCommands(&cmd_struct);
    num_cmd = readFile(fd, cmd_struct);
    for(int i = 0; i < num_cmd; i++)
        executeCommand(cmd_struct, i, mode);

    freeCommands(cmd_struct);
    return 0;
}

void initCommands(Command **cmd_struct){

    *cmd_struct = (Command *) malloc(MAX_COMMANDS * sizeof(Command));
    for(int i = 0; i < MAX_COMMANDS; i++){
        (*cmd_struct)[i].c = (char *) malloc(STR_SIZE * sizeof(char));
        (*cmd_struct)[i].params = (char **) malloc(MAX_ARGS_EXEC * sizeof(char *));
        (*cmd_struct)[i].num_params = 0;
        for(int l = 0; l < MAX_ARGS_EXEC; l++)
            (*cmd_struct)[i].params[l] = (char *) malloc(STR_SIZE * sizeof(char));
    }

}

void freeCommands(Command *cmd_struct){

    for(int i = 0; i < MAX_COMMANDS; i++){
        if(cmd_struct[i].c != NULL)
            free(cmd_struct[i].c);

        for(int l = 0; l < MAX_ARGS_EXEC; l++)
            if(cmd_struct[i].params[l] != NULL)
                free(cmd_struct[i].params[l]);

        free(cmd_struct[i].params);
    }

    free(cmd_struct);
}

int readFile(int file, Command *cmd_str){

    int new_cmd;                // New command flag (0-No; 1-Yes)
    int cmd_curr, arg_curr, cmd_read;
    char *word = NULL;
    char buffer[STR_SIZE] = "";

    cmd_curr = 0;
    arg_curr = 0;
    cmd_read = 0;
    new_cmd = 1;

    // Reading the file
    while(read(file, buffer, STR_SIZE) > 0){
        modCharStr(buffer, '\r', ' ');
        modCharStr(buffer, '\n', ' ');
        word = strtok(buffer, " ");
        while(word != NULL) {
            if(strcmp(word, endCommand) == 0) {
                cmd_str[cmd_curr].params[arg_curr] = NULL;
                new_cmd = 1;
                arg_curr = 0;
                cmd_curr++;
            }
            else {
                if(new_cmd == 1) {
                    cmd_read++;
                    strcpy(cmd_str[cmd_curr].c, word);
                    cmd_str[cmd_curr].num_params++;
                    strcpy(cmd_str[cmd_curr].params[arg_curr++], word);
                    new_cmd = 0;
                }
                else {
                    cmd_str[cmd_curr].num_params++;
                    strcpy(cmd_str[cmd_curr].params[arg_curr++], word);
                }
            }
            word = strtok(NULL, " ");
        }
    }

    return cmd_read;
}

void modCharStr(char *str, char find, char sub){

    for(int i = 0; str[i] != '\0'; i++)
        if(str[i] == find)
            str[i] = sub;

}

int convertStrToInt(char *str){

    int value;
    char *endptr = NULL;

    value = (int) strtol(str, &endptr, 10);
    if(str == endptr || (value == 0 && errno != 0) || value < 0)
        return -1;

    if(value < 1 || value > 2)
        return -1;

    return value;
}

int executeCommand(Command *cmd_struct, int id_cmd, int mode){

    int status = 0;   // Status code of the system function

    switch (mode){
        case SYSTEM:
            status = sysCommand(cmd_struct, id_cmd);
            break;
        case EXECL:
            status = execCommand(cmd_struct, id_cmd);
            break;
        default:
            status = -1;
            break;
    }

    return status;
}

int sysCommand(Command *cmd_struct, int id){

    int status;
    char command[STR_SIZE];

    strcpy(command, cmd_struct[id].c);
    // Avoid using the "id of the program" parameter
    for(int i = 1; i < cmd_struct[id].num_params; i++) {
        strcat(command, " ");
        strcat(command, cmd_struct[id].params[i]);
    }

    status = system(command);
    if(status == -1 || status == 127)
        return -1;

    return 0;
}

int execCommand(Command *cmd_struct, int id){

    int status;
    pid_t pid;

    pid = fork();
    if(pid == 0){
        status = execvp(cmd_struct[id].c, cmd_struct[id].params);
        if(status == -1) {
            exit(status);
        }
        else{
            sleep(3);
            exit(0);
        }
    }
    else{
        wait(&status);
    }

    return status;
}