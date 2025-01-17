all: ohosh

ohosh: mySH.c
	gcc -o ohosh mySH.c
clean:
	rm ohosh
