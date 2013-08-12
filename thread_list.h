
struct thread_list {
    struct link *first;
    int length;
};

struct link {
    struct link *next;
    struct thread *thread;
};

