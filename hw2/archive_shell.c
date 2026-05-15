#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER 1024
#define ARG_SIZE 10

int enoughArgs(int arguments, int size);
void merge(int file1, int file2);
void count(int file1);

// FIX CONTINUE PROBLEM

int main()
{
    char input[BUFFER];
    char* args[ARG_SIZE];
    int pid, size, fd1, fd2;

    printf("Entering Archive Shell...\n");

    while (1)
    {
        printf("Archive $** ");
        if (fgets(input, BUFFER, stdin) == NULL)
        {
            perror("fgets failed!\n");
            exit(1);
        }

        // Stop condition
        if (strcmp("Esc\n", input) == 0) break;

        input[strcspn(input, "\n")] = '\0';

        // Tokenize arguments
        int i = 0;
        char* token = strtok(input, " ");

        while (token != NULL && i < ARG_SIZE - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        size = i;

        // No arguments at all
        if (args[0] == NULL)
        {
            printf("Not Supported\n");
            continue;
        }
        // merge <src> <dst>
        if (strcmp(args[0], "merge") == 0)
        {
            if (!enoughArgs(size, 3)) continue;
            fd2 = open(args[1], O_RDONLY, 0664);
            if (fd2 < 0) printf("File %s not found\n", args[1]);
            fd1 = open(args[2], O_RDWR | O_APPEND, 0664);
            if (fd1 < 0) printf("File %s not found\n", args[2]);
            merge(fd1, fd2);
            printf("Data from %s merged into %s\n", args[1], args[2]);
        }
        // count <file>
        else if (strcmp(args[0], "count") == 0)
        {
            if (!enoughArgs(size, 2)) continue;
            fd1 = open(args[1], O_RDONLY, 0664);
            if (fd1 < 0) printf("File %s not found\n", args[1]);
            count(fd1);
        }
        // remove <file>
        else if (strcmp(args[0], "remove") == 0)
        {
            if (unlink(args[1]) != 0) printf("File not found\n");
        }
        // protect <mode> <file>
        else if (strcmp(args[0], "protect") == 0)
        {
            int mode = atoi(args[1]);
            if (mode > 777 || mode < 0)
            {
                printf("Invalid Mode!!\n");
            }
            else if (chmod(args[2], strtol(args[1], NULL, 8)) == 0)
                printf("Permission updated\n");
            else
                perror("chmod");
        }
        else
            printf("Not Supported\n");
    }
    printf("Returning to LibShell...\n");
    return 0;
}

// Check if amount of arguments smaller than we demand
int enoughArgs(int arguments, int size)
{
    if (arguments < size)
    {
        printf("Missing parameters\n");
        return 0;
    }
    return 1;
}

// Appends file2 to file1
void merge(int file1, int file2)
{
    char buffer[BUFFER];
    int f1_w, f2_r = 1;

    f2_r = read(file2, buffer, BUFFER);
    if (f2_r < 0)
    {
        perror("Reading file");
        exit(1);
    }

    while (f2_r > 0)
    {
        f1_w = write(file1, buffer, f2_r);
        if (f1_w < 0)
        {
            perror("Append failed");
            exit(1);
        }

        if (f2_r != f1_w)
        {
            fprintf(stderr, "bad write\n");
            return;
        }

        f2_r = read(file2, buffer, BUFFER);
        if (f2_r < 0)
        {
            perror("Reading file");
            exit(1);
        }
    }

    close(file1);
    close(file2);
}

void count(int file1)
{
    char buffer[BUFFER];
    int f1_r = 1;
    int chars = 0, lines = 0, i;
    char last_char = '\n';  // Tracks the last char

    f1_r = read(file1, buffer, BUFFER);
    if (f1_r < 0)
    {
        perror("Reading file");
        exit(1);
    }

    while (f1_r > 0)
    {
        // Count characters
        chars += f1_r;

        for (i = 0; i < f1_r; i++)
        {
            // Count lines
            if (buffer[i] == '\n') lines++;

            // Update the last character we've seen
            last_char = buffer[i];
        }

        f1_r = read(file1, buffer, BUFFER);
        if (f1_r < 0)
        {
            perror("Reading file");
            exit(1);
        }
    }

    // If the file didn't end with a new line => count the final line
    if (last_char != '\n' && (chars > 0 || lines > 0)) lines++;

    printf("Characters: %d Lines: %d\n", chars, lines);
    close(file1);
}