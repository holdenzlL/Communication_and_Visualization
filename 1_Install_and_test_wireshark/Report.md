- [Report](#report)
  - [§1 4-step HTTP flow](#1-4-step-http-flow)
  - [§2 All filter results for HTTP](#2-all-filter-results-for-http)

# Report
## §1 4-step HTTP flow

For this experiment of HTTP, I choose the official website of the embassy of the People's Republic of China in the Federal Republic of Germany because it is a website without using the HTTPS protocol.

With the help of the "Follow the TCP steam" functionality, I filtered out the TCP steam as follow:

![asdasdasd](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/asdasdasd.png)

From the screenshot shown above, I can see that there was a TCP 3-way handshake process before the HTTP request.
The request has a structure as follows:

1. A starter line
2. HTTP headers
3. A blank line

In the HTTP headers, I can find information such as

1. The Host is de.china-embassy.org
2. The connection support "keep-alive."
3. My browser's some features, such as supporting "Upgrade-insecure-requests", "Accept-Language" and "Accept-Encoding"

4. HTTP request nine contents in total, and this is the first content.
5. The response from the host is at the frame 3991 and the request (2/9) is in the frame 3994

Then I went to the frame 3991, and got a screenshot as shown below:
![asdfasdfds](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/asdfasdfds.png)
From the screenshot shown above, I can see a response from the host and another request from the User-agent. Between the two requests from the User-agent, there is no close of the TCP connection because the protocol HTTP 1.1 is used so that the efficiency is hereby got improved.

The response has a structure as follows:
1. A status line including
   a. The protocol version HTTP/1.1;
   b. A status code 200, indicating the success of the request;
   c. A status text, OK, a brief description of the status code.
2. Headers, including
    a. the requested content has a type of text/html
    b. transfer coding information and so on
3. A blank line

Go to the very end of this TCP stream; I can see a close of connection via TCP 4-way handshake, which is shown below:
![asdiclnadhjs](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/asdiclnadhjs.png)

## §2 All filter results for HTTP

![uxkasjbdn](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/uxkasjbdn.png)