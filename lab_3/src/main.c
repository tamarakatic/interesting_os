#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <stdbool.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
#define READ_END 0
#define WRITE_END 1

char **lsh_split_line(char *);
char *lsh_read_line(void);
int lsh_execute(char **);

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_launch(char **args);
int lsh_pipe(char **args);
int lsh_multi_pipe(char **args);
int lsh_redirection(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int lsh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args)
{
    int i;
    printf(" ******* Tamara Katic's Shell *******\n");
    printf(" ******* Please type program names and arguments, and hit enter: *******"
            "\n\tcd"
            "\n\tls"
            "\n\thelp"
            "\n\texit"
            "\n\tunix default commands"
            "\n\tredirection"
            "\n\tpipe and multiple pipes\n"); 

    return 1;
}

int lsh_multi_pipe(char **args) {
    int fds1[2], fds2[2];

    int index = 0, len_command1 = 0, len_command2 = 0;

    if (pipe(fds1) < 0) {
        perror("lsh");
    }

    if (pipe(fds2) < 0) {
        perror("lsh");
    }

    while (args[index] != NULL) {
        if (strcmp(args[index], "|") == 0  && len_command1 == 0) {
            len_command1 = index;
        } else if (strcmp(args[index], "|") == 0 && len_command2 == 0) {
            len_command2 = index;
        }
        index++;
    }

    char * commands1[len_command1 + 1];
    char * commands2[len_command2 - len_command1];
    char * commands3[index - len_command2];

    int i;
    for (i = 0; i < len_command1; i++) {
        commands1[i] = args[i];
    }

    commands1[i] = NULL;


    int j = 0;
    for (i = len_command1 + 1; i < len_command2; i++) {
        commands2[j] = args[i];
        j++;
    }
    commands2[j] = NULL;

    int k = 0;
    for (i = len_command2+1; i < index; i++) {
        commands3[k] = args[i];
        k++;
    }
    commands3[j-1] = NULL;

    pid_t pid = fork();
    int in_file_descr = dup(STDIN_FILENO);
    int out_file_descr = dup(STDOUT_FILENO);    

    if (pid < 0) {
        return errno;
    }

    if (pid == 0) {
        dup2(fds1[1], STDOUT_FILENO);
        close(fds1[0]);
        close(fds1[1]);
        close(fds2[0]);
        close(fds2[1]);

        execvp(commands1[0], commands1);
        perror(commands1[0]);
    } else {
        // printf("Parent process PID (%d) Child PID (%d) \n", getpid(), pid);
        pid = fork();

        if (pid < 0) {
            return errno;
        } 
        if (pid == 0) {
            dup2(fds1[0], STDIN_FILENO);
            dup2(fds2[1], STDOUT_FILENO);
            close(fds1[0]);
            close(fds1[1]);
            close(fds2[0]);
            close(fds2[1]);
            
            execvp(commands2[0], commands2);
            perror(commands2[0]);
        } else {
            // printf("Parent process PID (%d) Child PID (%d) \n", getpid(), pid);
            dup2(fds2[0], STDIN_FILENO);
            close(fds1[0]);
            close(fds1[1]);
            close(fds2[0]);
            close(fds2[1]);

            lsh_launch(commands3);

            dup2(in_file_descr, STDIN_FILENO);
            dup2(out_file_descr,  STDOUT_FILENO);
            return 1;
        }
    }
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status); 
}

int has_pipe(char **args) {
    int index = 0, numPipe = 0;
  
    while (args[index] != NULL) {
        if (strcmp(args[index], "|") == 0) {
            numPipe++;
        }
        index++;
    }

    return numPipe;
}


