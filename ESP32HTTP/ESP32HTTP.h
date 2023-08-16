/*
*	Author: Migue27au --> https://github.com/migue27au
*	Last Update: 07/08/2023
*
*	Library to handle easier the HTTP and HTTPS conecctions between ESP32 and server
*/
#ifndef ESP32HTTP_H
#define ESP32HTTP_H

#include "Arduino.h"
#include <string.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>


#define HTTP_POST "POST"
#define HTTP_GET "GET"
#define HTTP_DELETE "DELETE"
#define HTTP_PUT "PUT"
#define HTTP_HEAD "HEAD"
#define HTTP_OPTIONS "OPTIONS"

#define HTTP_RESPONSE_OK 200
#define HTTP_RESPONSE_CREATED 201
#define HTTP_RESPONSE_ACCEPTED 202
#define HTTP_RESPONSE_PARTIAL_CONTENT 206
#define HTTP_RESPONSE_MULTIPLE_CHOICE 300
#define HTTP_RESPONSE_MOVED_PERMANENTLY 301
#define HTTP_RESPONSE_FOUND 302
#define HTTP_RESPONSE_BAD_REQUEST 400
#define HTTP_RESPONSE_UNAUTHORIZED 401
#define HTTP_RESPONSE_FORBIDDEN 403
#define HTTP_RESPONSE_METHOD_NOT_ALLOWED 405
#define HTTP_RESPONSE_CONFLICT 409
#define HTTP_RESPONSE_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_RESPONSE_INTERNAL_SERVER_ERROR 500
#define HTTP_RESPONSE_NOT_IMPLEMENTED 501
#define HTTP_RESPONSE_BAD_GATEWAY 502
#define HTTP_RESPONSE_SERVICE_UNAVAILABLE 503
#define HTTP_RESPONSE_GATEWAY_TIMEOUT 504
#define HTTP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED 505


#define HTTPS_PORT 443
#define HTTP_PORT 80

#define TIMEOUT_SECONDS 10

/*
* Class to handle https requests 
*/
class HTTPRequest{
public:
	HTTPRequest(String newHttpMethod = "", String newHttpResource = "", String newPayload = "");
	~HTTPRequest();

	//return the http method that the request is using
	String getMethod();
	//return the resource uri
	String getResource();

	//set and get payload of the request
	void setPayload(String payload);
	String getPayload();

	//to specify a new header. IMPORTANT --> Content-Length header is added automatically
	void setHeader(String headerName, String headerValue);

	//to remove all headers
	void removeHeaders();

	//return all the headers that the request is using
	String getRequestHeaders();

private:
	String httpMethod;
	String httpResource;
	String httpResquestHeaders;
	String httpPayload;
};

/*
* Class to handle https responses 
*/
class HTTPResponse{
public:
	HTTPResponse(unsigned int newHttpResponseCode = 0, String newHeaders = "", String newHttpResponseData = "");
	~HTTPResponse();

	/*return response headers*/
	String getResponseHeaders();
	/*return http response code*/
	unsigned int getResponseCode();

	/*return response payload*/
	String getPayload();
	

private:
	String httpResponseData;
	unsigned int httpResponseCode;
	String httpResponseHeaders;
};

/*
* Class to handle https conections 
*/
class HTTP{
public:
	HTTP();
	HTTP(char* newHost, unsigned int newPort, bool logger = false);
	HTTP(char* newHost, unsigned int newPort, char* newRootCACert, bool logger = false);
	~HTTP();


	void setValues(char* newHost, unsigned int newPort, bool logger = false);
	void setValues(char* newHost, unsigned int newPort, char* newRootCACert, bool logger = false);

	void begin();

	//return the host IP or domain
	char* getHost();

	//return the host IP or domain in string object
	String getStringHost();
	
	//return the server port
	unsigned int getPort();

	//set a specified HTTPRequest object
	void setRequest(HTTPRequest req);
	//return the HTTPRequest object
	HTTPRequest getRequest();

	//return the HTTPResponse object
	HTTPResponse getResponse();

	//return WiFiClient object
	WiFiClient getWifiClient();
	
	//return WiFiClientSecure object
	WiFiClientSecure getWifiClientSecure();

	//Send the specified HTTPSREQUEST to host and return the HTTPResponse object for this request
	HTTPResponse sendRequest();

	//Set and send a specified HTTPREQUEST to host and return the HTTPResponse object for this request
	HTTPResponse sendRequest(HTTPRequest request);

private:
	bool log;
	bool isSSL = false;

	WiFiClient client;
	WiFiClientSecure clientSecure;

	HTTPRequest httpRequest;
	HTTPResponse httpResponse;


	char* host;
	unsigned int port;
	char* rootCACert;
};


#endif