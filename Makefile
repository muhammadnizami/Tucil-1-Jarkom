all: bin/transmitter bin/receiver

bin/transmitter: src/transmitter.c
	gcc src/transmitter.c -o bin/transmitter -pthread

bin/receiver: src/receiver
	gcc src/receiver.c -o bin/receiver
