#CC=gcc

IDIR=-I.

SOURCE=
SOURCE+=adt7310_driver.c
SOURCE+=spi.c
#CFLAG=-lpigpio #For pthread module
CFLAG+=-Wall
build:
	$(CC) -o adt7310_driver.out $(SOURCE) $(IDIR) $(CFLAG)

