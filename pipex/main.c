#include "cell.h"

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

static char	*try_paths(char *cmd, char **paths)
{
	int		i;
	char	*tmp;

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

	//burada her dizine gidip dizin sonuna cmd ekliyoruz ve access ile buna bakıyoruz
	//var mı yok mu diye, varsa dizin + / + komutu döndürüyoruz, bu da full_path oluyor.
}

char	*get_full_path(char *cmd, char **envp)
{
	char	*path_str;
	char	**paths;
	char	*result;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/')) //burada cmd tam ya da göreceli yolla 
	//verilmiş mi ona bakıyoruz.
	{
		if (access(cmd, X_OK) == 0) //access ile bakıyoruz bu çalıştırılabilir mi.
			return (strdup(cmd)); //çalıştırılabilirse direkt olarak bunu full_path olarak 
		//döndürüyoruz.
		return (NULL);
	}
	path_str = getenv("PATH"); //eğer bu şekil değilse path stringini işlememiz gerekecek.
	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':'); //burada path içindeki her dizini ayrı ayrı paths** içine
	//atıyoruz.
	if (!paths)
		return (NULL);
	result = try_paths(cmd, paths); //burada access ile dizinlerin içinde komutu deniyoruz çalı
	//şuyor mu diye. çalışıyorsa full_pathi döndürüyoruz.
	ft_free_array(paths);
	return (result);
	
	//burada pathteki bütün dizinlerin içerisinde cmd dosyası var mı diye bakıyorum,
	//bulduğum ilkini full_path olarak geri döndürüyorum. yoksa null döndürüyorum.
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
	fullpath = get_full_path(splitted_tokens[0], envp); //burada splitted tokenstaki
	//ilk argümanı yani cmdyi path içerisinde arıyorum, varsa full pathi' geri döndürüyorum.
	if (!fullpath)
	{
		ft_free_array(splitted_tokens);
		return (NULL);
	}
	update_splitted_tokens(splitted_tokens, fullpath); //fullpath varsa, splitted_tokensda
	//ilk argümanı full path'i ile güncelliyorum. 
	//yani 
	//splitted_tokens = { "/usr/bin/ls", "-la", NULL } oluyor.
	return (splitted_tokens);
}

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
	//burada malloclanmış çift pointera argları tek tek yüklüyorum
	//splitted tokens = { "ls", "-la", NULL } gibi bir şey oluyor.
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
	//boşluklar arasında gerçekten kaç tane token var hesaplaması.
}

char	**get_cmd_and_args(char *cmd_string, char **envp)
{
	int		token_count;
	char	**splitted_tokens;
	char	**ready_for_execve;

	token_count = get_token_count(cmd_string); //burda token countu sayıyorum, 
	//aslında saymadan da olur ancak linkedlist gerekiyor, bunu sonradan değiştiririz.
	if (token_count == 0)
		return (NULL);
	splitted_tokens = (char **)malloc(sizeof(char *) * (token_count + 1)); //burada tırtıklanmış
	//arglar için malloc yapıyom
	if (!splitted_tokens)
		return (NULL);
	splitted_tokens = serve_array_of_tokens(cmd_string, splitted_tokens); //argları malloclanmış
	//arraya yüklüyorum.
	if (!splitted_tokens)
		return (NULL);
	ready_for_execve = prepare_for_execve(splitted_tokens, envp); //en sonunda execve için
	//eğer varsa ilk komutun tam pathini buluyorun ve ilk argümanı güncellliyorum.
	return (ready_for_execve);
}

void	error_exit(char *message)
{
	perror(message); //en son oluşan syscall hatasını ya da direkt olarak istenen
	//mesajı stderr'e yazar. syscall başarısız olduğunda kernel tarafından set edilen
	//errno ya bakar ve buna uygun mesajı yazar. perror eğer NULL geçirilirse parametre olarak
	//sadece errno'ya ait mesajı yazar. yoksa message: syscall hata mesajı yazdırır. errno her
	//process için özeldir ve kernel tarafından özel olarak syscall hatalarında güncellenir.
	exit(EXIT_FAILURE); //programın sonlandırılması için kullanılır. bu syscall çağrıldığıında
	//programın işletilmesi o anda durdurulur. ayrıca exit_failure'u shell içerisinden $? yazarak
	//görebiliriz. işlem hatayla bittikten hemen sonra bakılması gerekir. exit ne yapar?
	// **sistemdeki kernel tarafından yürütülen temizleme işlemlerini çağırır.
	// **i/o bufferlarını boşaltır.
	// **açık fdler varsa kapatır.
	// **başka kaynak varsa serbest bırakır.
	// **processi bitirir.
}

