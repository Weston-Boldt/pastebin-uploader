#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #define BUFFSIZE = 512000;

#include "test.h"

int main(int argc, char *argv[]) {

    /*
	if (curl) {
		printf("test\n");
	}
    */

    if (argc == 1) {
        printf("useage:\npastebin-poster filename\n");
        exit(0);
    }

    char *filename = argv[1];
    char *filebuf = malloc(512 * 1000);
    
    load_file(filename, filebuf);

	char *filetype = get_file_ext(filename);

    char *api_key = getenv("PASTEBIN_TOKEN");
    
    char curlopt_str[512100]; 
    get_postopt_str(curlopt_str, filename, filetype, filebuf, api_key);

    /*
    printf("%s\n", filebuf);
    printf("%s\n", curlopt_str);
    printf("%s\n", api_key);
    printf("%s\n", filetype);
    printf("%s\n", filename);
    */

	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            "https://pastebin.com/api/api_post.php"
        );
        curl_easy_setopt(
            curl,
            CURLOPT_POSTFIELDS,
            curlopt_str
        );
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        }
        /* always cleanup */ 
        free(filebuf);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

void get_postopt_str(
    char* buf,
    char* filename,
    char* filetype,
    char* filebuf,
    char* api_key
) {
    sprintf(buf,
        "api_option=paste&api_paste_name='%s'&api_format_type='%s'&api_paste_code=%s&api_dev_key=%s",
        filename,filetype,filebuf,api_key); 
}

char* load_file(char *fn, char *buf) {
    FILE *fp;
    fp = fopen(fn, "r");
    fread(buf, sizeof(char), 512000, fp);
    fclose(fp);
    return fn;
}

char* get_file_ext(char fn_ptr[]) {
    char delimn = '.';
    int hit_delimn = 0;

    // iterate to the end of the string
    // and also scan through and validate the str
	while(*fn_ptr!= '\0') {
        if (*fn_ptr == delimn) {
            hit_delimn++;
        }
		fn_ptr++;
	}
    if (hit_delimn == 0) {
        return "";
    }

    // go back to the last .
    while(*fn_ptr != delimn) {
        fn_ptr--;
    }

    // go one more further to just have the filetype
    fn_ptr++;

    return fn_ptr;
}
