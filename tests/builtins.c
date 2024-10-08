#include "../include/minishell.h"

void ft_echo(int line , char *str)
{
    if (line)
    {
        write(STDIN_FILENO , str ,  sizeof(char)*ft_strlen(str)); 
        write(STDIN_FILENO , "\n" , sizeof(char)); 
    }
    else
        write(STDIN_FILENO , str , sizeof(char)*ft_strlen(str) );
}

void ft_env(char **envp)
{
    int i ;
    i = 0 ;

    while(envp[i])
    {
        printf("%s" , envp[i]);
        i++;
    }
}

void ft_cd(char* path)
{
    char *current_path ; 
    struct stat fstats;

    current_path = malloc(sizeof(char)  *  (  ft_strlen (path)  + 1    )   ) ;
    if (stat(path, &fstats) < 0)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (!S_ISDIR(fstats.st_mode))
    {
        printf("Not a directory");
        exit(EXIT_FAILURE);
    }
    if (access(path, X_OK) )
    {
       perror("error");
       exit(EXIT_FAILURE);
    } 
    chdir(path); 
    getcwd(current_path , ft_strlen (path)  + 1); 
    printf("%s" , current_path ); 
    free(current_path);
}

int main(int argc , char **argv)
{
    ft_cd(argv[1]);
}

