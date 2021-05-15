#include <iostream>
#include <sstream>

#include "MyServerSocket.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << endl;
    return 0;
  }

  int port;
  stringstream stream(argv[1]);
  stream >> port;

  MyServerSocket socket(port);

  while (true) {
    MySocket *client = socket.accept();

    string data = client->read();
    client->write(data);
    delete client;
  }
}
