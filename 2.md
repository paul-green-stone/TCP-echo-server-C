# Basic TCP Socket

## Creating and Destroying Sockets

To communicate using TCP or UDP, a program begins by asking the operating system to create an instance of the socket abstraction.

```C
int socket(int domain, int type, int protocol);
```

The first parameter determines the communication *domain* of the socket.

The second parameter specifies the *type* of the socket. The type determines the semantics of data transmission with the socket - for example, whether transmission is reliable, whether message boundaries are preserved, and so on.

The third parameter specifies the particular *end-to-end* protocol to be used. For both IPv4 and IPv6, we want TCP (identified by the constant `IPPROTO_TCP`) for a stream socket, or UDP (identified by `IPPROTO_UDP`) for a datagram socket. Supplying the constant 0 as the third parameter causes the system to select the *default* end-to-end protocol for the specified protocol family and type.

When an application is finished with a socket, it calls `close`, giving the descriptor for the socket that is no longer needed.

```C
int close(int socket);
```

## Specifying Addresses

Applications using sockets need to be able to identify the remote endpoint(s) with which they will communicate.

### Generic Addresses

```C
struct sockaddr {
    /* Address family (e.g., AF_INET) */
    sa_family_t sa_family;
    
    /* Family-specific address information */
    char sa_data[14];
}
```

The second part is a blob of bits whose exact form depends on the address family.

### IPv4 Addresses

```C
struct in_addr {
    /* Internet address (32 bits) */
    uint32_t s_addr;
}
```

```C
struct sockaddr_in {
    /* Internet protocol (AF_INET) */
    sa_family_t sin_family;
                    
    /* Address port (16 bits) */
    in_port_t sin_port;
                    
    /* IPv4 address (32 bits) */
    struct in_addr sin_addr;
                    
    /* Not used */
    char sin_zero[8];
}
```

### IPv6 Addresses

```C
struct in_addr {
    /* Internet address (32 bits) */
    uint32_t s_addr[16];
}
```

```C
struct sockaddr_in6 {
    /* Internet protocol (AF_INET) */
    sa_family_t sin6_family;
                    
    /* Address port (16 bits) */
    in_port_t sin6_port;
    
    /* Flow information */
    uint32_t sin6_flowinfo;
                    
    /* IPv4 address (128 bits) */
    struct in6_addr sin6_addr;
                    
    /* Scope identifier */
    uint32_t sin6_scope_id;
}
```

### Generic Address Storage

The `sockaddr_storage` structure, which is guaranteed to be as large as any supported address type.

```C
struct sockaddr_storage {
    sa_family_t;
    
    /* ... */
    
    /* Padding and fields to get correct length and alignment */
    
    /* ... */
};
```

### Binary/String Address Conversion

For socket functions to understand addresses, they must be in “numeric” (i.e., binary) form; however, addresses for human use are generally “printable” strings (e.g., 192.168.1.1 or 1::1). We can convert addresses from printable string to numeric using the `inet_pton` function (**pton** = **p**rintable **to** *n*umeric):

```C
int inet_pton(int addressFamily, const char* src, void* dest);
```

The first parameter, *addressFamily*, specifies the address family of the address being converted. The *src* parameter references a null terminated character string containing the address to convert. The *dst* parameter points to a block of memory in the caller’s space to hold the result; its length must be sufficient to hold the result (at least 4 bytes for IPv4 and 16 bytes for IPv6).

To go the other way, converting addresses from numeric to printable form, using `inet_ntop` (**ntop** = **nu**meric **to** **p**rintable):

```C
const char *inet_ntop(int addressFamily, const void* src, char* dst, socklen_t dstBytes);
```

The first parameter, *addressFamily*, specifies the type of the address being converted. The second parameter *src* points to the first byte of a block of memory containing the numeric address to convert.

### Getting a Socket’s Associated Addresses

The system associates a local and foreign address with each connected socket. To find out these addresses
using `getsockname` for the local address and `getpeername` for the foreign address. Both methods return a `sockaddr` structure containing the Internet address and port information.

```C
int getpeername(int socket, struct sockaddr* remoteAddress, socklen_t* addressLength);

int getsockname(int socket, struct sockaddr* localAddress, socklen_t* addressLength);
```

## Connecting a Socket

```C
int connect(int socket, const struct sockaddr* foreignAddress, socklen_t addressLength);
```

## Binding to an Address

```C
int bind(int socket, struct sockaddr* localAddress, socklen_t addressSize);
```

It is important to realize that it is not possible for a program to bind a socket to an *arbitrary* Internet address — if a specific Internet address is given (of either type), the call will only succeed if that address is assigned to the host on which the program is running. A server on a host with multiple Internet addresses might bind to a specific one because it *only wants to accept connections that arrive to that address*. Typically, however, the server wants to accept connections sent to *any* of the host’s addresses, and so sets the address part of the sockaddr to the “wildcard” address `INADDR_ANY` for IPv4 or `in6addr_any` for IPv6. The semantics of the wildcard address are that it matches any specific address. For a server, this means that it will receive connections addressed to any of the host’s addresses (of the specified type).

## Handling Incoming Connections

```C
int listen(int socket, int queueLimit);
```

The `listen` function causes internal state changes to the given socket, so that incoming TCP connection requests will be processed and then queued for acceptance by the program.

Once a socket is configured to listen, the program can begin accepting client connections on it. The socket that has been bound to a port and marked “listening” is never actually used for sending and receiving. Instead, it is used as a way of getting *new* sockets, one for each client connection; the server then sends and receives on the *new* sockets.

```C
int accept(int socket, struct sockaddr* clientAddress, socklen_t* addressLength);
```

This function dequeues the next connection on the queue for *socket*. If the queue is empty, `accept` blocks until a connection request arrives.

**A common beginner mistake is to fail to initialize the integer that addressLength points to so it contains the length of the structure that clientAddress points to**. The following shows the correct way:

```C
struct sockaddr_storage address;
socklen_t addrLength = sizeof(address);
int newConnection = accept(sock, &address, &addrLength);
```

## Communication

Once a socket is “connected,” you can begin sending and receiving data. Through a connected TCP socket, you can communicate using `send` and `recv`.

```C
ssize_t send(int socket, const void* msg, size_t msgLength, int flags);

ssize_t recv(int socket, void* rcvBuffer, size_t bufferLength, int flags);
```

The first parameter *socket* is the descriptor for the connected socket through which data is to be sent or received. For `send`, *msg*
points to the sequence of bytes to be sent, and *msgLength* is the number of bytes to send. For `recv`, *rcvBuffer* points to the buffer - that is, an area in memory such as a character array - where received data will be placed, and *bufferLength* gives the length of the buffer, which is the maximum number of bytes that can be received at once.

Remember: TCP is a byte-stream protocol, so `send` boundaries are not preserved. **The number of bytes read in a single call to `recv` on the receiver is not necessarily determined by the number of bytes written by a single call to `send`**. A common mistake when writing TCP socket applications involves assuming that if you write all of the data with one `send` you can read it all with one `recv`.