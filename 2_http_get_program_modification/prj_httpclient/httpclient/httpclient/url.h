#ifndef _HEADER_URL_
#define _HEADER_URL_
#include <stdio.h>
#include "str.h"
typedef struct
{
	str* url_origin;
	str* protocol;
	str* host;
	str* port;
	str* path;
} url;


url parse_url(str_p url_origin);

#endif /* _HEADER_URL_ */