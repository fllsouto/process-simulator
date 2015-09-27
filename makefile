#   **************************************************************************************************
#
#   Fellipe Souto Sampaio - 7990422
#   Computer Science Undergraduate - University of São Paulo (IME/USP)
#   Operational Systems - Program Exercise I - Makefile
#   Prof: Daniel M. Batista
#
#   September, 2015
#
#   **************************************************************************************************

CFLAGS = -g -I. -Wall -pedantic -ansi
LDFLAGS = -L.
LDLIBS_SH = -lm -lreadline 
LDLIBS_SCH = -lm -lpthread -lrt

RM = /bin/rm -f
CC = gcc

#======================================================================

.c.o:
	$(CC) $(CFLAGS) -c $*.c

.c:
	make $*.o
	$(CC) $(CFLAGS) -o $* $*.o $(LDLIBS)

#======================================================================

clean:
	$(RM) ep1sh ep1 *.o src/objects/*.o exec/ep1sh exec/ep1

#======================================================================

ep1sh: main.o commandLine.o coreCommands.o
	$(CC) -o ep1sh coreCommands.o commandLine.o main.o $(LDLIBS_SH)
#======================================================================

ep1: scheduler_main.o fcfs_core.o sjf_core.o srtf_core.o rr_core.o pr_core.o rtshd_core.o stderr_msg.o scheduler_core.o
	$(CC) -o ep1 scheduler_main.o fcfs_core.o sjf_core.o srtf_core.o rr_core.o pr_core.o rtshd_core.o stderr_msg.o scheduler_core.o $(LDLIBS_SCH)
#======================================================================

exec: ep1 ep1sh
	mv *.o src/objects/
	mv ep1sh exec/
	mv ep1 exec/

#======================================================================
# EP1 Shell
#======================================================================

main.o: src/main.c
	gcc -c src/main.c $(CFLAGS)
commandLine.o: src/commandLine.c
	gcc -c src/commandLine.c $(CFLAGS)
coreCommands.o: src/coreCommands.c
	gcc -c src/coreCommands.c $(CFLAGS)
#======================================================================
# EP1 Scheduler Simulator
#======================================================================

scheduler_main.o: src/scheduler_main.c
	gcc -c src/scheduler_main.c $(CFLAGS)
fcfs_core.o: src/fcfs_core.c
	gcc -c src/fcfs_core.c $(CFLAGS)
sjf_core.o: src/sjf_core.c
	gcc -c src/sjf_core.c $(CFLAGS)
srtf_core.o: src/srtf_core.c
	gcc -c src/srtf_core.c $(CFLAGS)
rr_core.o: src/rr_core.c
	gcc -c src/rr_core.c $(CFLAGS)
pr_core.o: src/pr_core.c
	gcc -c src/pr_core.c $(CFLAGS)
rtshd_core.o: src/rtshd_core.c
	gcc -c src/rtshd_core.c $(CFLAGS)
stderr_msg.o: src/stderr_msg.c
	gcc -c src/stderr_msg.c $(CFLAGS)
scheduler_core.o: src/scheduler_core.c
	gcc -c src/scheduler_core.c $(CFLAGS)
#======================================================================

