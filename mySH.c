#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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
	int bufsize = sizeof(char*)*MYSH_TOK_BUFSIZE;
	char **tokens = (char **)malloc(bufsize);
	if (!tokens) e("mysh: allocation error");
	char *token;

	int argc = 0;
	
	token = strtok(line, MYSH_TOK_DELIM);
	while(token != NULL) {
		tokens[argc++] = token;

		if (argc >= MYSH_TOK_BUFSIZE) {
			bufsize += sizeof(char*)*MYSH_TOK_BUFSIZE;
			tokens = (char **)realloc(tokens, bufsize);
			if (!tokens) e("mysh: allocation error");
		}
		
		// In the tutorial, they use NULL instead of line
		// i dont know why, or if it is an oversight from the tutorial
		token = strtok(line, MYSH_TOK_DELIM);
	}

	return tokens;
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
