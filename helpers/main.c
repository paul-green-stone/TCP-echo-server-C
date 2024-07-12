#ifndef HELPERS_H
#define HELPERS_H

#include "../network.h"

/* ================================================================ */

int main(int argc, char** argv) {
    
    /* operation status/return value */
    int op_status;
    
    char* address_str;
    char* port_str;
    
    struct addrinfo* list_of_addresses;
    struct addrinfo hints;
    
    /* ================ */
    
    /* ================================================================ */
    /* ================ Handling command line arguments =============== */
    /* ================================================================ */
    
    if (argc != 3) {
        fprintf(stdout, "usage: %s <Address/Name> <Port/Service>\n", argv[0]);
        
        /* ======== */
        return EXIT_SUCCESS;
    }
    
    address_str = argv[1];
    port_str = argv[2];
    
    /* ================================================================ */
    /* ======= Tell the system what kind(s) of address we want ======== */
    /* ================================================================ */
    
    memset(&list_of_addresses, 0, sizeof(list_of_addresses));
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    /* ================================================================ */
    /* == Get address(es) associated with the specified name/service == */
    /* ================================================================ */
    
    if ((op_status = getaddrinfo(address_str, port_str, &hints, &list_of_addresses)) != 0) {
        fprintf(stderr, "getaddrinfo() failed - %s\n", gai_strerror(op_status));
        
        /* ======== */
        return EXIT_FAILURE;
    }
    
    /* Display returned addresses */
    for (struct addrinfo* address = list_of_addresses; address != NULL; address = address->ai_next) {
        print_socket_address(address->ai_addr, stdout);
    }
    
    freeaddrinfo(list_of_addresses);
}

/* ================================================================ */

#endif /* HELPERS_H */
