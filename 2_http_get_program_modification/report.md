<!-- TOC -->

- [0 Notes](#0-notes)
- [1 Tasks for an HTTP client](#1-tasks-for-an-http-client)
- [2 Code explanation](#2-code-explanation)
  - [Initialization](#initialization)
  - [Parsing URLs](#parsing-urls)
  - [create a socket and connect to the server](#create-a-socket-and-connect-to-the-server)
  - [send request](#send-request)
  - [receive packet](#receive-packet)
  - [postprocessor](#postprocessor)
    - [header extractor](#header-extractor)
  - [second part of the postprocessor](#second-part-of-the-postprocessor)
- [Test program](#test-program)

<!-- /TOC -->

# 0 Notes
1. I tried to modify the code provided by the professor, but I modified too much, and I am very interested in this topic, so I decided to rewrite almost everything and enclose them as a library for later use
2. The code is designed to be able to run on both Windows and UNIX/LINUX systems because when coding, the Winsock and Berkely socket interface are both considered. However, since the programming is done on windows, it hasn't been tried on UNIX/LINUX. So the portability can not be 100% guaranteed, but I will try.
3. One library called "str.h" is from the lecture "Advanced programming" because I have written too much code for processing character strings in C language when I am learning. So this time, I want to do it once and for all. "str.h" is a library for dynamically managing the character string. Since the warmup assignment is not mandatory, the development process is not in a hurry.

# 1 Tasks for an HTTP client
By analyzing the professor's code and doing some research (also studying the source code of wget), I found that the tasks for an HTTP client to download a file remains the same.
0. Initialization (only for Winsock)
1. Parsing a URL
2. create a socket and connect to the server
3. Fabricate a request and send it to the server (normally, it is GET)
4. Receive response
5. Extract the Body from the response based on the Header, this can be done via postprocessor real-time process (process the data and receive the packets at the same time, specifically for chunked data)
6. Clean up the socket, only for Winsock

![20220406001541](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001541.png)

# 2 Code explanation
For each step shown in section 1, I enveloped functions to realize them with the hope that I can maintain them and use them conveniently in the future.

## Initialization
This step is done by function ALLinitialization(); but the initialization is specifically for Microsoft Visual C compiler.

![20220406001612](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001612.png)

## Parsing URLs
This step is done by function URL parse_url(str_p url_origin); since processing URL is another separated library in wget, so I also separate this function into an isolated source file. The parsed URL is stored in a struct. To parse a url, the function will try to find "://" first to separate the protocol and the rest part. If :// is not found, then the protocol will be set to HTTP as the default value. Then the function will try to find : for the port, and / for the site, and # for the end of parsing.

![20220406001637](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001637.png)

## create a socket and connect to the server
This step is done by the function connect2server. This function request the URL struct as the input, and if the connection is constructed, the socket will be returned.
To switch the IPV4 and IPV6 address conveniently in the later, getaddrinfo is used to configure the server info. And to set the time out, the function setsockopt is used. Users can use the Macro _MAX_WAITING_TIME_PER_PACKET to set the timeout of receiving one packet. Since the recv() function is blocking, it is necessary to set the time out.

![20220406001718](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001718.png)

## send request
A request is fabricated and sent by the function send_request. So far, this function only supports sending the easiest request with the command GET.

![20220406001751](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001751.png)

## receive packet
In this program, I use postprocessing to deal with the body and Header of the HTTP response, which means all the response packets will be received and stored in a temporary file. After the receiving part is finished, the function postProcessor(&data, saveFP); will do the post-process the extract the body.
For receiving part, it is done by function recv_response(SOCKET _socket); this function requires the socket as the input and it will return a struct called datapack. In this struct, a file pointer fp is used for operating the temporary file in which all the packet data is stored. This struct also provides information such as headersize, bodysize, headerposition and bodyposition for file handling, and it also has an important bool value "ifchunked" which is important for postprocessing.


## postprocessor
Postprocessing, including extract information from the header and extracting file from body, is done by function postProcessor(datapack* data, FILE* downloadpath). This function takes the datapack struct as the input, and it also takes a file pointer for saving the extracted body.

### header extractor
The function headerExtractor reads in the temporary file line by line, trying to find the blank line, which is the indicator of the separation of the Header from the body. It also tries to read in the first line, the state line, get the HTTP protocol version and the state code. Then the header extractor will also try to find two headers, Transfer-Encoding and Content-Length. If the header Content-Length is found, then the data in the body is not truncated, so the postprocessor will simply save the body without further process. If the transfer-encoding: chunked is detected, then the headerextractor will set the ifChunked flag for the postprocessor.

![20220406001840](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001840.png)

## second part of the postprocessor
The second part of the postprocessor is used to extract the body from the raw data. it first checks if the ifChunked flag is set or not. If the flag is set, then the received data is chunked. It will read the size of the fragment and move the file pointer to process the data.

![20220406001912](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406001912.png)

# Test program
In the Macro, there is a _DEBUG_MODE switch, by opening the switch, there will be more output for printing.

![20220406002043](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406002043.png)

For this test, I choose a picture of Prof. Dr. Westermann on his HTTP webpage :) And the download path is also specified
![20220406002217](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406002217.png)

Based on the Parsed URL, the name of the file will be set automatically by moving the pointer to point the last "/", and the remaining part will be set as the name of the file.

![20220406005144](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406005144.png)

I am not programming an user interface, because I want it to be a general library.

Then, by running the program, we can see the result.

![20220406005510](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406005510.png)

And a file is also saved

![20220406005417](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220406005417.png)