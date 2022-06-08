#include "httpclient.h"

void perr_exit(const char* msg, int ret_code)
{
	fprintf(stderr, "%s  Error code: %d\n", msg, ret_code);
	exit(ret_code);
}

inline time_t _difftime(time_t start, time_t end)
{
	return (start - end) / CLOCKS_PER_SEC;
}

SOCKET connect2server(url _url) {
	//Configure remote address
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;	//TCP
	struct addrinfo* serverInfo;
	if (getaddrinfo(_url.host->buffer, _url.port->buffer, &hints, &serverInfo))
		perr_exit("Failed congiguring info with getaddrinfo().", GETSOCKETERRNO());

	char address_buffer[100];
	char protocol_buffer[100];
	getnameinfo(serverInfo->ai_addr, serverInfo->ai_addrlen,
		address_buffer, sizeof(address_buffer),
		protocol_buffer, sizeof(protocol_buffer),
		NI_NUMERICHOST);

#if defined(_WIN32)
	// set the time out
	DWORD timeout = _MAX_WAITING_TIME_PER_PACKET * 1000;
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
#else
	struct timeval tv;
	tv.tv_sec = _MAX_WAITING_TIME_PER_PACKET;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
#endif

	SOCKET server;
	server = socket(serverInfo->ai_family,
		serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (!ISVALIDSOCKET(server))
		perr_exit("Failed to open an socket.", GETSOCKETERRNO());

	if (connect(server,
		serverInfo->ai_addr, serverInfo->ai_addrlen))
		perr_exit("Failed to connet to the server.", GETSOCKETERRNO());

	freeaddrinfo(serverInfo);

	printf("Connected to %s with protocol %s\n", address_buffer, protocol_buffer);

	return server;
}

void send_request(SOCKET _socket, url _url) {
	char buffer[_MAX_SEND_BUFFER_SIZE];

	sprintf_s(buffer, _MAX_SEND_BUFFER_SIZE, "GET /%s HTTP/1.1\r\n", _url.path->buffer);
	sprintf_s(buffer + strlen(buffer), _MAX_SEND_BUFFER_SIZE - strlen(buffer), "Host: %s:%s\r\n", _url.host->buffer, _url.port->buffer);
	sprintf_s(buffer + strlen(buffer), _MAX_SEND_BUFFER_SIZE - strlen(buffer), "Connection: close\r\n");	//or keep-alive
	sprintf_s(buffer + strlen(buffer), _MAX_SEND_BUFFER_SIZE - strlen(buffer), "User-Agent: http client beta 1.0\r\n");
	sprintf_s(buffer + strlen(buffer), _MAX_SEND_BUFFER_SIZE - strlen(buffer), "\r\n");	// end of header

	send(_socket, buffer, strlen(buffer), 0);
	printf("Sent:\n%s", buffer);
}

datapack recv_response(SOCKET _socket)
{
	size_t bytes_received = 0;	// used to store received size
	str_p responseBuffer = zero_str(_MAX_RECV_BUFFER_SIZE);
	int tmp;					// used to store received size temporarily

	FILE* fp;					// create a temporary file to store all received data
	errno_t err = tmpfile_s(&fp);
	if (err)
		perr_exit("Could not open new temporary file\n", err);

	bool loop_flag = TRUE;
	const clock_t start_time = clock();	// used to store the time now
	clock_t tempStartTime, totalWaitingTime = 0, tempWaitingTime;

	while (loop_flag)
	{
		tempStartTime = clock();

		if (totalWaitingTime > _MAX_WAITING_TIME_TOTAL) {
			if (_MAX_WAITING_TIME_TOTAL == -1);
			else
			{
				perr_exit("Timeout", -1);
				loop_flag = false;   // Since we set up a waiting time here, blocking is not acceptable, we have to use unblocking method
			}
		}

		tmp = recv(_socket, responseBuffer->buffer, _MAX_RECV_BUFFER_SIZE, 0);
		if (tmp < 1) {   // if the connection has been closed
			printf("Connection closed by remote.\n");
			break;
		}
		tempWaitingTime = _difftime(clock(), tempStartTime);
		totalWaitingTime = _difftime(clock(), start_time);

		// not working this way because recv is blocking
		if (tempWaitingTime > _MAX_WAITING_TIME_PER_PACKET)
		{
#ifdef _DEBUG_MODE
			printf("Consumed %ld seconds.", tempWaitingTime);
#endif // !_DEBUG_MODE

			perr_exit("Timeout", -1);
			loop_flag = false;
		}

		bytes_received += tmp;

#ifdef _DEBUG_MODE
		printf("One time received:%d bytes, spent %ld seconds, have received %llu bytes in total, consumed %ld seconds so far\n", tmp, tempWaitingTime, bytes_received, totalWaitingTime);
#endif // _DEBUG_MODE

		responseBuffer->size = tmp + 1;
		//responseBuffer->buffer[tmp] = '\0';
		//chk_str(&responseBuffer);	//update the size for later writing into files
		if (write2_file_str(fp, responseBuffer))
		{
			free_str(responseBuffer);
			perr_exit("Error writing temporary file\n", -1);	//-1 is an artifitial number
		}
	}
#ifdef _DEBUG_MODE
	printf("Have finished receiving, total received size:%llu, consumed %ld seconds in total\n", bytes_received, (clock() - start_time) / CLOCKS_PER_SEC);
#endif // _DEBUG_MODE
	free_str(responseBuffer);
	datapack received = { fp, bytes_received };
	return received;
}

void ALLcleanup(datapack data, SOCKET _socket)
{
	fclose(data.fp);
	CLOSESOCKET(_socket);
#if defined(_WIN32)
	/*
	* Before our program finishes, WSACleanup() is called if we're compiling for Winsock on
Windows. This function allows the Windows operating system to do additional cleanup.
	*/
	WSACleanup();
#endif
	free_str(data.protocol);
}

void ALLinitialization()
{
	// for windows, initialize the socket
#if defined(_WIN32)
	/*
	When using Berkeley sockets, no special initialization is needed, and the socket API is
always ready to use.
	In the main() function, we call WSAStartup() on Windows to initialize Winsock. The
MAKEWORD macro allows us to request Winsock version 2.2. If our program is unable to
initialize Winsock, it prints an error message and aborts.
	*/
	WSADATA wsaData;	//The WSADATA structure will be filled in by WSAStartup() with details about the Windows Sockets implementation
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		perr_exit("Failed to initialize Winsock.", GETSOCKETERRNO());
#endif
	return;
}

void headerExtractor(datapack* data)
{
	str_p tmpArray = zero_str(_MAX_RECV_BUFFER_SIZE); //maximum number of characters contained in a line

	size_t line_index = 1;
	char* movingPointer, * movingPointer2;

	rewind(data->fp);
	fgetpos(data->fp, &(data->headerPosition));
	data->ifChunked = FALSE;// set the default value for content type
	//Parsing header
	while (!feof(data->fp))
	{
		fgets(tmpArray->buffer, tmpArray->size, data->fp);
		if (line_index == 1)	//Parsing the first line, extract the state code and protocol
		{
			movingPointer = strstr(tmpArray->buffer, " ");
			if (!movingPointer)
				perr_exit("Error format of state line", -1);	//The first line must have a space to indicate the seperation of the protocol and the code
			else
			{
				*movingPointer++ = '\0';
				data->protocol = new_str(tmpArray->buffer);
				//The first line must have another space to indicate the seperation of the code and the description
				//for example HTTP/1.1 200 OK
				movingPointer2 = strstr(movingPointer, " ");
				if (!movingPointer2)
					perr_exit("Error format of state line", -1);
				else
				{
					*movingPointer2 = '\0';
					data->stateCode = strtol(movingPointer, NULL, 10);
				}
			}
		}
		else // not the first line, then the headers could be anything
		{
			/*
			we need to determine whether the HTTP server
			is using Content-Length or Transfer-Encoding: chunked to indicate body length. If it
			doesn't send either, then we assume that the entire HTTP body has been received once the
			connection is closed.
			*/
			// situation: if find content-length - must not be chunked
			// then find the encoding - then judge if it is chunked

			movingPointer = strstr(tmpArray->buffer, "Transfer-Encoding: ");
			movingPointer2 = strstr(tmpArray->buffer, "Content-Length: ");
			if (movingPointer2)
			{
				//if found content-length, then must not be chunked
				data->contentLength = strtol(movingPointer2 + 16, NULL, 10);
				data->ifChunked = FALSE;
			}
			else
			{
				//if not found content-length
				if (movingPointer)
				{
					//if not found content - length but found encoding
					// make judgement
					if (strstr(movingPointer + 19, "chunked"))
						data->ifChunked = TRUE;
					else//encoding is other type
						data->ifChunked = FALSE;
				}
				else
				{
					//not found anything, assume it is to be not chunked, do nothing since we have already set the default value for chunk
				}
			}
		}
		if (!strcmp(tmpArray->buffer, "\r\n"))
			break;
		line_index++;
	}
	free_str(tmpArray);

	data->headerSize = ftell(data->fp);
	//str_p header = zero_str(byte_ofHeader);
	//rewind(data->fp);
	//fread_s(header->buffer, byte_ofHeader, sizeof(char), byte_ofHeader, data->fp);
	data->bodySize = data->size - data->headerSize;
	fgetpos(data->fp, &(data->bodyPosition));
	return;
}

void postProcessor(datapack* data, FILE* downloadpath)
{
	headerExtractor(data);	//extract the header

	//judge if the content is chunked or not
	if (data->ifChunked)
	{
		//if it is chunked, then try to read the 16based data and do modification
		str_p tmpArray = zero_str(_MAX_RECV_BUFFER_SIZE);
		long chunkSize;

		fseek(data->fp, data->bodyPosition, SEEK_SET);	//set the cursor to the correct position
		while (fgets(tmpArray->buffer, tmpArray->size, data->fp))
		{
			chunkSize = strtol(tmpArray->buffer, NULL, 16);// read in the size of the chunk
			if (!chunkSize)	//when 0, then finish
				break;
			
			{
				free_str(tmpArray);
				tmpArray = zero_str(chunkSize);
			}
			fread_s(tmpArray->buffer, tmpArray->size, sizeof(char), chunkSize, data->fp);
			fwrite(tmpArray->buffer, sizeof(char), chunkSize, downloadpath); 
			fseek(data->fp, 2, SEEK_CUR);//+2 over jump the link break
		}
	}
	else
	{
		//现在是把文件全部读入内存，最好使用循环进行读
		//if not chunked, directly write to file from the temporary file
		fseek(data->fp, data->bodyPosition, SEEK_SET);	//function fsetpos not work
		size_t size = data->bodySize + 1;  //additional '\0' for appending to the end of the string
		str_p p = zero_str((size_t)size);
		if (size - 1 != fread(p->buffer, sizeof(char), size - 1, data->fp))
		{
			free_str(p);
			perr_exit("Assignment: Error reading file\n", -1);
		}
		p->buffer[size - 1] = '\0';

		if (data->bodySize != fwrite(p->buffer, sizeof(char), size - 1, downloadpath))
		{
			free_str(p);
			perr_exit("Assignment: Error writing file\n", -1);
		}
		free_str(p);
	}

}