int lsh_pipe(char **args)
{
    int index = 0;
    int len_command = 0, pipeIdx = -1;
  
    while (args[index] != NULL) {
        if (strcmp(args[index], "|") == 0) {
            len_command = index;
        }
        index++;
    }

    char * commands[len_command + 1];
    for (int i=0; i < index; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipeIdx = i;
            commands[i] = NULL;
            break;
        }
        commands[i] = args[i];
    }

    char * new_commands[index-pipeIdx];
    int j=0;
    int i;

    for (i=pipeIdx+1; i < index; i++) {
        if (args[i] == NULL) {
            break;
        }
        new_commands[j] = args[i];
        j++;
    }

    new_commands[j] = NULL;

    pid_t pid = fork();

    if (pid < 0) {
        return errno;
    } else if (pid == 0) {
        int fds[2];
        int in_file_descr = dup(STDIN_FILENO);
        int out_file_descr = dup(STDOUT_FILENO);   

        if (pipe(fds) < 0) {
            perror("lsh");
        }

        pid_t pid_1 = fork();
        if (pid_1 < 0){
            return errno;
        }

        if (pid_1 == 0) {
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]); // close pip
            close(fds[1]);
            execvp(commands[0], commands);
            perror(commands[0]);

        } else {
            // printf("Parent process PID (%d) Child PID (%d) \n", getpid(), pid_1);
            int status;
            waitpid(pid_1, &status, 0);

            if (WIFEXITED(status))
            {
                dup2(fds[0], STDIN_FILENO);
                close(fds[1]);
                close(fds[0]);
                lsh_launch(new_commands);
            }

        }

        dup2(in_file_descr, STDIN_FILENO);
        dup2(out_file_descr,  STDOUT_FILENO);
        return 1;

    } else {
        // printf("Parent process PID (%d) Child PID (%d) \n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    return 0;
}

int has_redirection(char **args) {
    int index = 0;
    int result = 0;
    while(args[index] != NULL) {
        // ls > test.txt
        if (strcmp(args[index], ">") == 0) {
           result = 1;
            // cat < test.txt
        } else if (strcmp(args[index], "<") == 0) {
            result = 1;
        } else if (strcmp(args[index], ">>") == 0) {
            result = 1;
        }
        index++;
    }

    return result;
}

int lsh_redirection(char **args) {
    char *inFile = NULL;
    char *outFile = NULL;

    bool isInput = false;
    bool isOutput = false;
    bool isOutputAppend = false;

    int index = 0, end_index = 0;
    while(args[index] != NULL) {
        // ls > test.txt
        if (strcmp(args[index], ">") == 0) {
            isOutput = true;
            outFile = args[index+1];
            end_index = index;
            // cat < test.txt
        } else if (strcmp(args[index], "<") == 0) {
            isInput = true;
            inFile = args[index+1];
            end_index = index;
        } else if (strcmp(args[index], ">>") == 0) {
            isOutputAppend = true;
            outFile = args[index+1];
            end_index = index;
        }
        index++;
    }

    char * commands[end_index];

    int i;

    pid_t pid = fork();

    int in_file_descr = dup(STDIN_FILENO);
    int out_file_descr = dup(STDOUT_FILENO); 
    int out_file, in_file;

    if (pid < 0){
        return errno;
    }

    if (pid == 0) {
        if (isInput) {
            in_file = freopen(inFile, "r", stdin);

        for (i=0; i < end_index; i++) {
            commands[i] = args[i];
        }
        commands[i] = NULL;
        dup2(in_file, STDIN_FILENO);
        close(in_file);
        lsh_launch(commands);


        } else if (isOutput) {
            out_file = freopen(outFile, "w", stdout);

            for (i=0; i < end_index; i++) {
                commands[i] = args[i];
            }
            commands[i] = NULL;
            dup2(out_file,  STDOUT_FILENO);
            close(out_file);
            lsh_launch(commands);

        } else if (isOutputAppend) {
            freopen(outFile, "a+", stdout);

            for (i=0; i < end_index; i++) {
                commands[i] = args[i];
            }
            commands[i] = NULL;
            lsh_launch(commands);
        }

        dup2(in_file_descr, STDIN_FILENO);
        dup2(out_file_descr,  STDOUT_FILENO);
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status); 
    }
    
    return 0;

}

int lsh_exit(char **args)
{
    return 0;
}

char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while(token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

char *lsh_read_line(void)
{
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        c = getchar();

        if (c == EOF) {
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position ++;

        if (position >= bufsize) {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


int lsh_launch(char **args)
{
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("lsh");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


int lsh_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        } 
    }

    if(has_redirection(args)){
        return lsh_redirection(args);
    } else if (has_pipe(args) == 1){
        return lsh_pipe(args);
    } else if(has_pipe(args) > 1) {
        return lsh_multi_pipe(args); 
    }

    return lsh_launch(args);
}


void lsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}


int main(int argc, char **argv) {
    lsh_loop();

    return EXIT_SUCCESS;
}