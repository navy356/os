#include "io.h"
#include "kheap.h"
#include "terminal.h"
#include "string.h"

struct command
{
    char *cmd;
    uint32_t offset;
};
typedef struct command CMD;
CMD *core;

void printCmd(CMD *command)
{
    write(command->cmd);
    write("\n");
}

void handleCmd(char *cmd)
{
    int l = strlen(cmd);
    char *command = NULL;
    int i;
    for (i = 0; i < l+1; i++)
    {
        if (cmd[i] == ' ' || cmd[i] == '\0')
        {
            command = (char *)malloc((l + 1) * sizeof(char), 0);
            int j;
            for (j = 0; j < i; j++)
            {
                command[j] = cmd[j];
            }
            command[j] = '\0';
            break;
        }
    }
    if (command != NULL && strcmp(command, "echo") == 0)
    {
        write(&cmd[i + 1]);
        write("\n");
    }
    else
    {
        write(cmd);
        write(": unrecognized command\n");
    }
}

void getCommand(char c)
{
    if (!core)
    {
        core = (CMD *)malloc(sizeof(CMD), 0);
        core->cmd = (char *)malloc(INITSIZE, 0);
        core->offset = 0;
    }
    if (c == '\n' && core && (core->offset > 0))
    {
        core->cmd[core->offset++] = '\0';
        handleCmd(core->cmd);
        memset(core->cmd, 0, core->offset);
        core->offset = 0;
    }
    else
    {
        core->cmd[core->offset++] = c;
    }
}
void terminal()
{
    char c = 0;
    write("MOS-0.1$ ");
    while (c != '\n')
    {
        c = getc();
        getCommand(c);
    }
}

void startTerminal()
{
    while (1)
    {
        terminal();
    }
}