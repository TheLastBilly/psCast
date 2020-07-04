#include "downloader.hpp"

int downloader::init()
{
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

    return OK;
}

int downloader::end()
{
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

    return OK;
}

void downloader::download( std::string url, std::string path )
{	
    int imageFD = sceIoOpen( path.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if(!imageFD){
		return;
	}
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
        std::string header, body;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Set useragant string
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		// Set SSL VERSION to TLS 1.2
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		// Set timeout for the connection to build
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		// Follow redirects (?)
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		// The function that will be used to write the data 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
		// The data filedescriptor which will be written to
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFD);
		// write function of response headers
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		// the response header data where to write to
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		// Request Header :
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		//headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");  Setting this will lead to errors when trying to download. should be set depending on location : possible : cdn.discordapp.com or images.discordapp.com
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		
		// Perform the request
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			
		}else{
			
		}
		
		
	}else{
		
	}

	// close filedescriptor
	sceIoClose(imageFD);

	// cleanup
	curl_easy_cleanup(curl);
	
}