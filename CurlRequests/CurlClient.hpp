#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

enum
{
	NO_FLAGS	     = 0b00000000,
	CLIENT_INFO	     = 0b00000001,
	CLIENT_CURL_INFO = 0b00000010,
};

class CurlClient
{
public:
	CurlClient(uint8_t flags);
	~CurlClient();

	CURL* getHandle();

	static curl_slist* to_headers(curl_slist * list, const std::vector<std::string>& headers);

private:

	CURL* curl_handle = nullptr;
	
	bool debug_info = false;
	bool verbose_info = false;
};

struct HTTP_REQUEST
{
	std::string url;
	CURLcode request_exit_code;

	enum HTTPMethod
	{
		GET,
		POST
	} method;

	struct Credentials
	{
		std::string username;
		std::string password;
	} auth;

	struct HTTPMessage
	{
		HTTPMessage();
		HTTPMessage(std::string headers_raw, std::string body_raw);

		std::vector<std::string> message_headers;
		std::string message_body;
	} request, response;

	HTTP_REQUEST(CurlClient& client, HTTPMethod method, std::string url, HTTPMessage message = HTTPMessage(), Credentials auth = {});
private:
	CURLcode make_request(CurlClient& client);
};

size_t write_callback(void* buffer, size_t elem_size, size_t elem_num, void* outbuffer);