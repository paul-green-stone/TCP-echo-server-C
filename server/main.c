#include "../network.h"

#define MAXPENDING 5

/* ================================================================ */

int main(int argc, char** argv) {
    
    int sock;
    
    uint16_t port;
    
    /* The network address that a server needs to be bound to */
    struct sockaddr_in server_address;
    
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
    
    port = atoi(argv[1]);
    
    /* ================================================================ */
    /* ============= Create an endpoint for communication ============= */
    /* ================================================================ */
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket() failed - %s\n", strerror(errno));
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    /* ================================================================ */
    /* ============== Construct local address structure =============== */
    /* ================================================================ */
    
    memset(&server_address, 0, sizeof(server_address));
    
    server_address.sin_family = AF_INET;
    /* Bind to all interfaces available */
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    
    /* ================================================================ */
    /* ================== Bind to the local address =================== */
    /* ================================================================ */
    
    if (bind(sock, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "bind() failed - %s\n", strerror(errno));
        
        close(sock);
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    /* ================================================================ */
    /* ============== Listen for connections on a socket ============== */
    /* ================================================================ */
    
    if (listen(sock, MAXPENDING) == -1) {
        fprintf(stderr, "listen() failed - %s\n", strerror(errno));
        
        close(sock);
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
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
        
        char client_name[INET_ADDRSTRLEN];
        
        /* Convert IPv4 (and IPv6) addresses from binary to text
       form */
        if (inet_ntop(server_address.sin_family, &client_address.sin_addr.s_addr, client_name, sizeof(client_name)) != NULL) {
            fprintf(stdout, "Handling a client %s:%d\n", client_name, ntohs(client_address.sin_port));
        }
        else {
            fprintf(stdout, "Unable to get a client name\n");
        }
        
        /* ================================================= */
        /* =============== Handle the client =============== */
        /* ================================================= */
        
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


