
OBJ1 = transfertcl.o
OBJ2 = transfertse.o 
OPTIONS	=
# Adaptation a Darwin / MacOS X avec fink
# Du fait de l'absence de libtermcap on se fait pas mal
# cracher dessus mais ca marche...
ifeq ($(shell uname), Darwin)
LFLAGS	+= -L/opt/local/lib
CFLAGS	+= -I /opt/local/include
endif
#Changer si necessaire le chemin d'acces aux librairies

# Adaptation a Linux
ifeq ($(shell uname), Linux)
OPTIONS	+= -ltermcap
endif

# Adaptation a FreeBSD
# Attention : il faut utiliser gmake...
ifeq ($(shell uname),FreeBSD)
OPTIONS	+= -ltermcap
endif

# Adaptation a Solaris

ifeq ($(shell uname),SunOS)
OPTIONS	+= -ltermcap  -lsocket -lnsl
CFLAGS	+= -I..
endif

EXEC = ${OBJ1} transfertcl ${OBJ2} transfertse
all: ${EXEC}

transfertcl.o : transfertcl.c
	gcc  $(CFLAGS) -c  transfertcl.c

transfertse.o :	transfertse.c
	gcc  $(CFLAGS) -c  transfertse.c

transfertcl : ${OBJ1}
	gcc $(LFLAGS) ${OBJ1} -o transfertcl -lcurses   $(OPTIONS)

transfertse : ${OBJ2}
	gcc $(LFLAGS) ${OBJ2} -o transfertse -lcurses   $(OPTIONS)



clean :
	rm -f ${EXEC} core 
