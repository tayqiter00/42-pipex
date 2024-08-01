#include "pipex.h"

void    error_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

char    *ft_getenv(const char *path, char **envp)
{
    while (*envp)
    {
        if (strncmp(*envp, path, 4) == 0)
            return (*envp  + strlen(path) + 1);
        envp++;
    }
    return (NULL);
}

void    free_list(char **list)
{
    int i;

    i = 0;
    while (list[i])
        free(list[i++]);
    free(list);
}

char    *find_pathname(char *cmd, char **envp, char *cmd_no_opts)
{
    char    *path;
    char    **dir_lst;
    char    *abs_dir;
    char    *temp;

    path = ft_getenv("PATH", envp);
    // if (!path)
    //     error_exit("PATH not found"); // free list
    dir_lst = ft_split(path, ':');
    // if (!dir_lst)
    //     error_exit("split directory list failed"); // free list
    while (*dir_lst)
    {
        temp = ft_strjoin(*dir_lst, "/");
        abs_dir = ft_strjoin(temp, cmd_no_opts);
        free(temp);
        if (access(abs_dir, X_OK) == 0)
            return (abs_dir);
        free(abs_dir);
        dir_lst++;
    }
    return (NULL);
}