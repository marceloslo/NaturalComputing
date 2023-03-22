CC = g++
CFLAGS = -Wall

all: main clean

run_tuning: tuning
	tuning.exe entrada1.txt

tuning: ant.o ACO.o tuning.o 
	$(CC) $(CFLAGS) -o tuning tuning.o ant.o ACO.o

main: ant.o ACO.o main.o  
	$(CC) $(CFLAGS) -o main main.o ant.o ACO.o

ant.o:
	$(CC) -c Sources/ant.cpp ant.o

ACO.o:
	$(CC) -c Sources/ACO.cpp ACO.o

main.o:
	$(CC) -c main.cpp main.o

tune.o:
	$(CC) -c tuning.cpp tuning.o

clean:
	del main.o ACO.o ant.o