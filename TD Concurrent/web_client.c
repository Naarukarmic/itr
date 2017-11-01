#include <sys/time.h>
#include "isocket.h"
#include "threadpool.h"

#define BUF_SIZE 500

struct _args{
    int n;
    pthread_mutex_t mutex;
    float lat;
} args;

float latency(void (*request)()) {
    struct timeval begin, end;
    float time_spent;

    gettimeofday(&begin, 0);
    request();
    gettimeofday(&end, 0);

    time_spent = (end.tv_sec - begin.tv_sec) * 1000.0f + (end.tv_usec - begin.tv_usec) / 1000.0f;
    return time_spent;
}

void connect_get() {
    int cnt, m, size = BUF_SIZE;
    char buf[BUF_SIZE];
    char req[32] = "    GET / HTTP/1.1 \r\n";

    int sock = i_socket();
    char* host = "localhost";
    int port = 1234;

    if(i_connect(sock, host, port) < 0) {
        printf("Connection refused\n");
        exit(1);
    }

    send(sock, req, sizeof(req), 0);

    m = 0;
    while((cnt=read(sock, buf, size)) > 0) {
        m += cnt;
        if(m >= 12) break;
    }
    // printf(buf);

    close(sock);
}

void* periodic_client(void* no_args) {
    float client_lat = 0.0;
    int n = args.n;
    
    for(int i = 0; i < n; i++)
    {
        client_lat += latency(&connect_get);
        sleep(0.3);
    }

    pthread_mutex_lock(&args.mutex);
    args.lat += client_lat;
    pthread_mutex_unlock(&args.mutex);
    // printf("Client latency : %.2f ms\n", client_lat);
    pthread_exit(NULL);
}

void periodic_pool(int m) {
    int req_n = m * 20;
    int i = 0;
    pthread_t threads[m];

    args.n = 20;
    pthread_mutex_init(&args.mutex, NULL);
    args.lat = 0.0;

    while(i < m) {
        pthread_create(&threads[i], NULL, &periodic_client, NULL);
        i++;
    }

    i = 0;
    while (i < m) {
        pthread_join(threads[i], NULL);
        i++;
    }
    printf("Requests served per second : %.1f\n"
           "Total latency : %.2f ms\n", req_n/(args.lat * 0.001), args.lat);
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
                periodic_pool(atoi(argv[2]));
                break;
            case 2:
                periodic_pool(atoi(argv[2]));
                break;
        }
    return 0;
}