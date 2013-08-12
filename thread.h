#define ADDRESS_SPACE_SIZE 256

struct address_space {
};

enum status {
    RUNNING,
    READY,
    BLOCKED
};

struct thread {
    unsigned stack_pointer;
    int thread_id;
    enum status status;
    struct address_space *address_space;
};

int thread_create( struct thread *thread,
                    void (*starting_point)(void));

void thread_switch( struct thread *new_thread );
void threads_init( struct thread *main_thread );
void thread_yield( void );
void thread_exit( int exit_status );
