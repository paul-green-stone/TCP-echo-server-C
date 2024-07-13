# Of Names and Address Families

## Mapping Names to Numbers

A host's Internet address is tied to the part of the network to which it is connected.

To solve the problem, most implementations of the Sockets API provide access to a *name service* that maps names to other information, including Internet addresses. Names for services (e.g., echo) can also be mapped to port numbers. The process of mapping a name to a numeric quantity (address or port number) is called *resolution*.

It is critical to remember that **a name service is not required for TCP/IP to work**. Two of the primary sources are the *Domain Name System* (DNS) and local configuration databases. The DNS is a distributed database that maps *domain names* to Internet addresses and other information. Local configuration databases are generally operating-system-specific mechanisms for name-to-Internet-address mappings. Fortunately for the programmer, the details of how the name service is *implemented* are hidden behind the API, so the only thing we need to know is how to ask it to *resolve* a name.

### Accessing the Name Service

```C
int getaddrinfo(const char* hostStr, const char* serviceStr, const struct addrinfo* hints, struct addrinfo** results);
```

`getaddrinfo` creates a dynamically allocated linked list of results, which must be deallocated after the caller is finished with the list. The method **should only be called when the program is finished with the returned information**; no information contained in the list of results is reliable after this function has returned.

```C
void freeaddrinfo(struct addrinfo* addrList);
```

In case `getaddrinfo` returns a nonzero (error) value, passing it to `gai_strerror` yields a string that describes what went wrong.

```C
const char* gai_strerror(int errorCode);
```

Each entry in the linked list is placed into an addrinfo structure, declared as follows:

```C
struct addrinfo {
    int ai_flags;               // Flags to control info resolution
    int ai_family;              // Family: AF_INET, AF_INET6, AF_UNSPEC
    int ai_socktype;            // Socket type: SOCK_STREAM, SOCK_DGRAM
    int ai_protocol;            // Protocol: 0 (default) or IPPROTO_XXX
    socklen_t ai_addrlen;       // Length of socket address ai_addr
    struct sockaddr* ai_addr;   // Socket address for socket
    char* ai_canonname;         // Canonical name
    struct addrinfo* ai_next;   // Next addrinfo in linked list
};
```

### Details

The third argument (**addrinfo** structure) tells the system what kinds of endpoints the caller is interested in. The `ai_flags` field in the third parameter provides additional control over the behavior of `getaddrinfo`.

* **AI_PASSIVE** If *hostStr* is NULL when this flag is set, any returned **addrinfo**s will have their addresses set to the appropriate “any” address constant.

* **AI_CANONNAME** One name is usually defined to be the official (“canonical”) name. By setting this flag in `ai_flags`, we instruct `getaddrinfo` to return a pointer to the canonical name (if it exists) in the **ai_canonname** field of the first **struct addrinfo** of the linked list.

* **AI_NUMERICHOST** This flag causes an error to be returned if *hostStr* does not point to a string in valid numeric address format.

* **AI_ADDRCONFIG** If set, `getaddrinfo` returns addresses of a particular family only if the system has an interface configured for that family. So an IPv4 address would be returned only if the system has an interface with an IPv4 address, and similarly for IPv6.

* **AI_V4MAPPED** If the *ai_family* field contains *AF_INET6*, and no matching IPv6 addresses are found, then `getaddrinfo` returns IPv4-mapped IPv6 addresses.

## Getting Names from Numbers

There is an “inverse” function called `getnameinfo`, which takes a sockaddr address structure (really a struct `sockaddr_in` for IPv4 and struct `sockaddr_in6` for IPv6) and the address length. The function returns a corresponding node and service name in the form of a null-terminated character string — if the mapping between the number and name is stored in the name system.

```C
int getnameinfo(const struct sockaddr* address, socklen_t addressLength, char* node, socklen_t nodeLength, char* service, socklen_t serviceLength, int flags);
```

As with `getaddrinfo`, several flags control the behavior of function.

* **NI_NOFQDN** Return only the hostname, not FQDN (Fully Qualified Domain Name), for local hosts.

* **NI_NUMERICHOST** Return the numeric form of the address instead of the name. This avoids potentially expensive name service lookups if you just want to use this service as a substitute for `inet_ntop`.

* **NI_NUMERICSERV** Return the numeric form of the service instead of the name.

* **NI_NAMEREQD** Return an error if a name cannot be found for the given address. Without this option, the numeric form of the address is returned.

* **NI_DGRAM** Specifies datagram service; the default behavior assumes a stream service. In some cases, a service has different port numbers for TCP and UDP.

What if your program needs its own host’s name?

```C
int gethostname(char* nameBuffer, size_t bufferLength);
```