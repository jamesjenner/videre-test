#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>

static int callback_http(
  struct libwebsocket_context *context,
  struct libwebsocket *wsi,
  enum libwebsocket_callback_reasons reason,
  void *user,
  void *in,
  size_t len) {
    switch(reason) {
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            printf("connection established\n");
        case LWS_CALLBACK_HTTP: {
            char *requested_uri = (char *) in;
            printf("requested URI: %s\n", requested_uri);
            
            if(strcmp(requested_uri, "/") == 0) {
                void *universal_response = "Hello, World!";
                libwebsocket_write(wsi, 
                    universal_response,
                    strlen(universal_response), 
                    LWS_WRITE_HTTP);
            } else {
                // try to get current working directory
                char cwd[1024];
                char *resource_path;
                
                if(getcwd(cwd, sizeof(cwd)) != NULL) {
                    // allocate enough memory for the resource path
                    resource_path = malloc(strlen(cwd) + strlen(requested_uri));
                    
                    // join current working directory to the resource path
                    sprintf(resource_path, "%s%s", cwd, requested_uri);
                    printf("resource path: %s\n", resource_path);
                    
                    char *extension = strrchr(resource_path, '.');
                    char *mime;
                    
                    // choose mime type based on the file extension
                    if (extension == NULL) {
                        mime = "text/plain";
                    } else if (strcmp(extension, ".png") == 0) {
                        mime = "image/png";
                    } else if (strcmp(extension, ".jpg") == 0) {
                        mime = "image/jpg";
                    } else if (strcmp(extension, ".gif") == 0) {
                        mime = "image/gif";
                    } else if (strcmp(extension, ".html") == 0) {
                        mime = "text/html";
                    } else if (strcmp(extension, ".css") == 0) {
                        mime = "text/css";
                    } else {
                        mime = "text/plain";
                    }
                    
                    // by default non existing resources return code 400
                    // for more information how this function handles headers
                    // see it's source code:
                    // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/parsers.c#n1896                    
                    
                    libwebsockets_serve_http_file(wsi, resource_path, mime);
                }
            }
            
            // close the connection
            libwebsocket_close_and_free_session(context, wsi, LWS_CLOSE_STATUS_NORMAL);
            break;
        }
        default:
            printf("unhandled callback\n");
            break;
    }
    
    return 0;
}

static struct libwebsocket_protocols protocols[] = {
    // first potocol must always be HTTP handler
    {
        "http-only",        // name
        callback_http,      // callback
        0                   // per_session_data_size
    },
    {
        NULL, NULL, 0           // end of list
    }
};

int main(void) {
    // server url will be http://localhost:7681
    int port = 8095;
    const char *interface = NULL;
    struct libwebsocket_context *context;
    // we're not using ssl
    const char *cert_path = NULL;
    const char *key_path = NULL;
    // no special options
    int opts = 0;
    
    // create libwebsocket context representing this server
    context = libwebsocket_create_context(
        port,
        interface,
        protocols,
        libwebsocket_internal_extensions,
        cert_path,
        key_path,
        -1,
        -1,
        opts);
        
    if(context == NULL) {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }
    
    printf("starting server...\n");
    
    // infinite loop, the only option to end this server is by sending SIGTERM.
    while (1) {
        libwebsocket_service(context, 50);
        // libwebsocket_service will process all waiting events with their callback 
        // functions and then wait 50 ms.
        // (this is single threaded webserver and this will keep
        // our server from generating load while there are not requests to process
    }
    
    libwebsocket_context_destroy(context);
    
    return 0;
}
