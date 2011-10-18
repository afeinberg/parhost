#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include "boost/threadpool.hpp"

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

namespace {

const int k_default_maxthreads = 32;
const int k_nameinfo_maxhost = NI_MAXHOST;
        
using namespace boost::threadpool;

void lookup(const std::string &buf);
void submit_tasks(int max_threads);

void lookup(const std::string &buf) {
    struct addrinfo hints;
    struct addrinfo *ai;
    int rc;
   
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    
    if ((rc = getaddrinfo(buf.c_str(), NULL, &hints, &ai)) != 0) {
        std::cerr << "getaddrinfo(): " << gai_strerror(rc) << std::endl;
    } else if (ai != NULL) {
        char hostname[k_nameinfo_maxhost] = "";
        rc = getnameinfo(ai->ai_addr,
                         ai->ai_addrlen,
                         hostname,
                         k_nameinfo_maxhost,
                         NULL,
                         0,
                         0);
        if (rc != 0) {
            std::cerr << "getnameinfo(): " << gai_strerror(rc) << std::endl;
        } else if (*hostname != '\0') {
            std::cout << buf << " -> " << hostname << std::endl;           
        }
    }
    
    freeaddrinfo(ai);
}

void submit_tasks(int max_threads) {
    pool tp(max_threads);    
    std::string line;
    while (std::getline(std::cin, line)) {
        tp.schedule(boost::bind(lookup, line));
    }    
}

} // namespace


int main(int argc, char **argv) {
    int max_threads = argc < 2 ? k_default_maxthreads : atoi(argv[1]);
    submit_tasks(max_threads);
    return EXIT_SUCCESS;
}
