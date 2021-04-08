// Use pipe() and dup2() to implement "ls | wc -l"
#include <cstring>
#include <iostream>
#include <string>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char *argv[])
{
    int fd[2];
    // pipe() will connect the input in fd[0] to the output of fd[1]
    pipe(fd);
    pid_t pid = fork();
    pid_t pid_ls = 0, pid_wc = 0;

    if (pid == 0)
    {
        pid_ls = getpid();
        cout << "I am the child process for ls: " << pid_ls << endl;
        // dup2() will copy the file descriptor to stdout
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        // Use execv in project 2
        // execlp("ls", "ls", nullptr);
        char *arr[2];
        arr[0] = strdup("/bin/ls");
        arr[1] = nullptr;
        execv(arr[0], arr);
    }
    else
    {

        pid = fork();

        if (pid == 0)
        {
            pid_wc = getpid();
            cout << "I am the child process for wc: " << pid_wc << endl;
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execlp("wc", "wc", "-l", nullptr);
        }
        else
        {
            int status;
            close(fd[0]);
            close(fd[1]);
            waitpid(pid_ls, &status, 0); // ls
            waitpid(pid_wc, &status, 0); // wc
        }
    }

    return 0;
}