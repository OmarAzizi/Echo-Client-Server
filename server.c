#include "common.h"

void str_echo(int);

int main(int argc, char** argv) {

/*
    Steps: 

    1- Create a socket
    2- Bind the server to a port
    3- Wait for client connection to complete
    4- Concurrent server (fork)
*/
    int listen_fd, connfd;
    pid_t child_pid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    /* Step 1 */
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
        handle_errors("ERROR => socket error.\n");
    
    bzero(&servaddr, sizeof(servaddr));

    /* Step 2 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    if (bind(listen_fd, (SA*)&servaddr, sizeof(servaddr)) == -1) 
        handle_errors("ERROR => bind error.\n");

    /* Step 3 */
    if (listen(listen_fd, MAX_BACKLOG) == -1)
        handle_errors("ERROR => listen error.\n");
    
    /* Step 4 */
    while (true) {
        clilen = sizeof(cliaddr);
        if ( (connfd = accept(listen_fd, (SA*)&cliaddr, &clilen)) == -1 )
            handle_errors("ERROR => accept error.\n");
        
        if ( (child_pid = fork()) == -1) 
            handle_errors("ERROR => error creating child process.\n");
    
        if (child_pid == 0) {
            if (close(listen_fd) == -1)
                handle_errors("ERROR => close error.\n");
            str_echo(connfd);
            exit(EXIT_SUCCESS);
        }
        if (close(connfd) == -1)
            handle_errors("ERROR => close error.\n");
    }
    return EXIT_SUCCESS; 
}

void str_echo(int socket_fd) {
/*
    Performs server processing for each client: It reads
    data coming from the client and echoes it back.
*/
    ssize_t n; // will hold the size of read data
    char buffer[MAXLINE]; // 4Kb buffer
    
    again:
        while ( (n = read(socket_fd, buffer, MAXLINE)) > 0) {
            if (write(socket_fd, buffer, n) == -1 )
                handle_errors("ERROR => write error.\n");
            
            if (fputs(buffer, stdout) < 0)
                handle_errors("ERROR => fputs error.\n");

            bzero(&buffer, sizeof(buffer));
        }

        if (n < 0 && errno == EINTR) goto again;
        else if (n < 0) handle_errors("ERROR => read error.\n");
}
