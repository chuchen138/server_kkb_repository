.SXG:all

# protobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf
LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
OBJ_SSP = pb/*.o 
OBJ_THREAD = common/*.o ./*.o

# mysql
LIB_MYSQL = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

#thread_pool
LIB_THREAD = -pthread

all: subsystem user relationinfo relation message photo db bus project

subsystem:
	$(MAKE) -C ./pb/
	$(MAKE) -C ./common/
	# cd pb/ && $(MAKE)
	# cd common/ && $(MAKE)

user:UserManager.cpp
	g++ -c $^ -o user.o
relationinfo:RelationInfo.cpp
	g++ -c $^ -o relationinfo.o
relation:RelationManager.cpp 
	g++ -c $^ -o relation.o
message:MessageManager.cpp
	g++ -c $^ -o message.o
photo:PhotoManager.cpp
	g++ -c $^ -o photo.o
bus:BusManager.cpp 
	g++ -c $^ -o bus.o
db:mydb.cpp
	g++ -c $^ -o db.o 

server:1.server.c UserManager.cpp RelationManager.cpp RelationInfo.cpp MessageManager.cpp PhotoManager.cpp mydb.cpp BusManager.cpp 
	@#g++ -o $@ $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL) $(OBJ_SSP) $^ 
	@g++ $(OBJ_SSP) $^ -o $@ $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL)
client:2.client.c
	@g++ $(OBJ_SSP) $^ -o client $(LIB_PROTOBUF) 
project:3.server.c
	g++ $(OBJ_SSP) $(OBJ_THREAD) $^ -o $@ $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL) $(LIB_THREAD) -D _D

.SXG:clean
clean:
	rm -f *.o server
