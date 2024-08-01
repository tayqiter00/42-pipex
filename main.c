#include "pipex.h"

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

int here_doc(char* delimiter)
{
    char    *buffer;
    int     write_fd;
    int     read_fd;

    write_fd = open("here_doc", O_CREAT | O_WRONLY);
    if (write_fd == -1)
        error_exit("write-end here_doc open failed");
    while (1)
    {
        printf("> ");
        buffer = get_next_line(STDIN_FILENO);
        if (!buffer || strcmp(delimiter, buffer) == 0)
        {
            free(buffer);
            break ;
        }
        ft_putstr_fd(buffer, write_fd);
        free(buffer);
    }
    close(write_fd);
    read_fd = open("here_doc", O_RDONLY);
    if (read_fd == -1)
        error_exit("read-end here_doc open failed");
    unlink("here_doc");
    return (read_fd);
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

    fileio_fds[1] = open(fileout, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fileio_fds[1] == -1)
        error_exit("open() failed on outfile");
}

static void set_here_doc_iostream(int fileio_fds[2], char* delimiter, char *fileout)
{
    delimiter = ft_strjoin(delimiter, "\n");
    fileio_fds[0] = here_doc(delimiter);
    free(delimiter);
    if (dup2(fileio_fds[0], STDIN_FILENO) == -1)
        error_exit("stdin not changed to heredoc");
    close(fileio_fds[0]);
    fileio_fds[0] = -1; // set to -1 to indicate it's no longer used
    fileio_fds[1] = open(fileout, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fileio_fds[1] == -1)
        error_exit("open() failed on outfile");
}

int main(int ac, char **av, char **envp)
{
    int i;
    int fileio_fds[2];

    if (ac <= 4)
        usage();
    if (strcmp(av[1], "here_doc") == 0)
    {
        if (ac <= 5)
            usage();
        set_here_doc_iostream(fileio_fds, av[2], av[ac - 1]);
        i = 3;
    }
    else
    {
        set_normal_iostream(fileio_fds, av[1], av[ac - 1]);
        i = 2;
    }
    while (i < ac - 2)
        pipe_n_exec(av[i++], envp);
    dup2(fileio_fds[1], STDOUT_FILENO);
    close(fileio_fds[1]);
    execute(av[i], envp);
    return (0);
}
