#include "pipex.h"

void    usage(void)
{
    dprintf(STDERR_FILENO, "Error: Improper arguments!\n");
    printf("Example: ./pipex filein cmd1 cmd2 <...> fileout");
    exit(EXIT_FAILURE);
}

void    pipe_n_exec(char *cmd, char **envp)
{
    int fds[2];
    pid_t   pid;

    if (pipe(fds))
    {
        perror("Pipe error!\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("Fork error!\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        close(fds[R_END]);
        dup2(fds[W_END], STDOUT_FILENO);
    }
    
    // execve(cmd, cmd);
}

int main(int ac, char **av, char **envp)
{
    int i;
    int filein;
    int fileout;

    if (ac <= 4)
        usage();
    filein = open(av[1], O_RDONLY, 0400);
    if (filein == -1)
    {
        perror("Filein cannot be opened!\n");
        exit(EXIT_FAILURE);
    }
    dup2(filein, STDIN_FILENO);
    close(filein);
    fileout = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0755);
    // i = 3;
    // while (ac - 4)
    // {
    //     pipe_n_exec(av[i++], envp);
    //     ac--;
    // }
}
