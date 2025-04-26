#include "cell.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// Yardımcı fonksiyonlar
void	ft_free_array_partial(char **arr, int count)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*ft_substr_split(char const *s, int start, char c)
{
	int		len;
	char	*substr;
	int		i;

	len = 0;
	i = 0;
	while (s[start + len] && s[start + len] != c)
		len++;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	**ft_free_split(char **split, int j)
{
	while (j >= 0)
	{
		free(split[j]);
		j--;
	}
	free(split);
	return (NULL);
}

// Split fonksiyonunu iki parçaya bölüyorum
static int	count_words(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		count;
	int		i;
	int		j;

	count = count_words(s, c);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count)
	{
		while (s[i] == c)
			i++;
		result[j] = ft_substr_split(s, i, c);
		if (!result[j])
			return (ft_free_split(result, j));
		while (s[i] && s[i] != c)
			i++;
		j++;
	}
	result[j] = NULL;
	return (result);
}

static char	*find_path_in_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*try_paths(char *cmd, char **paths)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		tmp = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		if (!tmp)
			return (NULL);
		strcpy(tmp, paths[i]);
		strcat(tmp, "/");
		strcat(tmp, cmd);
		if (access(tmp, X_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*get_full_path(char *cmd, char **envp)
{
	char	*path_str;
	char	**paths;
	char	*result;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	path_str = find_path_in_env(envp);
	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':');
	if (!paths)
		return (NULL);
	result = try_paths(cmd, paths);
	ft_free_array(paths);
	return (result);
}

static void	update_splitted_tokens(char **tokens, char *fullpath)
{
	char	*tmp;

	tmp = tokens[0];
	tokens[0] = fullpath;
	free(tmp);
}

char	**prepare_for_execve(char **splitted_tokens, char **envp)
{
	char	*fullpath;

	if (!splitted_tokens || !splitted_tokens[0])
		return (NULL);
	fullpath = get_full_path(splitted_tokens[0], envp);
	if (!fullpath)
	{
		ft_free_array(splitted_tokens);
		return (NULL);
	}
	update_splitted_tokens(splitted_tokens, fullpath);
	return (splitted_tokens);
}

// Token ayrıştırma işlemini bölerek 25 satır kuralına uyuyoruz
static int	allocate_token(char **tokens, int i, char *start, int len)
{
	tokens[i] = malloc(len + 1);
	if (!tokens[i])
	{
		ft_free_array_partial(tokens, i);
		return (0);
	}
	memcpy(tokens[i], start, len);
	tokens[i][len] = '\0';
	return (1);
}

char	**serve_array_of_tokens(char *cmd, char **splitted_tokens)
{
	char	*pos;
	int		i;
	char	*start;
	int		len;

	pos = cmd;
	i = 0;
	while (*pos)
	{
		while (*pos && isspace((unsigned char)*pos))
			pos++;
		if (!*pos)
			break ;
		start = pos;
		while (*pos && !isspace((unsigned char)*pos))
			pos++;
		len = pos - start;
		if (!allocate_token(splitted_tokens, i, start, len))
			return (NULL);
		i++;
	}
	splitted_tokens[i] = NULL;
	return (splitted_tokens);
}

int	get_token_count(char *cmd)
{
	char	*ptr;
	int		token_count;
	int		is_in_token;

	token_count = 0;
	is_in_token = 0;
	if (cmd == NULL)
		return (0);
	ptr = cmd;
	while (*ptr != '\0' && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	while (*ptr != '\0')
	{
		if (*ptr == ' ' || *ptr == '\t')
			is_in_token = 0;
		else if (is_in_token == 0)
		{
			token_count++;
			is_in_token = 1;
		}
		ptr++;
	}
	return (token_count);
}

char	**get_cmd_and_args(char *cmd_string, char **envp)
{
	int		token_count;
	char	**splitted_tokens;
	char	**ready_for_execve;

	token_count = get_token_count(cmd_string);
	if (token_count == 0)
		return (NULL);
	// 1) kaç adet boşluklar arasında var olan token var say
	// 2) bir adet char ** mallocla, token sayısı kadar sizeı olsun
	splitted_tokens = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!splitted_tokens)
		return (NULL);
	splitted_tokens = serve_array_of_tokens(cmd_string, splitted_tokens);
	if (!splitted_tokens)
		return (NULL);
	// Burası linked listle yapılabilir ama şimdilik böyle geçiyorum
	ready_for_execve = prepare_for_execve(splitted_tokens, envp);
	return (ready_for_execve);
}

int	main(int argc, char **argv, char **envp)
{
	char	**cmd_args;

	if (argc < 2)
		return (1);
	cmd_args = get_cmd_and_args(argv[1], envp);
	if (cmd_args)
	{
		// Burada execve() kullanabilirsiniz
		// execve(cmd_args[0], cmd_args, envp);
		ft_free_array(cmd_args);
	}
	return (0);
}
