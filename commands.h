#ifndef COMMANDS_H
#define COMMANDS_H

#include "types.h"

// Command handler function signature
typedef void (*cmd_handler_t)(int argc, char* argv[]);

typedef struct {
    const char* name;
    const char* description;
    cmd_handler_t  handler;
} Command;

void init_command_system(void);
void dispatch_command(int argc, char* argv[]);

#endif // COMMANDS_H
