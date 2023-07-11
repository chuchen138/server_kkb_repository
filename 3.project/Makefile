.SXG:all

# protobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf
LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
OBJ_SSP = pb/*.o


# mysql
LIB_MYSQL = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

all: user relationinfo relation message photo db server


user:UserManager.cpp
	g++ -c $^ -o user.o
relationinfo:RelationInfo.cpp
	g++ -c $^ -o relationinfo.o
relation:RelationManager.cpp 
	g++ -c $^ -o relation.o
message:MessageManager.cpp
	g++ -c $^ -o message.o
photo:PhotoManager.cpp
	g++ -c $^ -o photo.omutable
db:mydb.cpp
	g++ -c -o db.o $^

server:1.server.c UserManager.cpp RelationManager.cpp RelationInfo.cpp MessageManager.cpp PhotoManager.cpp mydb.cpp BusManager.cpp 
	@#g++ -o $@ $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL) $(OBJ_SSP) $^ 
	@g++ $(OBJ_SSP) $^ -o $@ $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL)
client:2.client.c
	@g++ $(OBJ_SSP) $^ -o client $(LIB_PROTOBUF) 

maintest:main.cpp
	g++ -o $@ *.o $^

dbtest:mydbtest.c mydb.cpp
	g++ -c mydb.cpp -o db.o
	g++ -o dbtest pb/user_pb.o db.o mydbtest.c $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL)
	rm db.o
prototest:protoTest.cpp pb/message_define.pb.cc
	g++ -o proto protoTest.cpp pb/message_define.pb.cc $(INC_PROTOBUF) $(LIB_PROTOBUF)

.SXG:clean
clean:
	rm -f *.o server
