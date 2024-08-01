#include "pipex.h"

static char    *ft_getenv(const char *path, char **envp)
{
    while (*envp)
    {
        if (strncmp(*envp, path, 4) == 0)
            return (*envp  + strlen(path) + 1);
        envp++;
    }
    return (NULL);
}

static char    *find_pathname(char *cmd, char **envp, char *cmd_arg)
{
    char    *path;
    char    **dir_lst;
    char    *abs_dir;
    char    *temp;
    int     i;

    path = ft_getenv("PATH", envp);
    dir_lst = ft_split(path, ':');
    i = 0;
    while (dir_lst[i])
    {
        temp = ft_strjoin(dir_lst[i], "/");
        abs_dir = ft_strjoin(temp, cmd_arg);
        free(temp);
        if (access(abs_dir, X_OK | F_OK) == 0)
        {
            free_list(dir_lst);
            return (abs_dir);
        }
        free(abs_dir);
        i++;
    }
    free_list(dir_lst);
    return (NULL);
}

void execute(char *cmd, char **envp)
{
    char    **cmd_args;
    char    *pathname;

    cmd_args = ft_split(cmd, ' '); // e.g., "ls -l" split to "ls" "-l")
    if (!cmd_args)
        error_exit("ft_split");
    pathname = find_pathname(cmd, envp, cmd_args[0]); // access() is used here so execve unlikely to fail
    if (pathname)
        execve(pathname, cmd_args, envp);
    else
    {
        free_list(cmd_args);
        error_exit("command not found");
    }
}
