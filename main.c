#include <ctype.h>
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

int is_valid_variable_name(const char *name)
{
	if (!isalpha(name[0]))
		return 0;
	for (int i = 1; name[i] != '\0'; i++)
	{
		if (!isalnum(name[i]) && name[i] != '_')
			return 0;
	}
	return 1;
}

int get_variable_value(const char *name)
{
	for (int i = 0; i < varCount; i++)
	{
		if (strcmp(variables[i].name, name) == 0)
		{
			return variables[i].value;
		}
	}
	fprintf(stderr, "Error: Variable '%s' not found.\n", name);
	return 0;
}

void set_variable(const char *name, int value)
{
	if (!is_valid_variable_name(name))
	{
		fprintf(stderr, "Error: Invalid variable name '%s'.\n", name);
		return;
	}

	for (int i = 0; i < varCount; i++)
	{
		if (strcmp(variables[i].name, name) == 0)
		{
			variables[i].value = value;
			return;
		}
	}
	if (varCount < MAX_VARIABLES)
	{
		strncpy(variables[varCount].name, name, MAX_VAR_NAME);
		variables[varCount].value = value;
		varCount++;
	}
	else
	{
		fprintf(stderr, "Error: Maximum number of variables reached.\n");
	}
}

void execute_print(const char *arg)
{
	if (arg[0] == '"')
	{
		printf("%s\n", arg + 1);
	}
	else if (isdigit(arg[0]) || (arg[0] == '-' && isdigit(arg[1])))
	{
		printf("%d\n", atoi(arg));
	}
	else
	{
		printf("%d\n", get_variable_value(arg));
	}
}

void execute_declaration(const char *var_name)
{
	set_variable(var_name, 0);
}

void execute_assignment(const char *var_name, int value)
{
	set_variable(var_name, value);
}

void execute_increment(const char *var_name)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current + 1);
}

void execute_decrement(const char *var_name)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current - 1);
}

void execute_multiply(const char *var_name, int value)
{
	int current = get_variable_value(var_name);
	set_variable(var_name, current * value);
}

void execute_divide(const char *var_name, int value)
{
	if (value == 0)
	{
		fprintf(stderr, "Error: Division by zero.\n");
		return;
	}
	int current = get_variable_value(var_name);
	set_variable(var_name, current / value);
}

void execute_modulus(const char *var_name, int value)
{
	if (value == 0)
	{
		fprintf(stderr, "Error: Division by zero in modulus operation.\n");
		return;
	}
	int current = get_variable_value(var_name);
	set_variable(var_name, current % value);
}

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

void interpreter(char *line)
{
	char *comment_start;

	if ((comment_start = strstr(line, "#")) != NULL || (comment_start = strstr(line, "//")) != NULL)
	{
		*comment_start = '\0';
	}
	while (*line == ' ' || *line == '\t')
		line++;
	line[strcspn(line, "\n")] = 0;

	if (strlen(line) == 0)
		return;

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
		if (sscanf(line, "%s %[^\n]", command, arg1) == 2)
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
	else if (strstr(line, "% ") == line)
	{
		if (sscanf(line, "%s %s %d", command, arg1, &value) == 3)
		{
			execute_modulus(arg1, value);
		}
	}
	else if (strstr(line, "adharsh ") == line)
	{
		if (sscanf(line, "%s %s %d %s %[^\n]", command, arg1, &value, arg2, arg2) == 5)
		{
			execute_if(arg1, value, arg2, arg2);
		}
	}
	else if (strncmp(line, "anakha", 6) == 0)
	{
		exit(0);
	}
	else
	{
		fprintf(stderr, "Syntax error: %s\n", line);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
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
