- [HTTP Protocol Overview](#http-protocol-overview)
  - [§1 History](#1-history)
  - [§2 OSI Model](#2-osi-model)
  - [§3 TCP (Transmission Control Protocol)](#3-tcp-transmission-control-protocol)
    - [§3.1 TCP 3-Way Handshake Process](#31-tcp-3-way-handshake-process)
  - [§4 HTTP is an application-layer protocol](#4-http-is-an-application-layer-protocol)
  - [§5 HTTP is a client-server protocol](#5-http-is-a-client-server-protocol)
  - [§6 HTTP is a stateless Protocol](#6-http-is-a-stateless-protocol)
  - [§7 HTTP flow (4 steps)](#7-http-flow-4-steps)
  - [&8 HTTP Messages](#8-http-messages)
    - [§8.1 Composition of HTTP Requests](#81-composition-of-http-requests)
      - [§8.1.1 Start line](#811-start-line)
      - [§8.1.2 Header](#812-header)
      - [§8.1.3 Body](#813-body)
    - [§8.2 Composition of HTTP Responses](#82-composition-of-http-responses)
      - [§8.2.1 Status line](#821-status-line)
      - [§8.2.2 Header](#822-header)
      - [§8.2.3 Body](#823-body)
  - [§9 Wireshark](#9-wireshark)
  - [§10 Reference](#10-reference)

# HTTP Protocol Overview

HTTP is a protocol for transmitting hypermedia documents, such as HTML. It is the foundation of any data exchange on the Web.
无连接的 
媒体独立的 任何类型的数据类型都可以发送
无状态的 无连接的结果，跨网页无法保留信息，可以支持更多用户更多链接
三次握手，四次挥手 HTTP1.0 每次传输都得握手非常不好，1.1更搞笑
HTTP 的tcp端口是80
HTTP是一个链接，数据连接和控制链接在一起

## §1 History

![TimelineHTTP](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/TimelineHTTP.png)

## §2 OSI Model

![osi7](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/osi7.png)

## §3 TCP (Transmission Control Protocol)

TCP (Transmission Control Protocol) is an important network protocol that lets two hosts connect and exchange data streams. TCP guarantees the delivery of data and packets in the same order as they were sent.

TCP provides **reliable communication** with **Positive Acknowledgement with Re-transmission (PAR)**. The transport layer's Protocol Data Unit (PDU) is called a segment. A device using PAR resends the data unit until it receives an acknowledgment. If the data unit received at the receiver’s end is damaged, the receiver discards the segment. So the sender has to resend the data unit for which positive acknowledgment is not received.

### §3.1 TCP 3-Way Handshake Process

In this TCP handshake process, a client needs to initiate the conversation by requesting a communication session with the server.

![20220325150312](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220325150312.png)

Step 1: The client establishes a connection with a server in the first step. It sends a segment with **SYN** and informs the server that the client should start communication and with what should be its sequence number.

Step 2: The server responds to the client request with the **SYN-ACK** signal set. ACK helps signify the response of the received segment, and the SYN represents what sequence number it should be able to start with the segments.

Step 3: In the final step, the client acknowledges the response of the server, and they both create a stable connection that will begin the actual data transfer process.

## §4 HTTP is an application-layer protocol

![20220325140244](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220325140244.png)
HTTP is an application layer protocol sent over TCP or a TLS-encrypted TCP connection, though any **reliable transport protocol** could theoretically be used.

## §5 HTTP is a client-server protocol

HTTP is a client-server protocol: requests are sent by one entity, the user-agent (or a proxy on behalf of it). The user-agent is a Web browser most of the time, but it can be anything. In the client-server model, the client opens a connection to make a request; then, it waits until it receives a response.

Between the client and the server, there are numerous entities, collectively called proxies, which perform different operations and act as gateways or caches, for example. Thanks to the layered design of the Web, these are hidden in the network and transport layers.

![20220325153051](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220325153051.png)

## §6 HTTP is a stateless Protocol

HTTP is a **stateless** protocol, meaning that the server does not keep any data (state) between two requests. With the HTTP protocol, a new response is generated every time a new request is sent. The protocol itself does not keep information about all previous requests or response messages, which is for dealing with a large number of transactions faster.

In some cases, statelessness can make HTTP inconvenient. For example, if a user logs in to a shopping site, one needs to be able to stay logged in even after jumping to another page on the site. In order to achieve the desired status-keeping function, cookie technology was introduced. With cookies, it is possible to maintain the login state while using the HTTP protocol.

## §7 HTTP flow (4 steps)

1. Open a TCP connection: The TCP connection is used to send a request, or several, and receive an answer. The client may open a new connection, reuse an existing connection, or open several TCP connections to the servers.
2. Send an **HTTP message**: HTTP messages (before HTTP/2) are human-readable. With HTTP/2, these simple messages are encapsulated in frames, making them impossible to read directly, but the principle remains the same.
3. Read the response sent by the server.
4. Close or reuse the connection for further requests.

## &8 HTTP Messages

HTTP messages are how data is exchanged between a server and a client. There are two types of messages: **requests** sent by the client to trigger an action on the server, and **responses**, the answer from the server.

HTTP messages are composed of textual information encoded in ASCII, and span over multiple lines. In HTTP/1.1, and earlier versions of the protocol, these messages were openly sent across the connection. In HTTP/2, the once human-readable message is now divided up into HTTP frames, providing optimization and performance improvements.

HTTP requests, and responses, share similar structure and are composed of:

1. A **start-line** describing the requests to be implemented, or its status of whether successful or a failure. This start-line is always a single line.
2. An optional set of **HTTP headers** specifying the request, or describing the body included in the message.
3. A **blank line** indicating all meta-information for the request has been sent.
4. An optional **body** containing data associated with the request (like content of an HTML form), or the document associated with a response. The presence of the body and its size is specified by the start-line and HTTP headers.

![20220325155228](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220325155228.png)

### §8.1 Composition of HTTP Requests

#### §8.1.1 Start line

HTTP requests are messages sent by the client to initiate an action on the server. Their start-line contain three elements:

1. An **HTTP method** describing the action to be performed;
2. The **request target**, usually a URL;
3. The **HTTP version** defining the structure of the remaining message.

#### §8.1.2 Header

The **basic structure** of an HTTP header:

1. **A case-insensitive string** followed by a colon ":"
2. **A value** whose structure depends upon the header.

#### §8.1.3 Body

Not all requests have a body. Some requests sending data to the server need a body.

### §8.2 Composition of HTTP Responses

#### §8.2.1 Status line

The start line of an HTTP response, called the status line, contains the following information:

1. The **protocol version**;
2. A **status code**, indicating success or failure of the request;
3. A **status text**, a brief description of the status code.

#### §8.2.2 Header

Same as the request header, the response header follows the basic header structure as well:

1. A **case-insensitive string** followed by a colon ":"
2. A **value** whose structure depends upon the header.

#### §8.2.3 Body

Not all responses have one: responses with a status code that sufficiently answers the request without the need for corresponding payload (like 201 Created or 204 No Content) usually don't.

## §9 Wireshark

1. Open Wireshark and choose the desired Internet interface
2. Open browser and go to a desired website
3. Use the filter "http.request.method==GET", because GET can always be the beginning of an HTTP request.
4. Right click the entry, choose "follow->TCP stream"

具体分析见实验报告

## §10 Reference

Developer.mozilla.org. 2022. HTTP | MDN. [online] Available at: <https://developer.mozilla.org/en-US/docs/Web/HTTP> [Accessed 25 March 2022].
Blogjava.net. 2022. 深入理解HTTP协议（转） - 苏勇的blog - BlogJava. [online] Available at: <http://www.blogjava.net/zjusuyong/articles/304788.html> [Accessed 25 March 2022].
Stevens, W. and Wright, G., 2000. TCP/IP illustrated. Reading, Mass.: Addison-Wesley Pub. Co.
Developer.mozilla.org. 2022. HTTP | MDN. [online] Available at: <https://developer.mozilla.org/en-US/docs/Web/HTTP> [Accessed 25 March 2022].
Developer.mozilla.org. 2022. HTTP | MDN. [online] Available at: <https://developer.mozilla.org/en-US/docs/Web/HTTP> [Accessed 25 March 2022].
