#include "../network.h"

#define ECHO_PORT 7

/* ================================================================ */

int main(int argc, char** argv) {
    
    int sock;
    
    /* operation status/return value */
    int op_status;
    
    char* server_IP_address;
    uint16_t server_port;
    
    char* echo_string;
    
    char buffer[BUFSIZ];
    
    /* A server address to connect to */
    struct sockaddr_in server_address;
    
    /* ================ */
    
    /* ================================================================ */
    /* ================ Handling command line arguments =============== */
    /* ================================================================ */
    
    if ((argc < 3) || (argc > 4)) {
        fprintf(stdout, "usage: %s <Server Address> <Echo String> [<Server Port>]\n", argv[0]);
        
        /* ======== */
        return EXIT_SUCCESS;
    }
    
    server_IP_address = argv[1];
    echo_string = argv[2];
    server_port = (argc == 4) ? atoi(argv[3]) : ECHO_PORT;
    
    /* ================================================================ */
    /* ============= Create an endpoint for communication ============= */
    /* ================================================================ */
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket() failed - %s\n", strerror(errno));
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    /* ================================================================ */
    /* ============ Construct the server address structure ============ */
    /* ================================================================ */
    
    memset(&server_address, 0, sizeof(server_address));
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    
    /* Convert IPv4 (and IPv6) addresses from text to binary form */
    op_status = inet_pton(server_address.sin_family, server_IP_address, &server_address.sin_addr.s_addr);
    
    if (op_status == 0) {
        fprintf(stderr, "inet_pton() failed - invalid address string\n");
        
        close(sock);
        
        /* ======== */
        return EXIT_FAILURE;
    }
    else if (op_status == -1) {
        fprintf(stderr, "inet_pton() failed - %s\n", strerror(errno));
        
        close(sock);
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    /* ================================================================ */
    /* ============ Establish the connection to the server ============ */
    /* ================================================================ */
    
    if (connect(sock, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "connect() failed - %s\n", strerror(errno));
        
        close(sock);
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    /* ================================================================ */
    /* =========== Get the greeting message from the server =========== */
    /* ================================================================ */
    
    read_msg(sock, buffer, BUFSIZ);
    
    /* Output it on the screen */
    fputs("Server: ", stdout);
    fputs(buffer, stdout);
    fputs("\n", stdout);
    
    /* ================================================================ */
    /* ============= Send the echo message to the server ============== */
    /* ================================================================ */
    
    write_msg(sock, echo_string);
    
    /* ================================================================ */
    /* ========== Read the message that just got echoed back ========== */
    /* ================================================================ */
    
    read_msg(sock, buffer, BUFSIZ);
    fputs("Server echoed back: ", stdout);
    fputs(buffer, stdout);
    fputs("\n", stdout);
    
    /* ================================================================ */
    /* ===================== Delete a descriptor ====================== */
    /* ================================================================ */
    
    close(sock);
    
    /* ======== */
    
    return EXIT_SUCCESS;
}

/* ================================================================ */