void	process_second_child(char *outfile, char *cmd2, int fd[2], char **env)
{
	int		fdout;
	char	**cmdprocessed;
	// Temel argümanların geçerliliğini kontrol et. Hata durumunda çıkar.
	if (!outfile || !cmd2 || !env)
		error_exit("Invalid arguments");
	// Bu child process sadece pipe'tan okuyacak (stdin'i pipe'ın okuma ucuna yönlendirilerek).
	// Pipe'ın yazma ucunu (fd[1]) kapat. Bu child onu kullanmayacak.
	close(fd[1]);
	// Standard girdiyi (STDIN_FILENO = 0), pipe'ın okuma ucuna (fd[0]) yönlendir.
	// Bu, dup2 ile 0 numaralı FD'nin pipefd[0]'ın işaret ettiği
	// File Table Entry'ye işaret etmesini sağlar.
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_exit("dup2 failed on stdin for pipe");
	// Yönlendirme yapıldığı için pipe'ın orijinal okuma ucunu (fd[0]) kapat.
	// Artık 0 numaralı FD pipe'a işaret ediyor.
	close(fd[0]);
	// Çıktı dosyasını (outfile) yazma, oluşturma ve içini boşaltma modunda aç.
	// 0644 izinleri (kullanıcı oku/yaz, grup oku, diğerleri oku) ile oluştur.
	// Shell'deki '>' redirection'ının davranışıdır.
	fdout = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (fdout == -1)
		error_exit("Failed to open output file"); // Hata mesajını iyileştir
	// Standard çıkışı (STDOUT_FILENO = 1), açılan çıktı dosyasının FD'sine (fdout) yönlendir.
	// Bu, dup2 ile 1 numaralı FD'nin fdout'ın işaret ettiği
	// File Table Entry'ye işaret etmesini sağlar.
	if (dup2(fdout, STDOUT_FILENO) == -1)
		error_exit("dup2 failed on stdout for outfile");
	// Yönlendirme yapıldığı için orijinal çıktı dosyası FD'sini (fdout) kapat.
	// Artık 1 numaralı FD çıktı dosyasına işaret ediyor.
	close(fdout);
	// Komut stringini argümanlarına ayır, PATH'te yolunu bul ve execve için hazırla.
	cmdprocessed = get_cmd_and_args(cmd2, env);
	if (!cmdprocessed)
		error_exit("Command not found or invalid"); 
	// Komutu çalıştır (execve).
	// execve, process'in kodunu cmd2 programıyla değiştirir.
	// Bu noktadan itibaren, çalışacak olan cmd2 programıdır.
	// cmd2 programı standard girdisinden okuyacak (STDIN_FILENO = 0) -> pipe'tan okur.
	// cmd2 programı standard çıktısına yazacak (STDOUT_FILENO = 1) -> çıktı dosyasına yazar.
	execve(cmdprocessed[0], cmdprocessed, env);
	// Eğer execve başarılı olursa bu satırlar ASLA çalışmaz.
	// execve sadece hata durumunda geri döner.
	// execve başarısız olursa:
	// cmdprocessed için malloc kullandıysan freele.
	ft_free_array(cmdprocessed);
	// execve başarısızlığını belirten hata mesajı yazdır ve child process'ten çık.
	// perror zaten execve hatasini set etmistir.
	error_exit("execve failed");
}

