#include "pipex.h"

void    usage(void)
{
    dprintf(STDERR_FILENO, "Error: Improper arguments!\n");
    printf("Example: ./pipex filein cmd1 cmd2 <...> fileout\n");
    printf("Example: ./pipex here_doc LIMITER cmd1 cmd2 fileout\n");
    exit(EXIT_FAILURE);
}

void    error_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


void    free_list(char **list)
{
    int i;

    i = 0;
    while (list[i])
        free(list[i++]);
    free(list);
}
