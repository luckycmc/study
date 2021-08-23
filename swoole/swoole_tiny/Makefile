all:main.o Base.o Factory.o FactoryProcess.o FactoryThread.o PipeBase.o PipeEventfd.o PipeMsg.o PipeUnsock.o ReactorBase.o ReactorEpoll.o 	 ReactorPoll.o ReactorSelect.o RingMempool.o RingQueue.o Server.o  swoole.o
	gcc -g main.o Base.o Factory.o FactoryProcess.o FactoryThread.o PipeBase.o PipeEventfd.o PipeMsg.o PipeUnsock.o ReactorBase.o ReactorEpoll.o 	 ReactorPoll.o ReactorSelect.o RingMempool.o RingQueue.o Server.o  swoole.o -o tiny_swoole -lpthread

main.o:main.c
	 gcc -c main.c -o main.o
Base.o:Base.c
	 gcc -c Base.c -o Base.o
Factory.o:Factory.c
	 gcc -c Factory.c -o Factory.o
FactoryProcess.o:FactoryProcess.c
	 gcc -c FactoryProcess.c -o FactoryProcess.o -lpthread	 	 
FactoryThread.o:FactoryThread.c
	 gcc -c FactoryThread.c -o FactoryThread.o -lpthread
PipeBase.o:PipeBase.c
	 gcc -c PipeBase.c -o PipeBase.o
PipeEventfd.o:PipeEventfd.c
	 gcc -c PipeEventfd.c -o PipeEventfd.o	 
PipeMsg.o:PipeMsg.c
	 gcc -c PipeMsg.c -o PipeMsg.o	 
PipeUnsock.o:PipeUnsock.c
	 gcc -c PipeUnsock.c -o PipeUnsock.o	 	 
ReactorBase.o:ReactorBase.c
	 gcc -c ReactorBase.c -o ReactorBase.o	 	 
ReactorEpoll.o:ReactorEpoll.c
	 gcc -c ReactorEpoll.c -o ReactorEpoll.o	 
ReactorPoll.o:ReactorPoll.c
	 gcc -c ReactorPoll.c -o ReactorPoll.o	 
ReactorSelect.o:ReactorSelect.c
	 gcc -c ReactorSelect.c -o ReactorSelect.o	 
RingMempool.o:RingMempool.c
	 gcc -c RingMempool.c -o RingMempool.o	 
RingQueue.o:RingQueue.c
	 gcc -c RingQueue.c -o RingQueue.o	 
Server.o:Server.c
	 gcc -c Server.c -o Server.o -lpthread	 	 
swoole.o:swoole.c
	 gcc -c swoole.c -o swoole.o

.PYTHON:clean
clean:
	rm -rf *.o	 tiny_swoole