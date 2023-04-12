#
#	This makefile will compile and build all the source files
#	needed for the encodeInput utility. The purpose of this 
#	utility is to transform binary files into either a -srec
#	file or an assembly file with the correct format.
#
# =======================================================
#                  EncodeInput
# =======================================================

#	FINAL BINARY TARGET
.bin/encodeInput : ./obj/main.o ./obj/parse.o ./obj/encryption.o
	cc 	   ./obj/main.o ./obj/parse.o ./obj/encryption.o -o ./bin/encodeInput

#
# =======================================================
#                     Dependencies
# =======================================================
./obj/main.o :		./src/main.c ./inc/encodeInput.h
	cc -c		./src/main.c -o ./obj/main.o

./obj/parse.o:		./src/parse.c ./inc/encodeInput.h
	cc -c		./src/parse.c -o ./obj/parse.o

./obj/encryption.o:	./src/encryption.c ./inc/encodeInput.h
	cc -c		./src/encryption.c -o ./obj/encryption.o

#
# =======================================================
# Other targets
# =======================================================    
all:	./bin/encodeInput

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o




