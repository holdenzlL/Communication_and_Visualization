#include "httpclient.h"

int main(int argc, char* argv[])
{
	//initialization
	ALLinitialization();

	//Parsing URL
	str_p url_input = new_str("http://www.home.hs-karlsruhe.de/~weth0002/images/TWatVGUsmall.jpg");
	url _url = parse_url(url_input);
	free_str(url_input);	// url_input needs to be freed after use

	printf("URL: %s\n", _url.url_origin->buffer);
	printf("Protocol: %s\n", _url.protocol->buffer);
	printf("Host: %s\n", _url.host->buffer);
	printf("Port: %s\n", _url.port->buffer);
	printf("Path: %s\n", _url.path->buffer);

	//Create socket and connect to the server
	SOCKET server = connect2server(_url);

	//Send request
	send_request(server, _url);

	//Receive response, the response data will be stored into a temporary file
	datapack data = recv_response(server);


	
	str_p name = cpy_str(_url.path);
	char* movingP, *savedPosition;
	 savedPosition = movingP = name->buffer;
	while (*movingP != '\0')
	{
		if (*movingP == '/')
			savedPosition = movingP + 1;
		movingP++;
	}
	str_p filePath = new_str("D:/");	// change the disk here
	str_p fullName = append_str(filePath, savedPosition);
	free_str(name);

	FILE* saveFP;
	fopen_s(&saveFP, fullName->buffer, "wb");
	if (!saveFP)
		perr_exit("Assignment: Error opening file\n", -1);


	//Extract data from header, and do download
	postProcessor(&data, saveFP);
	fclose(saveFP);
	

	// Close and clean up
	ALLcleanup(data, server);

	return 0;
}