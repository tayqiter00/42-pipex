#include "pipex.h"

void    usage(void)
{
    dprintf(STDERR_FILENO, "Error: Improper arguments!\n");
    printf("Example: ./pipex filein cmd1 cmd2 <...> fileout\n");
    exit(EXIT_FAILURE);
}

// void    pipe_n_exec(char *cmd, char **envp)
// {
//     int fds[2];
//     pid_t   pid;

//     if (pipe(fds))
//     {
//         perror("Pipe error!\n");
//         exit(EXIT_FAILURE);
//     }
//     pid = fork();
//     if (pid == -1)
//     {
//         perror("Fork error!\n");
//         exit(EXIT_FAILURE);
//     }
//     if (pid == 0)
//     {
//         close(fds[R_END]);
//         dup2(fds[W_END], STDOUT_FILENO);
//         close(fds[W_END]);
//         execute();
//     }
//     else
//     {
//         waitpid(-1, NULL, 0);
//         close(fds[W_END]);
//     }
    
//     // execve(cmd, cmd);
// }

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
    // if (strncmp(av[1], "here_doc", strlen("here_doc")) == 0)
    // {

    // }
    // else
        set_normal_iostream(fileio_fds, av[1], av[ac - 1]);
    // while (ac - 4)
    // {
    //     pipe_n_exec(av[i++], envp);
    //     ac--;
    // }
}
