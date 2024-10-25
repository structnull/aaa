#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLES 100
#define MAX_VAR_NAME 20

void interpreter(char *line);

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
    // Check if the first character is a double quote (")
    if (arg[0] == '"')
    {
        // Find the closing quote and remove it
        char *closing_quote = strrchr(arg, '"');
        if (closing_quote != NULL)
        {
            *closing_quote = '\0'; // Remove the closing quote
            printf("%s\n", arg + 1); // Print the string excluding the initial quote
        }
        else
        {
            fprintf(stderr, "Syntax error: Missing closing quote in string.\n");
        }
    }
    else if (isdigit(arg[0]) || (arg[0] == '-' && isdigit(arg[1])))
    {
        printf("%d\n", atoi(arg)); // Print numbers as integers
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

void execute_if(const char *var_name, int expected_value, const char *block)
{
    // Execute the block only if the variable's value matches the expected value
    if (get_variable_value(var_name) == expected_value)
    {
        interpreter((char *)block); // Reuse interpreter to execute the block of commands
    }
}

void interpreter(char *line)
{
    char *comment_start;

    // Remove comments (using # or //)
    if ((comment_start = strstr(line, "#")) != NULL || (comment_start = strstr(line, "//")) != NULL)
    {
        *comment_start = '\0';
    }

    // Remove leading spaces and tabs
    while (*line == ' ' || *line == '\t')
        line++;

    // Remove trailing newline
    line[strcspn(line, "\n")] = 0;

    if (strlen(line) == 0)
        return;

    // Split by '.' to handle multiple statements
    char *statement = strtok(line, ".");

    while (statement != NULL)
    {
        // Trim leading and trailing whitespace from the statement
        while (*statement == ' ' || *statement == '\t')
            statement++;

        if (strlen(statement) == 0)
        {
            statement = strtok(NULL, ".");
            continue;
        }

        // Handle print statement
        char command[20], arg1[256];

        if (strstr(statement, "anjima ") == statement)
        {
            if (sscanf(statement, "%s %[^\n]", command, arg1) == 2)
            {
                execute_print(arg1); // Pass the whole argument to execute_print
            }
        }
        // Handle variable declaration
        else if (strstr(statement, "vro ") == statement)
        {
            if (sscanf(statement, "%s %s", command, arg1) == 2)
            {
                execute_declaration(arg1);
            }
        }
        // Handle exit command
        else if (strncmp(statement, "anakha", 6) == 0)
        {
            exit(0);
        }
        // Handle assignment (x = 10)
        else if (strstr(statement, "=") != NULL && strstr(statement, "==") == NULL)
        {
            char var_name[256];
            int value;
            if (sscanf(statement, "%s = %d", var_name, &value) == 2)
            {
                execute_assignment(var_name, value);
            }
            else
            {
                fprintf(stderr, "Syntax error: Invalid assignment: %s\n", statement);
            }
        }
        // Handle increment (x++)
        else if (strstr(statement, "++") != NULL)
        {
            char var_name[256];
            if (sscanf(statement, "%s++", var_name) == 1)
            {
                execute_increment(var_name);
            }
            else
            {
                fprintf(stderr, "Syntax error: Invalid increment: %s\n", statement);
            }
        }
        // Handle if condition with () and block {}
        else if (strncmp(statement, "if (", 4) == 0)
        {
            char var_name[256];
            int expected_value;
            char *condition_start = strchr(statement, '(');
            char *condition_end = strchr(statement, ')');
            char *block_start = strstr(statement, "{");
            char *block_end = strstr(statement, "}");

            if (condition_start != NULL && condition_end != NULL && block_start != NULL && block_end != NULL && block_start < block_end)
            {
                // Extract the variable and expected value from inside the parentheses
                if (sscanf(condition_start + 1, "%s == %d", var_name, &expected_value) == 2)
                {
                    // Extract the block of commands between { and }
                    *block_end = '\0'; // Temporarily end the string at the closing }
                    execute_if(var_name, expected_value, block_start + 1);
                }
                else
                {
                    fprintf(stderr, "Syntax error: Invalid if statement: %s\n", statement);
                }
            }
            else
            {
                fprintf(stderr, "Syntax error: Missing or misbalanced parentheses/braces in if statement: %s\n", statement);
            }
        }
        else
        {
            fprintf(stderr, "Syntax error: %s\n", statement);
        }

        // Move to the next statement in the line
        statement = strtok(NULL, ".");
    }
}


int main(int argc, char *argv[])
{
	if (argc!=2)
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
