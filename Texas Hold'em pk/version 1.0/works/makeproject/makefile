vpath %.c ../source
vpath %.h ../source
objects = game.o Action.o card_decode.o command_decode.o HandCardsAssess.o msg_process.o Tables.o

game: $(objects)
	gcc -o game $(objects)
	cp game ../target/game

game.o: msg_process.h
	gcc -c ../source/game.c -I ../source

Action.o: 
	gcc -c ../source/Action.c

card_decode.o: card_decode.h
	gcc -c ../source/card_decode.c -I ../source

command_decode.o: command_decode.h
	gcc -c ../source/command_decode.c -I ../source

HandCardsAssess.o: HandCardsAssess.h Tables.h
	gcc -c ../source/HandCardsAssess.c -I ../source

msg_process.o: msg_process.h card_decode.h command_decode.h Message.h
	gcc -c ../source/msg_process.c -I ../source

Tables.o: Tables.h
	gcc -c ../source/Tables.c -I ../source

.PHONY: clean
clean: 
	-rm game $(objects) ../target/game



