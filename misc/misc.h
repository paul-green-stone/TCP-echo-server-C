#ifndef MISC_H
#define MISC_H

/* ================================================================ */

int write_msg(int sock, const char* msg);

/* ================================================================ */

int read_msg(int sock, char* buffer, ssize_t buf_size);

/* ================================================================ */

void print_socket_address(const struct sockaddr* address, FILE* stream);

/* ================================================================ */

#endif /* MISC_H */
