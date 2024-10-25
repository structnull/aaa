#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLES 100
#define MAX_VAR_NAME 20

typedef struct
{
	char name[MAX_VAR_NAME];
	int value;
} Variable;

Variable variables[MAX_VARIABLES];
int varCount = 0;

int get_variable_value(const char *name)
{
	for (int i = 0; i < varCount; i++)
	{
		if (strcmp(variables[i].name, name) == 0)
		{
			return variables[i].value;
		}
	}
	return 0;
}

void set_variable(const char *name, int value)
{
	for (int i = 0; i < varCount; i++)
	{
		if (strcmp(variables[i].name, name) == 0)
		{
			variables[i].value = value;
			return;
		}
	}
	strncpy(variables[varCount].name, name, MAX_VAR_NAME);
	variables[varCount].value = value;
	varCount++;
}

// Function to print value or variable
void execute_print(const char *arg)
{
	if (arg[0] >= '0' && arg[0] <= '9')
	{
		printf("%d\n", atoi(arg));
	}
	else
	{
		printf("%d\n", get_variable_value(arg));
	}
}

// Function to execute "anj" (declaration)
void execute_declaration(const char *var_name)
{
	set_variable(var_name, 0);
}

// Function to execute "ma" (assignment)
void execute_assignment(const char *var_name, int value)
{
	set_variable(var_name, value);
}

// Increment variable by 1
void execute_increment(const char *var_name)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current + 1);
}

// Decrement variable by 1
void execute_decrement(const char *var_name)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current - 1);
}

// Multiply variable by a value
void execute_multiply(const char *var_name, int value)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current * value);
}

// Divide variable by a value
void execute_divide(const char *var_name, int value)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current / value);
}

// Conditional (if) statement ("adharsh")
void execute_if(const char *var_name, int expected_value, const char *command, const char *arg)
{
	if (get_variable_value(var_name) == expected_value)
	{
		if (strcmp(command, "anjima") == 0)
		{
			execute_print(arg);
		}
	}
}

// Interpreter to parse each line
void interpreter(const char *line)
{
	char command[20], arg1[20], arg2[20];
	int value;

	if (strstr(line, "anj ") == line)
	{
		if (sscanf(line, "%s %s", command, arg1) == 2)
		{
			execute_declaration(arg1);
		}
	}
	else if (strstr(line, "ma ") == line)
	{
		if (sscanf(line, "%s %s %d", command, arg1, &value) == 3)
		{
			execute_assignment(arg1, value);
		}
	}
	else if (strstr(line, "anjima ") == line)
	{
		if (sscanf(line, "%s %s", command, arg1) == 2)
		{
			execute_print(arg1);
		}
	}
	else if (strstr(line, "++") == line)
	{
		if (sscanf(line, "%s %s", command, arg1) == 2)
		{
			execute_increment(arg1);
		}
	}
	else if (strstr(line, "--") == line)
	{
		if (sscanf(line, "%s %s", command, arg1) == 2)
		{
			execute_decrement(arg1);
		}
	}
	else if (strstr(line, "* ") == line)
	{
		if (sscanf(line, "%s %s %d", command, arg1, &value) == 3)
		{
			execute_multiply(arg1, value);
		}
	}
	else if (strstr(line, "/ ") == line)
	{
		if (sscanf(line, "%s %s %d", command, arg1, &value) == 3)
		{
			execute_divide(arg1, value);
		}
	}
	else if (strstr(line, "adharsh ") == line)
	{
		if (sscanf(line, "%s %s %d %s %s", command, arg1, &value, arg2, arg2) == 5)
		{
			execute_if(arg1, value, arg2, arg2);
		}
	}
	else if (strcmp(line, "kha\n") == 0)
	{
		printf("End of program\n");
		exit(0); // Terminate the program
	}
	else
	{
		printf("Syntax error: %s\n", line);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	if (argc > 2)
	{
		printf("Too many arguments\n");
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	if (!file)
	{
		perror("Could not open file");
		return 1;
	}

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		interpreter(line);
	}

	fclose(file);
	return 0;
}
