#include <iostream>
#include <cstring>
#include <string>

#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Hello world!" << endl;

  pid_t ret = fork();
  if (ret < 0) {
    cerr << "fork failed" << endl;
    return 1;
  } else if (ret == 0) {
    cout << "I am the child, my pid is " << getpid() << endl;

    char *myargs[3];
    myargs[0] = strdup("ls");
    myargs[1] = strdup("-a");
    myargs[2] = NULL;

    execvp(myargs[0], myargs);
    cerr << "something went wrong" << endl;
    return 1;
  } else {
    pid_t ret_wait = wait(NULL);
    cout << "I am the parent, my pid is " << getpid() << " child " << ret_wait << endl;
    
  }
}
