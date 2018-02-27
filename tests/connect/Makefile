.PHONY: all
all: test_mysql_connect

.PHONY: clean
clean:
	rm -f *.o test_mysql_connect

.PHONY: run
run: all
	./test_mysql_connect

test_mysql_connect: test_mysql_connect.o
	gcc -L/usr/local/mysql/lib -o $@ $^ -lmysqlclient

%.o: %.c
	gcc -I/usr/local/mysql/include -c -o $@ $<
