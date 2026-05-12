#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("ERROR\n");
        exit(1);
    }

    char* pos = argv[1];
    char ch;
    // run through characters in argv[1]
    while(*pos)
    {
        ch = *pos++;
        // not _
        if(ch != 95)
        {
            // check if small letter
            if(ch >= 97 && ch <= 122)
                ch -= 32;
            printf("%c", ch);
        }
        else
            printf("\n");
    }
    printf("\n");
    return 0;
}