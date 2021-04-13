#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

using namespace std;

int main(int argc, char *argv[]) {
  vector<int> file_descriptors;

  // we should probably also make STDIN non blocking, but whatev
  file_descriptors.push_back(STDIN_FILENO);
  for (int idx = 1; idx < argc; idx++) {
    int fd = open(argv[idx], O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
      cerr << "could not open " << argv[idx] << endl;
      return 1;
    }
    file_descriptors.push_back(fd);
  }

  fd_set read_fds;
  int ret;

  while (file_descriptors.size() > 0) {
    FD_ZERO(&read_fds);
    int max_fd = -1;
    for (int idx = 0; idx < file_descriptors.size(); idx++) {
      int fd = file_descriptors[idx];
      FD_SET(fd, &read_fds);
      if (fd > max_fd) {
	max_fd = fd;
      }
    }
    max_fd++;

    // select will block until one of our file descriptors can be read
    select(max_fd, &read_fds, NULL, NULL, NULL);

    // select will set read_fds with any fds that we can read
    for (int idx = 0; idx < file_descriptors.size(); idx++) {
      int fd = file_descriptors[idx];
      if (FD_ISSET(fd, &read_fds)) {
	unsigned char buffer[4096];
	ret = read(fd, buffer, sizeof(buffer));
	if (ret == 0) {
	  // we're done with this file, clean it up
	  close(fd);
	  file_descriptors.erase(file_descriptors.begin() + idx);
	  break;
	} else {
	  // NOTE: we're only writing what we read, not the entire
	  // file
	  //
	  // NOTE NOTE: write is blocking! we should handle it with
	  // with select also, but we're trying to keep this example
	  // simple
	  write(STDOUT_FILENO, buffer, ret);
	}
      }
    }
  }
}
