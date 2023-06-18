.SSP:all
all: user relationinfo relation message photo server

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

	
server:main.cpp
	g++ -o $@ *.o $^

.SSP:clean
clean:
	rm -f *.o server
