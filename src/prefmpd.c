#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>

char* (*__orig_getenv)(const char*);

static const char* const ENV_MPD_PORT = "MPD_PORT";
static const char* const ENV_MPD_HOST = "MPD_HOST";
static const char* const ENV_PREF_MPD_PORT = "PREF_MPD_PORT";
static const char* const ENV_PREF_MPD_HOST = "PREF_MPD_HOST";
static char* PREF_MPD_HOST = NULL;
static char* PREF_MPD_PORT = NULL;
static char pref_host_addrstr[INET6_ADDRSTRLEN];

__attribute__((constructor)) void init() {
	__orig_getenv = dlsym(RTLD_NEXT, "getenv");
	if(!__orig_getenv) {
		errno = ENOSYS;
		abort();
	}
	PREF_MPD_HOST = __orig_getenv(ENV_PREF_MPD_HOST);
	PREF_MPD_PORT = __orig_getenv(ENV_PREF_MPD_PORT);
}

static bool get_pref_host(const char* const pref_host_name) {
	struct addrinfo* pref_addr_info;
	const int gai_status = getaddrinfo(pref_host_name, NULL, NULL, &pref_addr_info);
	if(0 == gai_status){
		memset(pref_host_addrstr, 0, sizeof(pref_host_addrstr));
		if(NULL != inet_ntop(pref_addr_info->ai_family, pref_addr_info->ai_addr, pref_host_addrstr, INET6_ADDRSTRLEN)){
			return true;
		} else {
			perror("error converting sockaddr to string");
		}
	} else {
		fprintf(stderr, "Error resolving %s: %s", pref_host_name, gai_strerror(gai_status));
	}
	return false;
}

char* getenv(const char* name) {
	if(PREF_MPD_HOST != NULL) {
		const bool want_host = (0 == strcmp(name, ENV_MPD_HOST));
		const bool want_port = (0 == strcmp(name, ENV_MPD_PORT));
		bool have_host = false;
		if((want_host || want_port) && PREF_MPD_HOST != NULL) {
			have_host = get_pref_host(PREF_MPD_HOST);
		}
		if(want_host && have_host) {
			return pref_host_addrstr;
		}
		if(want_port && have_host) {
			return PREF_MPD_PORT;
		}
	}
	return __orig_getenv(name);
}
