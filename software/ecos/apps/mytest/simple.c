#include <stdio.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>

// These numbers depend entirely on your application
#define NUMBER_OF_WORKERS    4
#define PRODUCER_PRIORITY   10
#define WORKER_PRIORITY     11
#define PRODUCER_STACKSIZE  CYGNUM_HAL_STACK_SIZE_TYPICAL
#define WORKER_STACKSIZE    (CYGNUM_HAL_STACK_SIZE_MINIMUM + 1024)

static unsigned char producer_stack[PRODUCER_STACKSIZE];
static unsigned char worker_stacks[NUMBER_OF_WORKERS][WORKER_STACKSIZE];
static cyg_handle_t producer_handle, worker_handles[NUMBER_OF_WORKERS];
static cyg_thread   producer_thread, worker_threads[NUMBER_OF_WORKERS];

static void
producer(cyg_addrword_t data)
{
printf("producer\n");
}

static void
worker(cyg_addrword_t data)
{
printf("worker\n");
}

void
cyg_user_start(void)
{
    int i;

    cyg_thread_create(PRODUCER_PRIORITY, &producer, 0, "producer",
                      producer_stack, PRODUCER_STACKSIZE,
                      &producer_handle, &producer_thread);
    cyg_thread_resume(producer_handle);
    for (i = 0; i < NUMBER_OF_WORKERS; i++) {
        cyg_thread_create(WORKER_PRIORITY, &worker, i, "worker",
                          worker_stacks[i], WORKER_STACKSIZE,
                          &(worker_handles[i]), &(worker_threads[i]));
        cyg_thread_resume(worker_handles[i]);
    }
}

void cyg_hal_plf_serial_putc( void )
{
}
