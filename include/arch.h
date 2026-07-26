#ifndef ARCH_H
#define ARCH_H

#include "types.h"

// Hardware Abstraction Layer API
void arch_early_init(void);
void arch_putchar(char c);
void arch_print(const char* str);
void arch_reboot(void);
void arch_shutdown(void);
const char* arch_get_name(void);

#endif // ARCH_H
