#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Not enough Arguments");
        exit(1);
    }
    int fd1;

    fd1 = open(argv[1], O_RDWR | O_APPEND | O_CREAT, 0664);
    if (fd1 < 0)
    {
        perror("cant open");
        exit(1);
    }

    write(fd1, "END\n", 4);
    close(fd1);

    return 0;
}