#include "shell.h"
/**
 * execute_command - function that execute command in child process
 * @input: pointer
 */
void execute_command(char *input)
{
	pid_t child_pid;
	int status;
	char error_message[] = "Error: Command exited to status 0\n";
	char error_message_signal[] = "Error: Command terminated by signal 0\n";
	char error[] = "Error: Command not found\n";

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
		if (execve(input, args, NULL) == -1)
		{
			write(STDERR_FILENO, error, sizeof(error) - 1);
			exit(EXIT_FAILURE);
		}
		free(args);
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
			write(STDERR_FILENO, error_message_signal, sizeof(error_message_signal) - 1);
		}
	}
}
