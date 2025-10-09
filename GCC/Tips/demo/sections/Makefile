all: main_sections main_normal

main_sections:
	gcc -ffunction-sections -fdata-sections -c main.c -o main_sections.o
	gcc -Wl,--gc-sections -o $@ main_sections.o

main_normal:
	gcc -c main.c -o main_normal.o
	gcc -o $@ main_normal.o

clean:
	rm -rf *.o main_sections main_normal
