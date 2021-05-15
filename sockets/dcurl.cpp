#include <iostream>
#include <sstream>

#include "MySocket.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " domain port" << endl;
    return 0;
  }

  int port;
  char *domain = argv[1];
  stringstream stream(argv[2]);
  stream >> port;

  MySocket socket(domain, port);

  stringstream request;

  request << "GET /hello_world.html HTTP/1.1" << "\r\n";
  request << "Host: localhost:8080" << "\r\n";
  request << "User-Agent: GunrockClient/1.0" << "\r\n";
  request << "Accept: */*" << "\r\n";
  request << "\r\n";

  socket.write(request.str());

  try {
    while (true) {
      string text = socket.read();
      cout << text;
    }
  } catch (...) {
    // ignore the exception and continue executing
  }

  return 0;
}
