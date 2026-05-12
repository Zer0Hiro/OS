#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void ferr(int name, char* error);

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printf("Not enough Arguments");
        exit(1);
    }

    char* buffer[256];
    int dfd, fd_out, fd_in, in_read, out_write;

    // arguments
    int num = atoi(argv[2]);
    char* str = argv[3];

    // create full path
    char location[512];
    snprintf(location, sizeof(location), "encrypted/%s", argv[1]);

    fd_in = open(argv[1], O_RDONLY, 0);
    ferr(fd_in, "can't open input file");

    // create folder
    mkdir("encrypted", 0777);
    fd_out = open(location, O_WRONLY | O_CREAT, 0664);
    ferr(fd_out, "can't open location");

    while ((in_read = read(fd_in, buffer, num)) > 0)
    {
        out_write = write(fd_out, buffer, num);
        ferr(out_write, "can't write fd_out");

        if (out_write != in_read)
        {
            return -1;
            fprintf(stderr, "bad writing\n");
        }
        // add str
        write(fd_out, str, strlen(str));
    }
    ferr(in_read, "can't read fd_in");

    close(fd_in);
    close(fd_out);
    return 0;
}

// file error func
void ferr(int name, char* error)
{
    if (name < 0)
    {
        perror(error);
        exit(1);
    }
}
