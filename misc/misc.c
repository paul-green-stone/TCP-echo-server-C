#include "../network.h"

/* ================================================================ */

int write_msg(int sock, const char* msg) {
    
    /* Length of the original message to send */
    ssize_t msg_len = strlen(msg);
    /* */
    uint32_t net_len = htonl((uint32_t) msg_len);
    
    ssize_t bytes_written;
    ssize_t total_bytes_written = 0;
    
    /* ================================================ */
    /* ============ Send a length prefix  ============= */
    /* ================================================ */
    
    bytes_written = write(sock, &net_len, sizeof(net_len));
    
    if (bytes_written == -1) {
        fprintf(stderr, "write() failed - %s\n", strerror(errno));
        
        /* ============ */
        return EXIT_FAILURE;
    }
    
    /* ================================================ */
    /* ========== Send the actual message  ============ */
    /* ================================================ */
    
    while (total_bytes_written < msg_len) {
        bytes_written = write(sock, msg, msg_len);
        
        if (bytes_written == -1) {
            if (errno == EINTR) {
                continue ;
            }
            
            fprintf(stderr, "write() failed - %s\n", strerror(errno));
            
            /* ======== */
            return EXIT_FAILURE ;
        }
        
        total_bytes_written += bytes_written;
    }
    
    /* ======== */
    
    return EXIT_SUCCESS;
}

/* ================================================================ */

int read_msg(int sock, char* buffer, ssize_t buf_size) {
    
    ssize_t total_bytes_read = 0;
    ssize_t bytes_read = 0;
    
    uint32_t net_len;
    uint32_t host_len;
    
    /* ================================================ */
    /* ============ Read a length prefix  ============= */
    /* ================================================ */
    
    bytes_read = read(sock, &net_len, sizeof(net_len));
    
    if (bytes_read == -1) {
        fprintf(stderr, "read() failed - %s\n", strerror(errno));
            
        /* ======== */
        return EXIT_FAILURE;
    }
    
    host_len = ntohl(net_len);
    
    if (host_len >= buf_size) {
        
        host_len = buf_size % host_len;
    }
    
    /* ================================================ */
    /* ========== Read the actual messages  =========== */
    /* ================================================ */
    
    while (total_bytes_read < host_len) {
        bytes_read = read(sock, buffer + total_bytes_read, host_len - total_bytes_read);
        
        if (bytes_read == -1) {
            if (errno == EINTR) {
                continue ;
            }
            
            fprintf(stderr, "read() failed - %s\n", strerror(errno));
            
            /* ======== */
            return EXIT_FAILURE;
        }
        else if (bytes_read == 0) {
            break ;
        }
        
        total_bytes_read += bytes_read;
    }
    
    buffer[total_bytes_read - 1] = '\0';
    
    /* ======= */
    
    return EXIT_SUCCESS;
}

/* ================================================================ */
