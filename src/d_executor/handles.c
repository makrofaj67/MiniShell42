/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 07:20:38 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/14 07:20:38 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include "../../inc/d_executor.h"


void heredoc_helper(t_redirection *redirections)
{
    int fd;
    char *line;
    
    fd = open(redirections->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return;
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || mini_strcmp_path(line, redirections->filename) == 1)
            break;
        write(fd, line, mini_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);    
}

int fd_redirection_helper(int type, int fd)
{
    if (fd == -1)
        return -1;
    if (type == REDIR_IN || type == REDIR_HEREDOC)
        dup2(fd, STDIN_FILENO);
    else if (type == REDIR_OUT || type == REDIR_APPEND)
        dup2(fd, STDOUT_FILENO);
    else
        return -1;
    close(fd);
    return 0;
}

void handle_redirections(t_redirection **redirections)
{
    int fd;
    int i;

    i = 0;
    while (redirections && redirections[i])
    {
        if (redirections[i]->type == REDIR_IN)
            if (access(redirections[i]->filename, R_OK) == 0)
                fd = open(redirections[i]->filename, O_RDONLY);
            else
                exit(1);
        else if (redirections[i]->type == REDIR_OUT)
            fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (redirections[i]->type == REDIR_APPEND)
            fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (redirections[i]->type == REDIR_HEREDOC)
            fd = open(redirections[i]->filename, O_RDONLY);
        else
            exit(1);
        if (fd != -1)
            if (fd_redirection_helper(redirections[i]->type, fd) == -1)
                exit(1);
        i++;
    }
}

// void heredoc_helper(t_redirection **redirections)
// {
//     int fd;
//     char *line;
    
//     fd = open(redirections[0]->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//         return;
//     while (1)
//     {
//         line = readline("> ");
//         if (!line || mini_strcmp_path(line, redirections[0]->filename) == 1)
//             break;
//         write(fd, line, mini_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//     }
//     free(line);
//     close(fd);    
//     fd = open(redirections[0]->filename, O_RDONLY);
//     if (fd == -1)
//         return;
//     dup2(fd, STDIN_FILENO);
//     close(fd);

// }

// int fd_redirection_helper(int type, int fd)
// {
//     if (fd == -1)
//         return -1;
//     if (type == REDIR_IN)
//         dup2(fd, STDIN_FILENO);
//     else if (type == REDIR_OUT || type == REDIR_APPEND)
//         dup2(fd, STDOUT_FILENO);
//     else if (type == REDIR_HEREDOC)
//         return (3);
//     else
//         return -1;
//     close(fd);
//     return 0;
// }

// int handle_redirections(t_redirection **redirections)
// {
//     int fd;
//     int i;

//     i = 0;
//     while (redirections && redirections[i])
//     {
//         if (redirections[i]->type == REDIR_IN)
//             if (access(redirections[i]->filename, R_OK) == 0)
//                 fd = open(redirections[i]->filename, O_RDONLY);
//             else
//                 exit(1);
//         else if (redirections[i]->type == REDIR_OUT)
//             fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         else if (redirections[i]->type == REDIR_APPEND)
//             fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         else if (redirections[i]->type == REDIR_HEREDOC)
//             heredoc_helper(&redirections[i]);
//         else
//             return -1;
//         if (fd != -1)
//             if (fd_redirection_helper(redirections[i]->type, fd) == -1)
//                 return -1;
//         i++;
//     }
//     return 0;
// }

// int handle_redirections(t_redirection **redirections)
// {
//     int fd;

//     fd = -1;
//     for (int i = 0; redirections && redirections[i]; i++)
//     {
//         if (redirections[i]->type == REDIR_IN)
//         {
//             fd = open(redirections[i]->filename, O_RDONLY);
//             if (fd == -1)
//             {
//                 perror("minishell: open");
//                 return -1;
//             }
//             printf("%d\n", fd);
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//         }
//         else if (redirections[i]->type == REDIR_OUT)
//         {
//             fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd == -1)
//             {
//                 perror("minishell: open");
//                 return -1;
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (redirections[i]->type == REDIR_APPEND)
//         {
//             fd = open(redirections[i]->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd == -1)
//             {
//                 perror("minishell: open");
//                 return -1;
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//     }
//     return 0;
// }
