#include "libft.h"

#define DEFAULT_READ_SIZE 42

pid_t  input_redirection (int input_file_fd , char *cmd_path , char ** args , char **env)
{
    pid_t pid;

   char *argv[1] ; 

    argv[0] = NULL; 

     if (input_file_fd < 0)
    {
        perror("There was an error openning the input file");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("There was an error creating a child process");
        exit(1);
    }

    if (!pid)
    { 
        if (dup2(  input_file_fd , STDIN_FILENO   ) < 0 )
        {
            perror("There was an error in output redirection");
            exit(1);
        }
        if (execve(cmd_path , argv , env)  < 0)  
        {   
            perror("There was an error executing the command after output redirection");
            exit(1) ; 
        }
    }
    return pid ; 
}

pid_t output_redirection(int output_file_fd , char *cmd_path , char **args , char **env)
{

    pid_t pid;
    char *argv[1] ; 

    argv[0] = NULL; 

    if (output_file_fd < 0)
    {
        perror("There was an error openning the output file");
        exit(1);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("There was an error creating a child process");
        exit(1);
    }
    if (!pid)
    {
        if (dup2(  output_file_fd   ,    STDOUT_FILENO   ) < 0 )
        {
            perror("There was an error in output redirection");
            exit(1);
        }
        if (execve(cmd_path , argv , env)  < 0)  
        {
            perror("There was an error executing the command after output redirection");
            exit(1) ; 
        }
    }
    return pid;
}

char* append_data(char *src  , char *append )
{
    char *new_buffer;
    int i;
    int j;

    if (!src)
        return ft_strdup(append);
    i = 0;
    j = 0;
    new_buffer = malloc( sizeof(char)* ( ft_strlen(src) + ft_strlen(append) + 1));
    while (src[i])
    {
        new_buffer[j]  = src[i] ; 
        i++;
        j++;
    }
    i = 0;
    while(append[i])
    {
        new_buffer[j]  = append[i] ; 
        i++;
        j++;
    }
    new_buffer[j] = '\0';
    free(src);
    return new_buffer;
}

char*  get_file_data(int output_file_fd)
{
    int bytes_read;
    char* buffer ;
    char* src ; 

    buffer = malloc(sizeof(char) * ( DEFAULT_READ_SIZE  + 1 ) );
    src = NULL ;
    bytes_read = 1;
    while (bytes_read)
    {
        bytes_read = read(  output_file_fd  , buffer , DEFAULT_READ_SIZE  ) ; 
        buffer[bytes_read] = '\0';
        src = append_data(src , buffer ) ; 
    }
    free(buffer);
    return src;
}

 void output_redirection_append_mode(char *cmd_path  , char **args , char **env , int output_file_fd)
 {
    char* file_data ; 
    char* command_ouput ; 
    char* final_append ; 
    int fd ; 

    file_data = get_file_data ( output_file_fd)  ; 
    output_redirection( output_file_fd , cmd_path , args , env);
    command_ouput =   get_file_data( output_file_fd) ; 
    final_append = ft_strjoin (file_data , command_ouput) ;
    free(file_data);
    free(command_ouput);
    fd = open(args[2] , O_RDWR );
    write(fd , final_append , ft_strlen(final_append));
    free(final_append);
 }


 pid_t output_redirection_append_mode2(char *cmd_path  , char **args , char **env , int output_file_fd)
 {
    pid_t pid;
    char *argv[1] ; 

    argv[0] = NULL; 

    if (output_file_fd < 0)
    {
        perror("There was an error openning the output file");
        exit(1);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("There was an error creating a child process");
        exit(1);
    }
    if (!pid)
    {
        if (dup2(  output_file_fd   ,    STDOUT_FILENO   ) < 0 )
        {
            perror("There was an error in output redirection");
            exit(1);
        }
        if (execve(cmd_path , argv , env)  < 0)  
        {
            perror("There was an error executing the command after output redirection");
            exit(1) ; 
        }
    }
    return pid;
 }

 void input_redirection_herdoc(char *delimiter  , char **env)
 {

    int fd ;
    int delimiter_len;
    char *buffer;
    int bytes_read; 
    char *argv[1] ; 

    argv[0] = NULL ; 

    fd = open("herdoc" , O_RDWR | O_CREAT , 0644 );
    delimiter_len = ft_strlen(delimiter);
    buffer = malloc(sizeof(char)*delimiter_len  + 1 ) ; 
     while(1)
    {
        bytes_read = read (STDIN_FILENO , buffer , delimiter_len) ;
        buffer[bytes_read] = '\0'; 
        if (!ft_strncmp(buffer, delimiter, ft_strlen(buffer) + delimiter_len )) 
        {
            break;
        }
        write(fd , buffer , ft_strlen(buffer)) ;
    }
    free(buffer) ; 
    fd = open("herdoc" , O_RDWR , 0644 );
    if ( dup2(fd , STDIN_FILENO) < 0 ) 
    {
        perror("There was an error redirecting stdin") ; 
        exit(1);
    }
    pid_t pid ; 
    pid = fork() ; 

    if (!pid)
    {
        if ( execve("/usr/bin/cat" , argv , env) <  0 ) 
        {
            perror("There was an error executing the command ") ; 
            exit (1);
        }
    }
    unlink("herdoc");
 }

int main(int argc , char **argv , char **env)
{
    int fd ;

    /*if (argc < 2 )
    {
        perror("Please give some argument");
        exit(1);
    }*/

    //fd = open(argv[2] , O_RDWR | O_CREAT | O_APPEND , 0644 );

   input_redirection_herdoc("end"  , env );
}