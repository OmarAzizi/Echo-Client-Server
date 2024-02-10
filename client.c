#include "common.h"

void str_cli(FILE*, int);

int main(int argc, char** argv) {
/*
    Steps:

    1- Create a socket for the client
    2- Fill in internet socket address structure (for the server)
    3- Connect to the server
*/
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    
    if (argc < 2)
        handle_errors("USAGE: tcpcli <IPAddress>\n");
    
    /* Step 1 */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        handle_errors("ERROR => socket error.\n"); 
   
    /* Step 2 */
    bzero(&servaddr, servlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 1)
        handle_errors("ERROR => inet_pton error.\n");
    
    /* Step 3 */
    if (connect(sockfd, (SA*)&servaddr, servlen) == -1)
        handle_errors("ERROR => connect error.\n");

    str_cli(stdin, sockfd);

    return EXIT_SUCCESS;
} 

void str_cli(FILE* stream, int socket_fd) {
/*
    handles the client processing loop: It reads a line of text from 
    standard input, writes it to the server, reads back the server’s 
    echo of the line, and outputs the echoed line to standard output.
*/
    char sendline[MAXLINE], recvline[MAXLINE]; // 4Kb buffers
    
    while (fgets(sendline, MAXLINE, stream) != NULL) { // reads a line from standard input
        if (write(socket_fd, sendline, strlen(sendline)) == -1) // sends it to the server
            handle_errors("ERROR => write error.\n");
        
        bzero(recvline, sizeof(recvline));
        // reads the line echoed back from the server
        if (read(socket_fd, recvline, MAXLINE) == 0)
            handle_errors("ERROR => Server terminated prematurely.\n");
        
        // writes the line echoed back from the server to standard output 
        printf("Server Response: ");
        if (fputs(recvline, stdout) < 0)
            handle_errors("ERROR => fputs error.\n");

    }
    
/*
    return to main when 'fgets' get a NULL pointer, which occur when it encouters:
    1- End-Of-File (EOF)
    2- Error
*/
}
