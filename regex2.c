#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
regex_t regex;
int reti;
char msgbuf[100];

int main(int argc, char const *argv[])
{
    while(1){
        fgets( msgbuf, 100, stdin );
        reti = regcomp(&regex, "^(-)?([0-9]+)((,|.)([0-9]+))?\n$", REG_EXTENDED);
        if (reti) {
            fprintf(stderr, "Could not compile regex\n");
            exit(1);
        }

        /* Execute regular expression */
        printf("%s\n", msgbuf);
        reti = regexec(&regex, msgbuf, 0, NULL, 0);
        if (!reti) {
            puts("Match");
        }
        else if (reti == REG_NOMATCH) {
            puts("No match");
        }
        else {
            regerror(reti, &regex, msgbuf, sizeof(msgbuf));
            fprintf(stderr, "Regex match failed: %s\n", msgbuf);
            exit(1);
        }

        /* Free memory allocated to the pattern buffer by regcomp() */
        regfree(&regex);
    }

}
