#include "url.h"

url parse_url(str_p url_origin) {
    url parsed_url;
    parsed_url.protocol = NULL;
    parsed_url.host = NULL;
    parsed_url.url_origin = cpy_str(url_origin);
    char* p = NULL; // moving pointer
    //tmp pointer for storing the locations
    char* protocol = NULL;
    char* host = NULL;
    char* port = NULL;
    char* path = NULL;   

    //Try to find the indicator ://
    p = srh_str(url_origin, "://");

    //If success finding :// set the protocol
    if (p) {
        protocol = url_origin->buffer;
        *p = 0;
        p += 3;
        parsed_url.protocol = new_str(protocol);
        // move p to overcome the indicator
    }
    else {
        p = url_origin->buffer; // no indicator, no protocol
    }

    //if a protocol is found, judge if it is http
    //only http is surpported
    //case sensitive! 这一点需要在后面改
    if (parsed_url.protocol) {
        if (cmp_charArray_(parsed_url.protocol->buffer, "http")) {
            fprintf(stderr,
                "Unknown protocol '%s'. Only 'http' is supported.\n",
                parsed_url.protocol->buffer);
        }
    }
    else
    {// if not fount, set the protocol as http, which is the default
        parsed_url.protocol = new_str("http");
    }
    //after over come the indicator, there is host address
    host = p;
    
    //find :port, or /site or #fragment
    while (*p && *p != ':' && *p != '/' && *p != '#') //trying to find :
        ++p;

    if (*p == ':') {
        *p++ = '\0';    // set the : to be \0, and overcome the :
        parsed_url.host = new_str(host);
        port = p;
    }
    else
    {
        parsed_url.port = new_str("80");    // set the default port
    }

    while (*p && *p != '/' && *p != '#')    //trying to find /
        ++p;

    if (*p == '/') { 
        *p++ = '\0';    // set the / to be \0
        if(port)
            parsed_url.port = new_str(port);
        if (!parsed_url.host)
            parsed_url.host = new_str(host);
        path = p;
    }
    
    while (*p && *p != '#')     //trying to find #
        ++p;

    if (*p == '#')
    {
        *p = '\0';
        if (!parsed_url.host)
            parsed_url.host = new_str(host);
    }

    if (path)
        parsed_url.path = new_str(path);
    else
        parsed_url.path = new_str("");
    if (!parsed_url.host)
        parsed_url.host = new_str(host);
    return parsed_url;
}