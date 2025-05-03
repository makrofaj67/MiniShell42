/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 22:10:15 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 22:02:49 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>  /* For PATH_MAX */

/**
 * @brief Colors and formatting for the prompt
 */
#define COLOR_RESET   "\001\033[0m\002"
#define COLOR_BOLD    "\001\033[1m\002"
#define COLOR_RED     "\001\033[31m\002"
#define COLOR_GREEN   "\001\033[32m\002"
#define COLOR_YELLOW  "\001\033[33m\002"
#define COLOR_BLUE    "\001\033[34m\002"
#define COLOR_MAGENTA "\001\033[35m\002"
#define COLOR_CYAN    "\001\033[36m\002"

/**
 * @brief Get current git branch name if in a git repository
 * 
 * @return char* Branch name or NULL if not in a git repository
 */
static char *get_git_branch(void)
{
    int     fd[2];
    pid_t   pid;
    char    *branch = NULL;
    char    buffer[128];
    int     nbytes;
    int     status;

    if (pipe(fd) < 0)
        return (NULL);
    
    pid = fork();
    if (pid < 0)
    {
        close(fd[0]);
        close(fd[1]);
        return (NULL);
    }
    else if (pid == 0)
    {
        // Child process - Redirect stdout to pipe and exec git command
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        
        // Redirect stderr to /dev/null
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0)
        {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }
        
        // Execute git command
        execlp("git", "git", "branch", "--show-current", NULL);
        exit(1);
    }
    
    // Parent process
    close(fd[1]);
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        nbytes = read(fd[0], buffer, sizeof(buffer) - 1);
        if (nbytes > 0)
        {
            buffer[nbytes] = '\0';
            // Remove trailing newline
            if (buffer[nbytes - 1] == '\n')
                buffer[nbytes - 1] = '\0';
            branch = strdup(buffer);
        }
    }
    
    close(fd[0]);
    return (branch);
}

/**
 * @brief Check if current directory is a git repository
 * 
 * @return int 1 if in git repo, 0 otherwise
 */
static int is_git_repository(void)
{
    char *branch;
    int  result;
    
    branch = get_git_branch();
    result = (branch != NULL);
    free(branch);
    return (result);
}

/**
 * @brief Get the current username from environment
 * 
 * @param envp Environment variables
 * @return char* Username or "user" if not found
 */
static char *get_username(char **envp)
{
    int i;
    char *username = NULL;
    
    i = 0;
    while (envp[i])
    {
        if (strncmp(envp[i], "USER=", 5) == 0)
        {
            username = envp[i] + 5;
            break;
        }
        else if (strncmp(envp[i], "LOGNAME=", 8) == 0)
        {
            username = envp[i] + 8;
            break;
        }
        i++;
    }
    
    if (!username)
        username = "user";
    
    return (username);
}

/**
 * @brief Get hostname (computer name)
 * 
 * @return char* Hostname or "shell" if not found
 */
static char *get_hostname(void)
{
    static char hostname[128];
    int fd;
    int nbytes;
    
    // Try to read hostname from /proc/sys/kernel/hostname
    fd = open("/proc/sys/kernel/hostname", O_RDONLY);
    if (fd >= 0)
    {
        nbytes = read(fd, hostname, sizeof(hostname) - 1);
        close(fd);
        
        if (nbytes > 0)
        {
            hostname[nbytes] = '\0';
            // Remove trailing newline
            if (hostname[nbytes - 1] == '\n')
                hostname[nbytes - 1] = '\0';
            return (hostname);
        }
    }
    
    return ("shell");
}

/**
 * @brief Get shortened current working directory
 *        Shows only the last two directory components for easier reading
 * 
 * @return char* Shortened path
 */
static char *get_shortened_cwd(void)
{
    char cwd[PATH_MAX];
    char *shortened;
    char *last_part;
    char *second_last_part;
    int  count;
    
    // Get current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return (strdup("~"));
    
    // Check if we're in the home directory
    char *home_dir = getenv("HOME");
    if (home_dir && strncmp(cwd, home_dir, strlen(home_dir)) == 0)
    {
        // Replace home directory part with ~
        sprintf(cwd, "~%s", cwd + strlen(home_dir));
    }
    
    // Count components and find last parts
    count = 0;
    last_part = NULL;
    second_last_part = NULL;
    
    for (char *p = cwd; *p; p++)
    {
        if (*p == '/')
        {
            count++;
            if (count >= 2)
                second_last_part = last_part;
            last_part = p;
        }
    }
    
    if (count <= 2 || !second_last_part)
        return (strdup(cwd));
    
    // Create shortened path
    shortened = (char *)malloc(strlen(second_last_part) + 5);
    if (!shortened)
        return (strdup(cwd));
    
    sprintf(shortened, "...%s", second_last_part);
    return (shortened);
}

/**
 * @brief Prepare a colorful prompt with useful info
 * 
 * Format: 
 * [username@hostname current_dir git_branch exit_status]
 * $ 
 * 
 * @param envp Environment variables
 * @param exit_status Last command exit status
 * @return char* Formatted prompt string
 */
char *prepare_fancy_prompt(char **envp, int exit_status)
{
    char *prompt;
    char *username;
    char *hostname;
    char *cwd;
    char *git_branch;
    char *git_indicator;
    char *exit_indicator;
    
    username = get_username(envp);
    hostname = get_hostname();
    cwd = get_shortened_cwd();
    git_branch = get_git_branch();
    
    // Prepare git branch indicator if in a repository
    if (git_branch)
    {
        git_indicator = (char *)malloc(strlen(git_branch) + 20);
        if (git_indicator)
            sprintf(git_indicator, " %s(%s)%s", COLOR_CYAN, git_branch, COLOR_RESET);
        else
            git_indicator = strdup("");
    }
    else
    {
        git_indicator = strdup("");
    }
    
    // Show exit status if it's non-zero
    if (exit_status != 0)
    {
        exit_indicator = (char *)malloc(20);
        if (exit_indicator)
            sprintf(exit_indicator, " %s[%d]%s", COLOR_RED, exit_status, COLOR_RESET);
        else
            exit_indicator = strdup("");
    }
    else
    {
        exit_indicator = strdup("");
    }
    
    // Assemble the final prompt with $ on the second line
    prompt = (char *)malloc(strlen(username) + strlen(hostname) + 
                           strlen(cwd) + strlen(git_indicator) + 
                           strlen(exit_indicator) + 120);
    
    if (prompt)
    {
        sprintf(prompt, "%s[%s%s@%s%s %s%s%s]%s%s%s\n%s$ %s", 
                COLOR_BOLD,
                COLOR_GREEN, username, hostname, COLOR_RESET,
                COLOR_BLUE, cwd, COLOR_RESET,
                COLOR_BOLD, git_indicator, exit_indicator,
                COLOR_YELLOW, COLOR_RESET);
    }
    
    free(cwd);
    free(git_branch);
    free(git_indicator);
    free(exit_indicator);
    
    return prompt;
}