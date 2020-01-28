adventure: peterlak.adventure.o
	gcc -std=c89 -o peterlak.adventure peterlak.adventure.o
peterlak.adventure.o:
	gcc -std=c89 -c peterlak.adventure.c
buildrooms: peterlak.buildrooms.o
	gcc -std=c89 -o peterlak.buildrooms peterlak.buildrooms.o
peterlak.buildrooms.o:
	gcc -std=c89 -c peterlak.buildrooms.c
clean:
	rm -f *.o peterlak.adventure peterlak.buildrooms
