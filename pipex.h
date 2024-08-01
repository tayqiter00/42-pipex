#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stddef.h>
#include <sys/wait.h>

# define R_END 0
# define W_END 1

void    error_exit(char *msg);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char    *find_pathname(char *cmd, char **envp, char *cmd_no_opts);

#endif