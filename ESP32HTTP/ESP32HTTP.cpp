#include "ESP32HTTP.h"


int utilCharToInt(char c){
  switch(c){
    case '0':
      return 0;
    case'1':
      return 1;
    case'2':
      return 2;
    case'3':
      return 3;
    case'4':
      return 4;
    case'5':
      return 5;
    case'6':
      return 6;
    case'7':
      return 7;
    case'8':
      return 8;
    case'9':
      return 9;
    default:
      return -1;
  }
}

//-------------------------------------------------------------------------
HTTPRequest::HTTPRequest(String newHttpMethod, String newHttpResource, String newPayload){
	httpMethod = newHttpMethod;
	httpResource = newHttpResource;
	httpPayload = newPayload;
	httpResquestHeaders = "";
}

HTTPRequest::~HTTPRequest() {
}

void HTTPRequest::setPayload(String newPayload){
	httpPayload = newPayload;
}

String HTTPRequest::getPayload(){
	return httpPayload;
}

String HTTPRequest::getMethod(){
	return httpMethod;
}

String HTTPRequest::getResource(){
	return httpResource;
}

String HTTPRequest::getRequestHeaders(){
	return httpResquestHeaders;
}

void HTTPRequest::setHeader(String headerName, String headerValue){
	httpResquestHeaders += headerName + ": " + headerValue + "\r\n";
}

void HTTPRequest::removeHeaders(){
	httpResquestHeaders = "";
}

//-------------------------------------------------------------------------
HTTPResponse::HTTPResponse(unsigned int newHttpResponseCode, String newHeaders, String newHttpResponseData){
	httpResponseData = newHttpResponseData;
  	httpResponseCode = newHttpResponseCode;
  	httpResponseHeaders = newHeaders;
}

HTTPResponse::~HTTPResponse() {
}

String HTTPResponse::getPayload(){
	return httpResponseData;
}

String HTTPResponse::getResponseHeaders(){
	return httpResponseHeaders;
}

unsigned int HTTPResponse::getResponseCode(){
	return httpResponseCode;
}

//-------------------------------------------------------------------------
HTTP::HTTP(char* newHost, unsigned int newPort, bool logger){
	host = newHost;
	port = newPort;

	log = logger;
	if(log){
		Serial.begin(115200);
		Serial.println("HTTP.h: HTTP.h log mode enabled");

		Serial.print("HTTP.h: host -> ");
		Serial.println(host);
		Serial.print("HTTP.h: port -> ");
		Serial.println(port);
	}
}

HTTP::~HTTP() {
}

char* HTTP::getHost(){
	return host;
}

String HTTP::getStringHost(){
	return String(host);
}

unsigned int HTTP::getPort(){
	return port;
}

void HTTP::setRequest(HTTPRequest req){
	httpRequest = req;
}

HTTPRequest HTTP::getRequest(){
	return httpRequest;
}

HTTPResponse HTTP::getResponse(){
	return httpResponse;
}

WiFiClient HTTP::getWifiClient(){
	return client;
}

HTTPResponse HTTP::sendRequest(){

	HTTPResponse res;

	if(client.connect(host, port)){
		if(log){
			Serial.println("HTTP.h: Connected to server");
		}

		String data = httpRequest.getMethod() + " " + httpRequest.getResource() + " HTTP/1.1\r\n";
		data += "Host: " + getStringHost() + ":" + (String)port + "\r\n";
		data += "Content-Length: " + (String)httpRequest.getPayload().length() + "\r\n";
		data += httpRequest.getRequestHeaders();
		data += "\r\n";
		data += httpRequest.getPayload();

		if(log){
			Serial.println("HTTP.h: Request -> ");
			Serial.println(data);
		}

		client.print(data);

		
		String firstLine = client.readStringUntil('\n');
		String responseHeaders = "";
		String payload = "";

		unsigned int code;


		for(int i = 0; i < firstLine.length(); i++){
			if(firstLine[i] == ' '){
				code = 100*utilCharToInt(firstLine[i+1]);
				code += 10*utilCharToInt(firstLine[i+2]);
				code += utilCharToInt(firstLine[i+3]);

				break;
			}
		}

		int PayloadSize = client.available();
		
		while (client.connected()){
			String line = client.readStringUntil('\n');

			if(line == "\r"){
		    	while (client.available()) {
		    		char c = client.read();
			    	payload += String(c);
			    }
				break;
			} else {
				responseHeaders += line + "\n";
			}
		}

		if(log){
			Serial.print("HTTP.h: PayloadSize -> ");
			Serial.println(PayloadSize);
			Serial.print("HTTP.h: ResponseCode -> ");
			Serial.println(code);
			Serial.print("HTTP.h: ResponseHeaders -> ");
			Serial.println(responseHeaders);
			
			Serial.print("HTTP.h: ResponsePayload -> ");
			Serial.println(payload);
		}

		res = HTTPResponse(code, responseHeaders, payload);
		httpResponse = res;
	} else if(log){
		Serial.println("HTTP.h: Cannot connect to server.");
	}

	client.stop();

	return res;
}


HTTPResponse HTTP::sendRequest(HTTPRequest request){
	HTTPResponse res;

	if(client.connect(host, port)){
		if(log){
			Serial.println("HTTP.h: Connected to server");
		}
		String data = request.getMethod() + " " + request.getResource() + " HTTP/1.1\r\n";
		data += "Host: " + getStringHost() + ":" + (String)port + "\r\n";
		data += "Content-Length: " + (String)request.getPayload().length() + "\r\n";
		data += request.getRequestHeaders();
		data += "\r\n";
		data += request.getPayload();

		if(log){
			Serial.println("HTTP.h: Request -> ");
			Serial.println(data);
		}

		client.print(data);

		
		String firstLine = client.readStringUntil('\n');
		String responseHeaders = "";
		String payload = "";

		unsigned int code;

		int PayloadSize = client.available();

		for(int i = 0; i < firstLine.length(); i++){
			if(firstLine[i] == ' '){
				code = 100*utilCharToInt(firstLine[i+1]);
				code += 10*utilCharToInt(firstLine[i+2]);
				code += utilCharToInt(firstLine[i+3]);

				break;
			}
		}

		while (client.connected()){
			String line = client.readStringUntil('\n');

			if(line == "\r"){
		    	while (client.available()) {
		    		char c = client.read();
			    	payload += String(c);
			    }
				break;
			} else {
				responseHeaders += line + "\n";
			}
		}

		if(log){
			Serial.print("HTTP.h: ResponseCode -> ");
			Serial.println(code);
			Serial.print("HTTP.h: ResponseHeaders -> ");
			Serial.println(responseHeaders);
			
			Serial.print("HTTP.h: ResponsePayload -> ");
			Serial.println(payload);
		}

		res = HTTPResponse(code, responseHeaders, payload);
		httpResponse = res;
	} else if(log){
		Serial.println("HTTP.h: Cannot connect to server.");
	}

	client.stop();

	return res;	
}