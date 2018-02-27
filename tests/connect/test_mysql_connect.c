#include <stdio.h>
#include <stdlib.h>

#include <mysql.h>
#include <sys/time.h>


const char* getenv_default(const char* name, const char* def) {
    const char *value = getenv(name);

    if (value == NULL)
        return def;

    return value;
}


MYSQL* connect_to_mysql() {
    const char *hostname = getenv_default("MYSQL_HOST", "localhost");
    const char *username = getenv_default("MYSQL_USER", "root");
    const char *password = getenv_default("MYSQL_PASSWORD", "rootpw");

    MYSQL *mysql = mysql_init(NULL);
    mysql_real_connect(
        mysql,
        hostname,
        username,
        password,
        NULL,
        3306,
        NULL,
        CLIENT_COMPRESS
    );

    return mysql;
}


void connection_pool_test(int num_connections) {
    int i;
    MYSQL *connections[num_connections];
    struct timeval tv_start, tv_end, tv_elapsed;

    gettimeofday(&tv_start, NULL);

    for (i = 0; i < num_connections; i++) {
        connections[i] = connect_to_mysql();
    }

    gettimeofday(&tv_end, NULL);
    tv_elapsed.tv_sec = tv_end.tv_sec - tv_start.tv_sec;
    tv_elapsed.tv_usec = tv_end.tv_usec - tv_start.tv_usec;

    if (tv_elapsed.tv_usec < -1) {
        tv_elapsed.tv_sec -= 1;
        tv_elapsed.tv_usec += 1000000;
    }

    printf("Opening %d parallel connections took %ld.%06ld seconds\n", num_connections,
            tv_elapsed.tv_sec, tv_elapsed.tv_usec);

    for (i = 0; i < num_connections; i++) {
        mysql_close(connections[i]);
    }
}

void individual_connection_test(int num_connections) {
    int i;
    MYSQL *connection;
    struct timeval tv_start, tv_end, tv_elapsed;

    gettimeofday(&tv_start, NULL);

    for (i = 0; i < num_connections; i++) {
        connection = connect_to_mysql();
        mysql_close(connection);
    }

    gettimeofday(&tv_end, NULL);
    tv_elapsed.tv_sec = tv_end.tv_sec - tv_start.tv_sec;
    tv_elapsed.tv_usec = tv_end.tv_usec - tv_start.tv_usec;

    if (tv_elapsed.tv_usec < -1) {
        tv_elapsed.tv_sec -= 1;
        tv_elapsed.tv_usec += 1000000;
    }

    printf("Opening %d individual connections took %ld.%06ld seconds\n",
            num_connections, tv_elapsed.tv_sec, tv_elapsed.tv_usec);
}


int main(int argc, char* argv[], char* env[]) {
    connection_pool_test(100);
    connection_pool_test(1000);

    individual_connection_test(100);
    individual_connection_test(1000);
}
