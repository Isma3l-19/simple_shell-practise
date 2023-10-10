#include "shell.h"
/**
 * main - Entry point for the simple shell program.
 * It reads user input, executes single-word commands, and displays
 * the results.
 * Return: (0).
 */
int main(void)
{
	char *input = NULL, prompt[] = "simple_shell$ ", error[] = "Error: execve\n";
	size_t len = 0;
	ssize_t read;
	pid_t child_pid;
	int status;
	char error_message[] = "Error: Command exited with status 0\n";
	char error_message_signal[] = "Error: Command terminated by signal 0\n";

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
		child_pid = fork();
		if (child_pid == -1)
		{
			char error[] = "Error: fork\n";

			write(STDERR_FILENO, error, sizeof(error) - 1);
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			char **args = (char **)malloc(2 * sizeof(char *));
			args[0] = input;
			args[1] = NULL;
			execve(input, args, NULL);
			free(args);
			write(STDERR_FILENO, error, sizeof(error) - 1);
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(child_pid, &status, 0);
			if (WIFEXITED(status))
			{
				int exit_status = WEXITSTATUS(status);

				if (exit_status != 0)
				{
					error_message[32] = '0' + exit_status % 10;
					error_message[31] = '0' + (exit_status / 10) % 10;
					write(STDERR_FILENO, error_message, sizeof(error_message) - 1);
				}
			}
			else if (WIFSIGNALED(status))
			{
				int signal_code = WTERMSIG(status);

				error_message_signal[30] = '0' + signal_code % 10;
				write(STDERR_FILENO, error_message, sizeof(error_message_signal) - 1);
			}
		}
	}
	free(input);
	return (0);
}

