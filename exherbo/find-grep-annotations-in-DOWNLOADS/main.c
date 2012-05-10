#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*  Hard to imagine but this is possible:
 *
 *  DOWNLOADS="
 *      \"foo\"
 *  "
 */

char* find_ending_quote(char* start)
{
    char* p;

    p = strchr(start, '\"');
    if (p[-1] == '\\')
    	return find_ending_quote(p+1);
    else
    	return p;
}

int main(int argc, char *argv[])
{
    char *start, *end, *buf;
    int fd, match, re_errcode;
    struct stat stat;
    regex_t re_compiled;

    if (argc < 2)
    	error(1, 0, "argv[1] must be a path to a file.\n");

    if ((fd = open(argv[1], O_RDONLY)) == -1)
    	error(2, errno, "couldn't open file \"%s\"", argv[1]);

    fstat(fd, &stat);
    buf = malloc(stat.st_size + 1);
    read(fd, buf, stat.st_size);
    buf[stat.st_size] = '\0';
    close(fd);

    if ((start = strstr(buf, "DOWNLOADS")) == NULL)
    	error(3, 0, "No DOWNLOADS variable in this file.");

    end = find_ending_quote(start + 11);
    end[1] = '\0';

    if (re_errcode = regcomp(&re_compiled, "\\[\\[ [[:alnum:]]* = \\[", 0)) {
    	size_t len = regerror(re_errcode, &re_compiled, NULL, 0);
    	char *errbuf = malloc(len);
    	regerror(re_errcode, &re_compiled, errbuf, len);
    	error(4, 0, "regcomp error %d: %s.", re_errcode, errbuf);
    }

    if ((match = regexec(&re_compiled, start, 0, NULL, 0)) == 0) {
    	printf("%s\n", start);
    }

    return match;
}
