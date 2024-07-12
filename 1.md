# Introduction

The application programming interface (API) known as ***Sockets*** was first developed in C.

## Networks, Packets, and Protocols

A computer network consists of machines interconnected by communication channels. We call these machines ***hosts*** and ***routers***. Hosts are computers that run applications uch as your Web browser. Routers (also called ***gateways***) are machines whose job is to relay, or ***forward***, information from one communication channel to another. For our purposes, a ***communication channel*** is a means of conveying sequences of bytes from one host to another.

---

By ***information*** we mean sequences of bytes that are constructed and interpreted by programs. In the context of computer networks, these byte sequences are generally called ***packets***. A packet contains control information that the network uses to do its job and sometimes also includes user data.

---

A ***protocol*** is an agreement about the packets exchanged by communicating programs and what they mean.

---

The Internet Protocol provides a ***datagram*** service: every packet is handled and delivered by the network independently. To make this work, each IP packet has to contain the ***address*** of its destination.

The layer above IP is called the ***transport*** layer. It offers a choice between two protocols: TCP and UDP. Both TCP and UDP use addresses, called ***port numbers***, to identify applications within hosts. TCP and UDP are called ***end-to-end transport protocols*** because they carry data all the way from one program to another (whereas IP only carries data from one host to another).

TCP provides a ***reliable byte-stream*** channel, so that applications do not have to deal with these problems. It is a ***connection-oriented*** protocol: before using it to communicate, two programs must first establish a TCP connection, which involves completing an exchange of ***handshake messages*** between the TCP implementations on the two communicating computers.

## About Addresses

Before a program can communicate with another program, it must tell the network something to identify the other program. In TCP/IP, it takes two pieces of information to identify a particular program: an ***Internet address***, used by IP, and a ***port number***, the additional address interpreted by the transport protocol (TCP or UDP).

Internet addresses are binary numbers. hey come in two flavors, corresponding to the two versions of the Internet Protocol that have been standardized. The most common is version 4 (***IPv4***); the other is version 6 (***IPv6***). IPv4 addresses are 32 bits long; IPv6 addresses
are 128 bits long.

### Writing Down IP Addresses

IPv4 addresses are conventionally written as a group of four decimal numbers separated by periods (e.g., 10.1.2.3); this is called the ***dotted-quad*** notation.

The 16 bytes of an IPv6 address, on the other hand, by convention are represented as groups of hexadecimal digits, separated by colons (e.g., 2000:fdb8:0000:0000:0001:00ab:853c:39a1). Each group of digits represents 2 bytes of the address; leading zeros may be omitted.

---

Technically, each Internet address refers to the connection between a host and an underlying communication channel — in other words, a ***network interface***. A host may have several interfaces; it is not uncommon, for example, for a host to have connections to both wired (Ethernet) and wireless (WiFi) networks.

### Dealing with Two Versions

To smooth the transition from IPv4 to IPv6, most systems are ***dual-stack***, simultaneously supporting both IPv4 and IPv6.

IPv4 addresses can be mapped into IPv6 addresses using ***IPv4 mapped addresses***. An IPv4 mapped address is formed by prefixing the four bytes in the IPv4 address with ::fff. For example, the IPv4 mapped address for 132.3.23.7 is ::ffff:132.3.23.

### Port Numbers

We mentioned earlier that it takes *two* pieces of address to get a message to a program. The port number in TCP or UDP is always interpreted *relative* to an Internet address. Port numbers are the same in both IPv4 and IPv6: 16-bit unsigned binary numbers.

### Special Addresses

In each version of IP, certain special-purpose addresses are defined. One of these that is worth knowing is the ***loopback address***, which is always assigned to a special ***loopback interface***, a virtual device that simply echoes transmitted packets right back to the sender. The loopback address for IPv4 is 127.0.0.1; 1 for IPv6 it is 0:0:0:0:0:0:0:1 (or just ::1).

---

Another group of IPv4 addresses reserved for a special purpose includes those reserved for “private use.” This group includes all IPv4 addresses that start with 10 or 192.168, as well as those whose first number is 172 and whose second number is between 16 and 31. These addresses were originally designated for use in private networks that are not part of the global Internet. Today they are often used in homes and small offices that are connected to the Internet through a ***network address translation*** (NAT) device. Such a device acts like a router that translates (rewrites) the addresses and ports in packets as it forwards them. More precisely, it maps (private address, port) pairs in packets on one of its interfaces to (public address, port) pairs on the other interface. This enables a small group of hosts (e.g., those on a home network) to effectively “share” a single IP address. The importance of these addresses is that ***they cannot be reached from the global Internet***.

---

A related class contains the ***link-local***, or “autoconfiguration” addresses. For IPv4, such addresses begin with 169.254. For IPv6, any address whose first 16-bit chunk is FE80, FE90, FEA0, or FEB0 is a link-local address. These addresses can only be used for communication between hosts connected to the same network; routers will not forward packets that have such addresses as their destination.

---

Another class consists of ***multicast*** addresses. Whereas regular IP (sometimes called “unicast”) addresses refer to a single destination, multicast addresses potentially refer to an arbitrary number of destinations. In IPv4, multicast addresses in dotted-quad format have a first number in the range 224 to 239. In IPv6, multicast addresses start with FF.

## About Names

Most likely you are accustomed to referring to hosts by name (e.g., host.example.com). However, the Internet protocols deal with addresses (binary numbers), not names. When you use a name to identify a communication end point, the system does some extra work to ***resolve*** the name into an address. Names are obviously easier for humans to remember than dotted-quads (or, in the case of IPv6, strings of hexadecimal digits). Names provide a level of indirection, which insulates users from IP address changes.

The name-resolution service can access information from a wide variety of sources. Two of the primary sources are the ***Domain Name System*** (DNS) and local configuration databases. The DNS is a distributed database that maps ***domain names*** to Internet addresses and other information; the DNS protocol allows hosts connected to the Internet to retrieve information from that database using TCP or UDP. Local configuration databases are generally OS-specific mechanisms for local name-to-Internet address mappings.

## Clients and Servers

The client program initiates communication, while the server program waits passively for and then responds to clients that contact it.
Together, the client and server compose the ***application***. The terms ***client*** and ***server*** are descriptive of the typical situation in which the server makes a particular capability — for example, a database service — available to any client able to communicate with it.

Whether a program is acting as a client or server determines the general form of its use of the Sockets API to establish communication with its ***peer***. (The client is the peer of the server and vice versa.)

How does a client find out a server’s IP address and port number? Usually, the client knows the name of the server it wants—for example, from a ***Universal Resource Locator*** (URL) such as http://www.mkp.com - and uses the name-resolution service to learn the corresponding Internet address

You may have heard of an alternative to client-server called peer-to-peer (P2P). In P2P, applications both consume and provide service, unlike the traditional client-server architecture in which servers provide service and clients consume.

## What is a Socket?

A ***socket*** is an abstraction through which an application may send and receive data, in much the same way as an open-file handle allows an application to read and write data to stable storage.

The main types of sockets in TCP/IP today are ***stream sockets*** and ***datagram sockets***. Stream sockets use TCP as the end-to-end protocol (with IP underneath) and thus provide a reliable byte-stream service. Datagram sockets use UDP (again, with IP underneath) and thus provide a best-effort datagram service that applications can use to send individual messages up to about 65,500 bytes in length.