#include "types.h"

extern void* kmalloc(size_t size);

typedef struct Task {
    uint32_t id;
    uint32_t esp;            // Stack pointer register
    uint32_t ebp;            // Base pointer register
    uint32_t eip;            // Instruction pointer register
    struct Task* next;       // Pointer to next task in linked list
} Task;

static Task* current_task = NULL;
static Task* task_list_head = NULL;
static uint32_t next_pid = 1;

/**
 * @brief Creates a new concurrent process with its own dedicated stack space
 */
Task* create_task(void (*entry_point)(void)) {
    Task* new_task = (Task*)kmalloc(sizeof(Task));
    new_task->id = next_pid++;
    
    // Allocate 16 KB stack for the new task
    uint32_t stack = (uint32_t)kmalloc(16384) + 16384;

    // Set up initial execution registers
    new_task->esp = stack;
    new_task->ebp = stack;
    new_task->eip = (uint32_t)entry_point;
    new_task->next = NULL;

    // Add to circular linked list
    if (!task_list_head) {
        task_list_head = new_task;
        new_task->next = new_task; // Circular reference
    } else {
        Task* temp = task_list_head;
        while (temp->next != task_list_head) {
            temp = temp->next;
        }
        temp->next = new_task;
        new_task->next = task_list_head;
    }

    return new_task;
}

/**
 * @brief Switches execution to the next process in the queue (Round-Robin)
 */
void schedule(void) {
    if (!current_task) {
        current_task = task_list_head;
        return;
    }

    // Switch to next task pointer
    current_task = current_task->next;
}
