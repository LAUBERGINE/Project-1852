main: main.c
	gcc -Wall main.c -o PlantsPASS

update:
	rm -f PlantsPASS
	gcc -Wall main.c -o PlantsPASS
clean:
	rm -f PlantsPASS
	rm -f password.txt
	rm -f userkey.key
	rm -f machinekey.key
