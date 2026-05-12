#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void compare(int file1, int file2);
void ferr(int name, char* error);
int copy(int from, char* to);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        perror("Not enough Arguments");
        exit(1);
    }

    //open both files
    int fd1 = open(argv[1], O_RDONLY, 0);
    ferr(fd1, "Error opening file");

    int fd2 = open(argv[2], O_RDONLY, 0);
    ferr(fd2, "Error opening file");

    compare(fd1, fd2);
    close(fd1);
    close(fd2);
    return 0;
}

// compares 2 files if not identical creates new file with context of both files
void compare(int file1, int file2)
{
    char buff1[256];
    char buff2[256];
    int f1_r = 1;
    int f2_r = 1;

    // compare whole file
    while (f1_r == f2_r && f1_r > 0)
    {
        f1_r = read(file1, buff1, 256);
        ferr(f1_r, "Error reading file1");

        f2_r = read(file2, buff2, 256);
        ferr(f2_r, "Error reading file2");

        if (strcmp(buff1, buff2) != 0)
        {
            printf("Different files\n");
            // reset pointers
            lseek(file1, 0, 0);
            lseek(file2, 0, 0);
            // start copying
            copy(file1, "compare.txt");
            copy(file2, "compare.txt");
            return;
        }
    }
    printf("Identical files\n");
    return;
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

// copy context to new file
int copy(int from, char* to)
{
    int fd_to, from_r, to_w;
    char buffer[256];

    fd_to = open(to, O_WRONLY | O_APPEND | O_CREAT, 0664);
    ferr(fd_to, "open 'to'");

    from_r = read(from, buffer, 256);
    ferr(from_r, "reading file 'from'");

    while (from_r > 0)
    {
        to_w = write(fd_to, buffer, from_r);
        ferr(to_w, "writing to 'to'");

        if (from_r != to_w)
        {
            fprintf(stderr, "bad write\n");
            return 1;
        }

        from_r = read(from, buffer, 256);
        ferr(from_r, "reading file 'from'");
    }
    close(fd_to);
    return 0;
}