#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void e(char *s) {
	fprintf(stderr, "%s", s);
	exit(errno);
}

int my_getchar() {
	int c, ret;
	int fd = STDIN_FILENO;
	ret = read(fd, &c, sizeof(char));
	if (ret < 0) e("mysh: rd getchar");
	if (ret > 0) return c;
	return EOF;
}


#define MYSH_RDLN_BUFSIZE 1024
char *mysh_rdln(void) {
	int pos = 0;
	int bufsize = sizeof(char)*MYSH_RDLN_BUFSIZE; 
	char *buf = (char *)malloc(bufsize);
	if (!buf) e("mysh: allocation error"); // malloc returns null pointer
	
	int c;
	while(1) {
		c = my_getchar();
		if (c == EOF || c == '\n') {
			buf[pos] = '\0';
			return buf;
		}

		buf[pos] = c;

		++pos;
		if (pos >= MYSH_RDLN_BUFSIZE) {
			bufsize += sizeof(char)*MYSH_RDLN_BUFSIZE;
			buf = (char*)realloc(buf, bufsize);
			if (!buf) e("mysh: allocation error"); // malloc returns null pointer
		}
	}

	return NULL;
}


#define MYSH_TOK_BUFSIZE 64
#define MYSH_TOK_DELIM "  \t\r\n\a"
char** mysh_parse(char* line) {
	int bufsize = sizeof(char)*MYSH_TOK_BUFSIZE;
	char *buf = (char *)malloc(bufsize);
	if (!buf) e("mysh: allocation error");

	while(1) {

	}
}
int mysh_execute(char **args);

void mysh_loop() {
	
	char *line;
	char **args;
	int status;

	do {
		
		printf("~> ");

		line = mysh_rdln();
		args = mysh_parse(line);
		status = mysh_execute(args);

		free(line);
		free(args);

	} while(status);


}

int main(int argc, char **argv) {
	// Load config files
	

	mysh_loop();

	// Cleanup
	

	return EXIT_SUCCESS;
}
