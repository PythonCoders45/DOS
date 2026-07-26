#include "types.h"

/**
 * @brief Calculates percentage using integer math (0-100%)
 */
int returnPerc(int value, int total) {
    if (total == 0) return 0;
    return (value * 100) / total;
}
