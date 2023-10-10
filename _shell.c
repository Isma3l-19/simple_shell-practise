#include "shell.h"
/**
 * main - Entry point for the simple shell program.
 * Return: (0).
 */
int main(void)
{
	char *input = NULL, prompt[] = "simple_shell$ ";
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);
		read = getline(&input, &len, stdin);

		if (read == -1)
		{
			if (feof(stdin))
			{
				write(STDOUT_FILENO, "\n", 1);
				break;
			}
			else
			{
				char error[] = "Error: getline/n";

				write(STDERR_FILENO, error, sizeof(error) - 1);
				exit(EXIT_FAILURE);
			}
		}
		if (input[read - 1] == '\n')
		{
			input[read - 1] = '\0';
		}
	}
	free(input);
	return (0);
}
