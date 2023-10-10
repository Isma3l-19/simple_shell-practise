#include "shell.h"
/**
 * main - function that creates a shell
 * Return: 0 (Success)
 */
int main(void)
{
	char input[MAX_INPUT];
	int status;
	pid_t child_pid;

	while (1)
	{
		printf("simpleshell$ ");
		if (fgets(input, MAX_INPUT, stdin) == NULL)
		{
			printf("\n");
			break;
		}
		input[strcspn(input, "\n")] = '\0';
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (child_pid == 0)
		{
			if (execlp(input, input, NULL) == -1)
			{
				perror("execlp");
				exit(1);
			}
			else
			{
				waitpid(child_pid, &status, 0);
				if (WIFEXITED(status))
				{
					int exit_level = WEXITSTATUS(status);

					if (exit_level != 0)
					{
						fprintf(stderr, "Error: Command exited with status
								level %d\n", exit_level);
					}
					else if ((WIFSIGNALED(status)))
					{
						fprintf(stderr, "Error: Command
								terminated by signal %d\n", WTERMSIG(status));
					}
				}
			}
		}
	}
	return (0);
}
