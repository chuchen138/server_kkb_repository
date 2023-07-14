.common:all

LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf

all:
	g++ -c -o common.o common.c 
	g++ -c -o thread.o thread_pool.c 
	g++ -c -o worker.o worker.c $(INC_PROTOBUF) $(LIB_PROTOBUF) -D _D 
	
clean:
	rm *.o 
