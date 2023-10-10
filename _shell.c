#include "shell.h"
/**
 * main - Entry point for the simple shell program.
 *
 * This function implements a basic UNIX command line interpreter.
 * It reads user input, executes single-word commands, and displays
 * the results.
 *
 * Return: Always returns (0).
 */
int main(void)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t child_pid;
	int status;

	while (1)
	{
		printf("$ ");
		read = getline(&input, &len, stdin);
		if (read == -1)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			else
			{
				perror("getline");
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
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			char **args = malloc(sizeof(char *) * 2);

			if (args == NULL)
			{
				perror("mallo");
				exit(EXIT_FAILURE);
			}
			args[0] = input;
			args[1] = NULL;

			execve(input, args, NULL);
		}
		else
		{
			waitpid(child_pid, &status, 0);
			if (WIFEXITED(status))
			{
				int exit_status = WEXITSTATUS(status);

				if (exit_status != 0)
				{
					fprintf(stderr, "Error: Command exited with status %d\n", exit_status);
				}
			}
			else if (WIFSIGNALED(status))
			{
				fprintf(stderr, "Error: Command terminated by signal %d\n",
						WTERMSIG(status));
			}
		}
	}
	free(input);
	return (0);
}

