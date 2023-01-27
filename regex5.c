#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <regex.h>

#define tofind    "^daemons=\\(([^)]*)\\)[ \t]*$"

int main(int argc, char **argv)
{
    FILE *fp;
    char line[1024];
    int retval = 0;
    regex_t re;
    regmatch_t rm[2];
    //this file has this line "DAEMONS=(sysklogd network sshd !netfs !crond)"
    const char *filename = "/etc/rc1.d";  // was /etc/rc.conf

    if (argc > 1)
        filename = argv[1];

    if (regcomp(&re, tofind, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "Failed to compile regex '%s'\n", tofind);
        return EXIT_FAILURE;
    }
    printf("Regex: %s\n", tofind);
    printf("Number of captured expressions: %zu\n", re.re_nsub);

    fp = fopen(filename, "r");
    if (fp == 0)
    {
        fprintf(stderr, "Failed to open file %s (%d: %s)\n", filename, errno, strerror(errno));
        return EXIT_FAILURE;
    }

    while ((fgets(line, 1024, fp)) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        if ((retval = regexec(&re, line, 2, rm, 0)) == 0)
        {
            printf("<<%s>>\n", line);
            // Complete match
            printf("Line: <<%.*s>>\n", (int)(rm[0].rm_eo - rm[0].rm_so), line + rm[0].rm_so);
            // Match captured in (...) - the \( and \) match literal parenthesis
            printf("Text: <<%.*s>>\n", (int)(rm[1].rm_eo - rm[1].rm_so), line + rm[1].rm_so);
            char *src = line + rm[1].rm_so;
            char *end = line + rm[1].rm_eo;
            while (src < end)
            {
                size_t len = strcspn(src, " ");
                if (src + len > end)
                    len = end - src;
                printf("Name: <<%.*s>>\n", (int)len, src);
                src += len;
                src += strspn(src, " ");
            }
        }
    } 
    return EXIT_SUCCESS;
}
