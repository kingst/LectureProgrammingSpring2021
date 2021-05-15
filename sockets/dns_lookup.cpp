#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;


int lookup(char *domain_name) {
  struct addrinfo hints;
  struct addrinfo *results;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int ret = getaddrinfo(domain_name, NULL, &hints, &results);
  if (ret != 0) {
    cerr << "Could not get host " << domain_name << endl;
    return 1;
  }

  for (struct addrinfo *address_info = results;
       address_info != NULL;
       address_info = address_info->ai_next) {

    struct in_addr in_addr = ((struct sockaddr_in *) (address_info->ai_addr))->sin_a\
ddr;
    char buffer[INET_ADDRSTRLEN];
    const char *ip_address_string = inet_ntop(AF_INET, &in_addr, buffer, sizeof(buffer));
    if (ip_address_string == NULL) {
      cerr << "could not convert IP address to a string" << endl;
      return 1;
    }

    cout << "IP Address: " << ip_address_string << endl;

    // either (1) loop through IP addresses until you can "connect" to one
    // or (2) open all in parallel and use the first result
  }
  
  return 0;
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " domain_name" << endl;
    return 0;
  }

  char *domain_name = argv[1];
  return lookup(domain_name);
}
