#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "fork.h"

int main(int argc, char *argv[])
{
    Pids *pid_container = create_shared_memory();
    pid_container->parent = getpid();

    create_two_children(&pid_container->children.first, &pid_container->children.second);

    if (getpid() == pid_container->children.first)
    {
        create_two_children(&pid_container->grandchildren[0], &pid_container->grandchildren[1]);
    }

    if (getpid() == pid_container->children.second)
    {
        create_two_children(&pid_container->grandchildren[2], &pid_container->grandchildren[3]);
    }

    if (getpid() == pid_container->parent)
    {

        if (argc > 1 && strcmp(argv[1], "sleep") == 0)
        {
            int printed = 0;
            while (1)
            {
                if (!printed)
                {
                    if (pid_container->grandchildren[0] &&
                        pid_container->grandchildren[1] &&
                        pid_container->grandchildren[2] &&
                        pid_container->grandchildren[3])
                    {
                        print_pids(*pid_container);
                        printed = 1;
                    }
                }
            };
        }
        else
        {
            waitpid(pid_container->children.first, NULL, 0);
            waitpid(pid_container->children.second, NULL, 0);
            print_pids(*pid_container);
        }
    }
    if (argc > 1 && strcmp(argv[1], "sleep") == 0)
    {
        while (1)
            ;
    }
    return 0;
}

Pids *create_shared_memory()
{
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(NULL, sizeof(Pids), protection, visibility, -1, 0);
}

void print_pids(Pids pid_container)
{
    printf("Children: %d %d\n", pid_container.children.first, pid_container.children.second);
    printf("Grandchildren: %d %d %d %d\n",
           pid_container.grandchildren[0],
           pid_container.grandchildren[1],
           pid_container.grandchildren[2],
           pid_container.grandchildren[3]);
    printf("Parent: %d\n", pid_container.parent);
}

void create_two_children(int *first_child_pid, int *second_child_pid)
{
    int pid_self = getpid();
    int pid_temp = fork();
    if (getpid() == pid_self)
    {
        *first_child_pid = pid_temp;
        pid_temp = fork();
        if (getpid() == pid_self)
            *second_child_pid = pid_temp;
    }
}