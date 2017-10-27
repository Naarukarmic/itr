#include <time.h>
#include "isocket.h"
#include "threadpool.h"

#define BUF_SIZE 500

double latency(void (*request)()) {
    clock_t begin = clock();
    request();
    clock_t end = clock();
    double time_spent = (double) (end - begin)/CLOCKS_PER_SEC;
    return time_spent;
}

void connect_get() {
    int cnt, m, size = BUF_SIZE;
    char buf[BUF_SIZE];
    char* req = "GET / HTTP/1.0 \n\n";

    int sock = i_socket();
    char* host = "localhost";
    int port = 1234;

    if(i_connect(sock, host, port) < 0) {
       printf("Connection refused\n");
       exit(1);
    }

    send(sock, req, sizeof(req), 0);
    sleep(0.3);
    
    m = 0;
    while((cnt=read(sock, buf, size)) > 0) {
       m += cnt;
       if(m >= 12) break;
    }

    close(sock);
}

void periodic_client(int n) {
    pthread_pool_t *thread_pool = malloc(sizeof(thread_pool));
    if ((thread_pool = pthread_pool_init(n)) < 0)
        perror ("ERROR: failed to create the thread pool, ");
    printf ("\x1B[32m %s \x1B[0m%s", "[MAIN]", "Start processing\n");
    sleep(1);
    double total_lat = 0.0;
    for(int i = 0; i < n; i++)
    {
        total_lat += latency(&connect_get);
    }

    sleep(2);
    printf("%f\n", total_lat);
}

void aperiodic_client(int n) {

}

int main(int argc, char **argv) {
    if (argc != 3)
    {
        printf("    Usage : web_client SCENARIO nCLIENT\n"
            "       SCENARIO = 1 (periodic) or 2 (aperiodic)\n"
            "       nCLIENT = n (number of clients)\n");
        return -1;
    }
    switch(atoi(argv[1])){
            case 1:
                periodic_client(atoi(argv[2]));
                break;
            case 2:
                aperiodic_client(atoi(argv[2]));
                break;
        }
    return 0;
}