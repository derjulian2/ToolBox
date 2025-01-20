#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

struct CurlEasyHandle
{
	enum CurlFlags
	{
		NO_FLAGS = 0b00000000,
		CLIENT_INFO = 0b00000001,
		CLIENT_CURL_INFO = 0b00000010,
	};

	CurlEasyHandle(CurlFlags flags);
	~CurlEasyHandle();

	static curl_slist* to_headers(curl_slist * list, const std::vector<std::string>& headers);


	CURL* curl_handle = nullptr;
	
	bool debug_info = false;
	bool verbose_info = false;
};

size_t write_callback(void* buffer, size_t elem_size, size_t elem_num, void* outbuffer);

struct HTTP_REQUEST
{
	std::string url;
	CURLcode request_exit_code;

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

	HTTP_REQUEST(CurlEasyHandle& client, std::string url, HTTPMessage message = HTTPMessage(), Credentials auth = {});
private:
	CURLcode make_request(CurlEasyHandle& client);
};

struct HTTP_GET : HTTP_REQUEST
{

};

struct HTTP_POST : HTTP_REQUEST
{

};
// initialze OnvifSession to init curl and the handle
// then init every Request with this session passed as a reference
class OnvifSession
{
public:
	OnvifSession();

	struct SOAPEnvelope
	{
		std::string xml_message; // XMLMessage
		std::string* header; // XMLTag*
		std::string* body; // XMLTag*
	};

	struct WSBaseAuthentication
	{
		std::string username;
		std::string password;
		std::string nonce;
		std::string password_digest;
		std::string timestamp;
	};

	struct PasswordDigestAuthentication
	{

	};

	struct GetDeviceInformation
	{

	};
};