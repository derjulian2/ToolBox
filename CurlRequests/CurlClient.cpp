#include "CurlClient.hpp"


CurlClient::CurlClient(uint8_t flags)
{
#ifndef CURLCLIENT_NO_CURL_INIT
	curl_global_init(CURL_GLOBAL_ALL);
#endif
	curl_handle = curl_easy_init();
	if (!curl_handle) {
		throw std::runtime_error("\nFailed to easy_init curl_handle of CurlClient\n");
	}

	if (flags != NULL) {
		if (flags & 1)
			debug_info = true;
		if ((flags >> 1) & 1)
			verbose_info = true;
	}
}

CurlClient::~CurlClient()
{
	curl_easy_cleanup(curl_handle);
#ifndef CURLCIENT_NO_CURL_INIT
	curl_global_cleanup();
#endif
}

CURL* CurlClient::getHandle()
{
	return curl_handle;
}

curl_slist* CurlClient::to_headers(curl_slist* list, const std::vector<std::string>& headers)
{
	for (const std::string& str : headers)
	{
		list = curl_slist_append(list, str.c_str());
	}
	return list;
}

HTTP_REQUEST::HTTPMessage::HTTPMessage()
{
	
}
HTTP_REQUEST::HTTPMessage::HTTPMessage(std::string headers_raw, std::string body_raw)
	: message_body(body_raw)
{
	std::string out;
	std::istringstream stream(headers_raw);
	while (std::getline(stream, out))
		message_headers.emplace_back(out);
}

HTTP_REQUEST::HTTP_REQUEST(CurlClient& client, HTTPMethod method, std::string url, HTTPMessage message, Credentials auth)
	: url(url), method(method), request(message), auth(auth)
{
	request_exit_code = make_request(client);
}
CURLcode HTTP_REQUEST::make_request(CurlClient& client)
{
	curl_easy_setopt(client.getHandle(), CURLOPT_URL, url.c_str());

	if (!request.message_headers.empty())
	{
		curl_slist* headers = nullptr;
		headers = CurlClient::to_headers(headers, request.message_headers);

		curl_easy_setopt(client.getHandle(), CURLOPT_HTTPHEADER, headers);
	}

	switch (method)
	{
		case (GET):
		{
			curl_easy_setopt(client.getHandle(), CURLOPT_HTTPGET, true);
			break;
		}
		case (POST):
		{
			curl_easy_setopt(client.getHandle(), CURLOPT_HTTPPOST, true);
			curl_easy_setopt(client.getHandle(), CURLOPT_POSTFIELDS, request.message_body.c_str());
			curl_easy_setopt(client.getHandle(), CURLOPT_POSTFIELDSIZE, request.message_body.size());
			break;
		}
	}

	std::string response_body_raw;
	std::string response_headers_raw;

	curl_easy_setopt(client.getHandle(), CURLOPT_WRITEDATA, &response_body_raw);
	curl_easy_setopt(client.getHandle(), CURLOPT_WRITEFUNCTION, write_callback);

	curl_easy_setopt(client.getHandle(), CURLOPT_HEADERDATA, &response_headers_raw);
	curl_easy_setopt(client.getHandle(), CURLOPT_HEADERFUNCTION, write_callback);

	CURLcode res = curl_easy_perform(client.getHandle());

	response = HTTPMessage(response_headers_raw, response_body_raw);
	curl_easy_reset(client.getHandle());

	return res;
}

size_t write_callback(void* buffer, size_t elem_size, size_t elem_num, void* outbuffer)
{
	static_cast<std::string*>(outbuffer)->append(static_cast<char*>(buffer), elem_num * elem_size);
	return elem_size * elem_num;
}
