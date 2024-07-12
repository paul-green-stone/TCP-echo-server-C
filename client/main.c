#include "../network.h"

#define ECHO_PORT 7

/* ================================================================ */

int main(int argc, char** argv) {
    
    int sock = -1;
    
    /* operation status/return value */
    int op_status;
    
    char* host;
    char* service;
    
    char* echo_string;
    
    char buffer[BUFSIZ];
    
    struct addrinfo hints;
    /* A server address to connect to */
    struct addrinfo* server_address;
    
    char name_buffer[128];
    
    /* ================ */
    
    /* ================================================================ */
    /* ================ Handling command line arguments =============== */
    /* ================================================================ */
    
    if ((argc < 3) || (argc > 4)) {
        fprintf(stdout, "usage: %s <Server Address> <Echo String> [<Server Port>]\n", argv[0]);
        
        /* ======== */
        return EXIT_SUCCESS;
    }
    
    /* server address/name */
    host = argv[1];
    /* string to echo */ 
    echo_string = argv[2];
    /* (optional): server port/service */
    service = (argc == 4) ? argv[3] : "echo";
    
    /* ================================================================ */
    /* ===== Tell the system what kind(s) of address info we want ===== */
    /* ================================================================ */
    
    memset(&hints, 0, sizeof(hints));
    
    /* v4 or v6 is OK */
    hints.ai_family = AF_UNSPEC;
    /* Only streaming sockets */
    hints.ai_socktype = SOCK_STREAM;
    /* Only TCP protocol */
    hints.ai_protocol = IPPROTO_TCP;
    
    /* ================================================================ */
    /* ======================= Get address(es) ======================== */
    /* ================================================================ */
    
    if ((op_status = getaddrinfo(host, service, &hints, &server_address)) != 0) {
        fprintf(stderr, "getaddrinfo() failed - %s\n", gai_strerror(op_status));
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    for (struct addrinfo* address = server_address; address != NULL; address = address->ai_next) {
        
        /* ================================================================ */
        /* ============= Create an endpoint for communication ============= */
        /* ================================================================ */
        
        if ((sock = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) == -1) {
            fprintf(stderr, "socket() failed - %s\n", strerror(errno));

            /* ======== */
            continue ;
        }
        
        /* ================================================================ */
        /* ============ Establish the connection to the server ============ */
        /* ================================================================ */
        
        if (connect(sock, address->ai_addr, address->ai_addrlen) == -1) {
            fprintf(stderr, "connect() failed - %s\n", strerror(errno));

            close(sock);

            /* ======== */
            return EXIT_FAILURE;
        }
        
        /* Socket connection succeeded; break and return socket */
        break ;    
    }
    
    freeaddrinfo(server_address);
    
    /* ================================================================ */
    /* ============= Just for fun. Getting the host name ============== */
    /* ================================================================ */
    
    if (gethostname(name_buffer, 128) != 0) {
        fprintf(stderr, "gethostname() failed - %s\n", strerror(errno));
    }
    else {
        fprintf(stdout, "I'm %s\n", name_buffer);
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

