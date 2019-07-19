struct children_struct
{
    int first;
    int second;
};

struct grandchildren_struct
{
    int first;
    int second;
    int third;
    int fourth;
};

struct pids_init
{
    int parent;
    struct children_struct children;
    int grandchildren[4];
} pids_default = {0, {0, 0}, {0, 0, 0, 0}};
typedef struct pids_init Pids;

void print_pids(Pids pid_container);
void create_two_children(int *first_child_pid, int *second_child_pid);
Pids *create_shared_memory();
