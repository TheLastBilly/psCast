#include "http.hpp"

#include <cstring>

//Taken from net_libcurl/src/main.cpp on https://github.com/vitasdk/samples

bool HttpBase::is_init = false;

namespace
{
	struct stringcurl {
		char *ptr;
		size_t len;
	}; 	

	void init_string(struct stringcurl *s) 
	{
		s->len = 0;
		s->ptr = (char*)malloc(s->len+1);
		if (s->ptr == NULL) {
			fprintf(stderr, "malloc() failed\n");
			return;
			//exit(EXIT_FAILURE);
		}
		s->ptr[0] = '\0';
	}

	size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s)
	{
		size_t new_len = s->len + size*nmemb;
		s->ptr = (char*)realloc(s->ptr, new_len+1);
		if (s->ptr == NULL) {
			fprintf(stderr, "realloc() failed\n");
			return 0;
			//exit(EXIT_FAILURE);
		}
		memcpy(s->ptr+s->len, ptr, size*nmemb);
		s->ptr[new_len] = '\0';
		s->len = new_len;

		return size*nmemb;
	}

		static size_t write_data_to_disk(void *ptr, size_t size, size_t nmemb, void *stream)
		{
		size_t written = sceIoWrite(   *(int*) stream , ptr , size*nmemb);
		return written;
	}
}

int HttpBase::init()
{
	if(is_init)
		return OK;

    //Net init
    if(sceSysmoduleLoadModule(SCE_SYSMODULE_NET) != 0)
        return NET_MODULE_ERROR;
        
	SceNetInitParam nparam;
	int size = 4*1024*1024;
	nparam.memory = malloc(size);
	nparam.size = size;
	nparam.flags = 0;
	if(sceNetInit(&nparam) != 0)
        return NET_RUN_ERROR;

    if(sceNetCtlInit() != 0)
        return NET_CTL_ERROR;
    
    //HTTP init
	if(sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP) != 0)
        return HTTP_MODULE_ERROR;
    if(sceHttpInit(size) != 0)
        return HTTP_RUN_ERROR;

	is_init = true;
    return OK;
}

int HttpBase::end()
{
	if(!is_init)
		return OK;

    //HTTP deinit
    if(sceHttpTerm() != 0)
        return HTTP_RUN_ERROR;
    if(sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP) != 0)
        return HTTP_MODULE_ERROR;

    //Net deinit
    sceNetCtlTerm();
    if(sceNetTerm() != 0)
        return NET_RUN_ERROR;
    if(sceSysmoduleUnloadModule(SCE_SYSMODULE_NET) != 0)
        return NET_MODULE_ERROR;

	is_init = false;
    return OK;
}

int Https::download( std::string url, std::string path )
{	
	int st = init();

	if(st != OK)
		return st;

    int imageFD = sceIoOpen( path.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if(!imageFD){
		return NET_SOCKET_ERROR;
	}

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) 
	{
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		static const char * usr_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36";
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, usr_agent);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFD);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, std::string("User-Agent: " + std::string(usr_agent)).c_str());
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);
		
		if(res != CURLE_OK){}
			st = CURL_ERROR;	
	}
	else
		st = CURL_ERROR;

	sceIoClose(imageFD);
	curl_easy_cleanup(curl);
	
	return st;
}

int Http::download( std::string url, std::string path )
{
	int st = init();

	if(st != OK)
		return st;
		
	int tpl = sceHttpCreateTemplate("psCast", 1, 1);
	int conn = sceHttpCreateConnectionWithURL(tpl, url.c_str(), 0);
	int request = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, url.c_str(), 0);
	int handle = sceHttpSendRequest(request, NULL, 0);

	int fh = sceIoOpen(path.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
    if(fh < 0)
        return NET_SOCKET_ERROR;

	unsigned char data[16*1024];
	int read = 0;

	while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0)
		int write = sceIoWrite(fh, data, read);

	sceIoClose(fh);

    return OK;
}