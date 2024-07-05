#ifndef MISC_H
#define MISC_H

/* ================================================================ */

int write_msg(int sock, const char* msg);

/* ================================ */

int read_msg(int sock, char* buffer, ssize_t buf_size);

/* ================================================================ */

#endif /* MISC_H */
