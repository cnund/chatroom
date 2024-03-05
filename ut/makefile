object=list.o server.o database.o thread.o main.o 
app:$(object)
	gcc -o app $(object) -lsqlite3 -lpthread
%.o:%.c
	gcc -c $< -o $@
clean:
	rm -rf *.o app