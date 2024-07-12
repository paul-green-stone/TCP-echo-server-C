#include "../network.h"

#define MAXPENDING 5

/* ================================================================ */

int main(int argc, char** argv) {
    
    /* operation status/return value */
    int op_status;
    
    int sock;
    
    char* service;
    
    struct addrinfo hints;
    struct addrinfo* server_address;
    
    const char* msg = "The server just said \"Hey!\"";
    
    char buffer[BUFSIZ];
    
    /* ================ */
    
    /* ================================================================ */
    /* ================ Handling command line arguments =============== */
    /* ================================================================ */
    
    if (argc != 2) {
        fprintf(stdout, "usage: %s [<Server Port>]\n", argv[0]);
        
        /* ======== */
        return EXIT_SUCCESS;
    }
    
    service = argv[1];
    
    /* ================================================================ */
    /* =========== Construct the server address structure ============= */
    /* ================================================================ */
    
    memset(&hints, 0, sizeof(hints));
    
    /* Any address family */
    hints.ai_family = AF_UNSPEC;
    /* Accept on any address/port */
    hints.ai_flags = AI_PASSIVE;
    /* Only stream sockets */
    hints.ai_socktype = SOCK_STREAM;
    /* Only TCP protocol */
    hints.ai_protocol = IPPROTO_TCP;
    
    if ((op_status = getaddrinfo(NULL, service, &hints, &server_address)) != 0) {
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
        /* ================== Bind to the local address =================== */
        /* ================================================================ */
        
        if (bind(sock, address->ai_addr, address->ai_addrlen) == -1) {
            fprintf(stderr, "bind() failed - %s\n", strerror(errno));

            close(sock);

            /* ======== */
            continue ;
        }
        
        /* ================================================================ */
        /* ============== Listen for connections on a socket ============== */
        /* ================================================================ */

        if (listen(sock, MAXPENDING) == -1) {
            fprintf(stderr, "listen() failed - %s\n", strerror(errno));

            close(sock);

            /* ======== */
            continue ;
        }
        
        /* ================================================================ */
        /* ================ Print local address of socket ================= */
        /* ================================================================ */
        
        struct sockaddr_storage local_address;
        socklen_t address_size = sizeof(local_address);
        
        if (getsockname(sock, (struct sockaddr*) &local_address, &address_size) < 0) {
            fprintf(stderr, "getsockname() failed - %s\n", strerror(errno));
            
            freeaddrinfo(server_address);
            
            /* ======== */
            return EXIT_FAILURE;
        }
        
        fprintf(stdout, "Binding to ");
        print_socket_address((struct sockaddr*) &local_address, stdout);
        
        break ;
    }
    
    freeaddrinfo(server_address);
    
    /* ================================================================ */
    /* ======================== Run the server ======================== */
    /* ================================================================ */
    
    for (; ;) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        
        /* ================================================= */
        /* ========= Wait for a client to connect ========== */
        /* ================================================= */
        
        int client_sock = accept(sock, (struct sockaddr*) &client_address, &client_len);
        
        if (client_sock < 0) {
            fprintf(stderr, "accept() failed - %s\n", strerror(errno));
            
            /* ======== */
            continue ;
        }
        
        /* ================================================= */
        /* ====== Output the connected client address ====== */
        /* ================================================= */
        
        fprintf(stdout, "Handling a client :");
        
        print_socket_address((struct sockaddr*) &client_address, stdout);
        
        /* ================================================= */
        /* =========== Send the greeting message =========== */
        /* ================================================= */
        
        write_msg(client_sock, msg);
        
        /* ================================================= */
        /* ======== Read the message the client sent ======= */
        /* ================================================= */
        
        read_msg(client_sock, buffer, BUFSIZ);
        fputs("Client sent: ", stdout);
        fputs(buffer, stdout);
        fputs("\n", stdout);
        
        /* ================================================= */
        /* ============= Echo the message back ============= */
        /* ================================================= */
        
        write_msg(client_sock, buffer);
        
        /* ================================================= */
        /* ============== Delete a descriptor ============== */
        /* ================================================= */

        close(client_sock);
    }
    
    /* ================================================================ */
    /* ===================== Delete a descriptor ====================== */
    /* ================================================================ */
    
    close(sock);
    
    /* ======== */
    
    return EXIT_SUCCESS;
}

/* ================================================================ */