void	process_first_child(char *infile, char *cmd1, int fd[2], char **env)
{
	int		fdin;
	char	**cmdprocessed;
	// Temel argümanların geçerliliğini kontrol et (NULL olmamalılar). Hata durumunda çıkar.
	if (!infile || !cmd1 || !env)
		error_exit("Invalid arguments"); 
	// Bu child process sadece pipe'a yazacak (stdout'u pipe'ın yazma ucuna yönlendirilerek).
	// Pipe'ın okuma ucunu (fd[0]) kapat. Bu child onu kullanmayacak.
	close(fd[0]);
	// Standard çıkışı (STDOUT_FILENO = 1), pipe'ın yazma ucuna (fd[1]) yönlendir.
	// Bu, dup2 ile 1 numaralı FD'nin pipefd[1]'in işaret ettiği
	// File Table Entry'ye işaret etmesini sağlar.
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 failed on stdout for pipe");
	// Yönlendirme yapıldığı için pipe'ın orijinal yazma ucunu (fd[1]) kapat.
	// Artık 1 numaralı FD pipe'a işaret ediyor.
	close(fd[1]);
	// Girdi dosyasını (infile) sadece okuma modunda aç.
	// Amaç: standard girdi yerine infile'dan okuma yapmak.
	fdin = open(infile, O_RDONLY);
	if (fdin == -1)
		error_exit("Failed to open input file");
	// Standard girdiyi (STDIN_FILENO = 0), açılan girdi dosyasının FD'sine (fdin) yönlendir.
	// Bu, dup2 ile 0 numaralı FD'nin fdin'in işaret ettiği
	// File Table Entry'ye işaret etmesini sağlar.
	if (dup2(fdin, STDIN_FILENO) == -1)
		error_exit("dup2 failed on stdin for infile");
	// Yönlendirme yapıldığı için orijinal girdi dosyası FD'sini (fdin) kapat.
	// Artık 0 numaralı FD girdi dosyasına işaret ediyor.
	close(fdin);
	// Komut stringini argümanlarına ayır, PATH'te yolunu bul ve execve için hazırla.
	cmdprocessed = get_cmd_and_args(cmd1, env);
	if (!cmdprocessed)
		error_exit("Command not found or invalid");
	// Komutu çalıştır (execve).
	// execve, process'in kodunu cmd1 programıyla değiştirir.
	// Bu noktadan itibaren, çalışacak olan cmd1 programıdır.
	// cmd1 programı standard girdisinden okuyacak (STDIN_FILENO = 0) -> infile'dan okur.
	// cmd1 programı standard çıktısına yazacak (STDOUT_FILENO = 1) -> pipe'ın yazma ucuna yazar.
	execve(cmdprocessed[0], cmdprocessed, env);
	// Eğer execve başarılı olursa bu satırlar ASLA çalışmaz.
	// execve sadece hata durumunda (komut bulunamadı, izin yok vb.) geri döner.
	// execve başarısız olursa:
	// cmdprocessed için malloc kullandıysan freele.
	ft_free_array(cmdprocessed);
	// execve başarısızlığını belirten hata mesajı yazdır ve child process'ten çık.
	// perror zaten execve hatasini set etmistir.
	error_exit("execve failed");
}

int	main(int argc, char **argv, char **env)
{
	int		pipefd[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
	if (pipe(pipefd) == -1) //burada pipe açılır, aynı anda return değeri kontrol edilir.
		//-1 ise error_exit ile programdan çıkılır.
		error_exit("Pipe creation failed"); //error_exit ile hem error mesajı veriyoruz
	//hem de programdan olması gerektiği gibi çıkıyoruz.
	pid1 = fork(); //cmd1 için fork yapılır
	if (pid1 < 0) //fork hatasından çıkış.
		error_exit("Fork failed");
	if (pid1 == 0) //birinci childda cmd1 işleri yapılır.
		process_first_child(argv[1], argv[2], pipefd, env); //bu satır sonrası child process
	//sonlanmıştır. şu an sadece yine parent kalır.
	pid2 = fork(); //ikinci child cmd2 için açılır.
	if (pid2 < 0)
		error_exit("Fork failed");
	if (pid2 == 0)
		process_second_child(argv[4], argv[3], pipefd, env);
	//birinci ve ikinci childlar işlenirken aynı zamandan pipe uçlarında da işlemler yapılır.
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0); //ana process pid1 ve pid2 yani childların processlerinin bitmesini bekler.
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}
