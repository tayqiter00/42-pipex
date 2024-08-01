#include "pipex.h"

void    usage(void)
{
    dprintf(STDERR_FILENO, "Error: Improper arguments!\n");
    printf("Example: ./pipex filein cmd1 cmd2 <...> fileout\n");
    exit(EXIT_FAILURE);
}

void execute(char *cmd, char **envp)
{
    char    **cmd_no_opts;
    char    *pathname;

    cmd_no_opts = ft_split(cmd, ' ');
    if (!cmd_no_opts)
        error_exit("split");
    pathname = find_pathname(cmd, envp, cmd_no_opts[0]);
    if (pathname)
    {
        execve(pathname, cmd_no_opts, envp);
    }
    dprintf(STDERR_FILENO, "command not found\n");
    // free_list();
}

void    pipe_n_exec(char *cmd, char **envp)
{
    int pipefds[2];
    pid_t   pid;

    if (pipe(pipefds))
        error_exit("pipe failed");
    pid = fork();
    if (pid == -1)
        error_exit("fork failed");
    if (pid == 0)
    {
        close(pipefds[R_END]);
        dup2(pipefds[W_END], STDOUT_FILENO);
        close(pipefds[W_END]);
        execute(cmd, envp);
    }
    else
    {
        waitpid(-1, NULL, 0);
        close(pipefds[W_END]);
        dup2(pipefds[R_END], STDIN_FILENO);
        close(pipefds[R_END]);
    }
}

static void set_normal_iostream(int fileio_fds[2], char *filein, char *fileout)
{
    fileio_fds[0] = open(filein, O_RDONLY, 0400);
    if (fileio_fds[0] == -1)
        error_exit("open() failed on infile");
    if (dup2(fileio_fds[0], STDIN_FILENO) == -1)
        error_exit("stdin not changed to infile");
    close(fileio_fds[0]);
    fileio_fds[0] = -1; // set to -1 to indicate it's no longer used
    fileio_fds[1] = open(fileout, O_CREAT | O_WRONLY | O_TRUNC, 0755); // can probably be set later too
    if (fileio_fds[1] == -1)
        error_exit("open() failed on outfile");
}

int main(int ac, char **av, char **envp)
{
    int i;
    int fileio_fds[2]; // fd_filein and fd_fileout

    if (ac <= 4)
        usage();
    if (strncmp(av[1], "here_doc", strlen("here_doc")) == 0)
    {
        i = 3;
    }
    else
    {
        set_normal_iostream(fileio_fds, av[1], av[ac - 1]);
        i = 2;
    }
    while (i < ac - 2)
    {
        pipe_n_exec(av[i++], envp);
    }
    dup2(fileio_fds[1], STDOUT_FILENO);
    close(fileio_fds[1]);
    execute(av[i], envp);
    return (0);
}
