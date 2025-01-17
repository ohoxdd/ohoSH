#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void e(char *s) {
	perror(s);
	exit(EXIT_FAILURE);
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
		//c = my_getchar();
		c = getchar();
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
		token = strtok(NULL, MYSH_TOK_DELIM);
	}

	tokens[argc] = (char *)0; // last element is a char* nullptr

	return tokens;
}

int mysh_launch(char **args) {
	pid_t ret, wpid;
	int status;

	ret = fork();
	if (ret < 0) e("fork error");
	if (ret == 0) {
		execvp(args[0], args);
		e("execvp");
	}

	do {
		wpid = waitpid(ret, &status, WUNTRACED);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));


	return 1;
}

int mysh_cd(char **args);
int mysh_exit(char **args);
int mysh_help(char **args);

char *builtin_names[] = {
	"cd", "exit", "help"
};

int (*builtin_func[]) (char**) = {
	&mysh_cd,
	&mysh_exit,
	&mysh_help
};

int mysh_cd(char **args) {

	char *dir;
	
	if (args[1] == NULL) {
		dir = "$HOME";
	} else {
		dir = args[1];
	}

	if (chdir(dir) < 0) e("mysh: chdir");

	return 1;
}

int mysh_num_builtin() {
	return sizeof(builtin_names)/sizeof(char *);
}

int mysh_exit(char **args) {
	return 0;
}

int mysh_help(char **args) {
	printf("ohoxd's shell OHOSH\n");
	printf("Type the name and arguments of the program, and hit enter.\n");
	printf("The following commands are built-in:\n\n");

	for (int i = 0; i < mysh_num_builtin(); ++i) {
		printf("\t%s\n", builtin_names[i]);
	}
	printf("\n");

	printf("RTFM for anything else.\n");
	printf("Angel Quiñones' Framework will instantly explode if any command is executed.\n");

	return 1;
}

int mysh_execute(char **args) {
	
	// We check if the command which is being executed 
	// is a shell built-in command 
	
	char *command = args[0];

	for (int i = 0; i < mysh_num_builtin(); i++) {
		if (strcmp(command, builtin_names[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return mysh_launch(args);
}

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
