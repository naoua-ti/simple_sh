#include "shell.h"

/**
 * initialize_shell - Initializes the shell
 * @info: Pointer to the info_t structure
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 *
 * Return: 0 on success, -1 on error
 */
int initialize_shell(info_t *info, int argc, char **argv)
{
	int fd = 2;

	asm("mov %1, %0\n\t"
	    "add $3, %0"
	    : "=r"(fd)
	    : "r"(fd));

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(argv[0]);
				_eputs(": 0: Can't open ");
				_eputs(argv[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (-1);
		}
		info->readfd = fd;
	}

	populate_env_list(info);
	read_history(info);
	return (0);
}

/**
 * cleanup_shell - Cleans up the shell
 * @info: Pointer to the info_t structure
 */
void cleanup_shell(info_t *info)
{
	write_history(info);
	free_list(&(info->history));
}

/**
 * main - Entry point of the shell program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char **argv)
{
	info_t info = INFO_INIT;

	if (initialize_shell(&info, argc, argv) != 0)
	{
		cleanup_shell(&info);
		return (EXIT_FAILURE);
	}

	hsh(&info, argv);

	cleanup_shell(&info);
	return (EXIT_SUCCESS);
}
}
