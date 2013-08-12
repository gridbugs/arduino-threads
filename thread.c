#include <stdio.h>
#include <avr/io.h>
#include "limits.h"
#include "thread.h"
#include "switch.h"
#include "prepare_stack.h"
#include "clock.h"

static struct thread *thread_list[MAX_THREADS];
static unsigned stack_tops[MAX_THREADS];
static struct thread *current_thread;

static int thread_count;


static unsigned next_free( void )
{
    for (int i = 0;i<MAX_THREADS;i++) {
        if (thread_list[i] == 0) {
            return i;
        }
    }
    return -1;
}

static int schedule( void )
{
    int index = (current_thread->thread_id + 1) % MAX_THREADS;

    while (thread_list[index] == 0)
    {
        index = (index + 1) % MAX_THREADS;
    }

    return index;
}

/* threads_init
 *
 * Initialises the thread subsystem.
 *
 * The argument is so that the current thread of execution
 * can be assigned an actual thread object.
 */
void threads_init( struct thread *main_thread )
{
    
    // get the stack pointer
    unsigned sp = (SPH << 8) | SPL;

    /* initialise the thread list and the
     * list of stack tops
     */
    for (int i = 0;i<MAX_THREADS;i++) {
        thread_list[i] = 0;
        stack_tops[i] = sp - (i * ADDRESS_SPACE_SIZE);
    }

    thread_list[0] = main_thread;
    current_thread = main_thread;
    main_thread->thread_id = 0;

    thread_count = 1;
    
    // start the clock
    TIMER0_init();
}

/* thread_switch
 *
 * Performs a context switch into the new thread.
 */
void thread_switch( struct thread *new_thread )
{
    // update statuses
    current_thread->status = READY;
    new_thread->status = RUNNING;
    
    // save the old thread's address
    struct thread *old_thread = current_thread;

    // update the current_thread
    current_thread = new_thread;

    /* do the switch
     * When this function returns, it does so into
     * the new thread. This instance of the function
     * may return as well, but it will be as the 
     * result of a switch back into this thread.
     */
    context_switch(old_thread, new_thread);
}

/* thread_yield
 *
 * The current thread gives up the CPU to another thread
 * determined by the scheduler.
 */
void thread_yield( void )
{
    if (thread_count == 1) {
        return;
    }

    int next_index = schedule();
    struct thread *next_thread = thread_list[next_index];


    thread_switch(next_thread);
}

/* thread_create
 *
 * Initializes a thread object and places it in the thread
 * list so that it may be switched into by thread_switch.
 */
int thread_create( struct thread *thread,
                    void (*starting_point)(void))
{
    thread->thread_id = next_free();
    unsigned stack_top = stack_tops[thread->thread_id];

    thread->stack_pointer = prepare_stack(starting_point, 
                                          stack_top, 
                                          thread_exit);
    thread->status = READY;

    thread_list[thread->thread_id] = thread;
    
    thread_count++;

    return 0;
}

/* thread_exit
 *
 * Exits the current thread. This function is also called
 * when a thread's base function returns, with the return
 * value as the argument. Does not return.
 */
void thread_exit( int exit_status )
{

    thread_list[current_thread->thread_id] = 0;
    thread_count--;

    thread_yield();
}
