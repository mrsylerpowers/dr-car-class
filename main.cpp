// Example program
#include <iostream>
#include <string.h>
// Keyboard exit signal escape
#include <signal.h>
// Parse arguments
#include <unistd.h>
// Run processes
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>



using namespace std;


void printShell();
std::string getLineInput();
void runSHellInput();
static int exec_prog(const char **argv);
const char** getArgVfromString(const char* args);
void handlePromptLine(string basic_string);
void signal_callback_handler(int signum);


int main(int argc, char *argv[]) {
    signal(SIGINT, signal_callback_handler);
    int opt;
    while ((opt = getopt(argc, argv, "")) != -1) {
        switch (opt) {
            default:
                fprintf(stderr, "Usage: %s [>> file...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }

    }
    printShell();
    while (true){
        runSHellInput();
    }
}

void signal_callback_handler(int signum) {
    cout << endl << "Terminating Shell With Exit Code: " << signum << endl;
    // Terminate program
    exit(signum);
}
void runSHellInput(){
    string stringInput = getLineInput();
    handlePromptLine(stringInput);
    printShell();

}

void handlePromptLine(string basic_string) {

    if (basic_string == "exit"){
        exit(EXIT_SUCCESS);
    }
    else{
        cout << endl;
        exec_prog(getArgVfromString(basic_string.c_str()));
    }
    cout << endl;
}

std::string getLineInput(){
    std::string lineInput;
    getline (std::cin, lineInput);
    return lineInput;
}
void printShell(){
        std::cout << "ash> ";
}

const char** getArgVfromString(const char* args) {
    const char ** sub_str = static_cast<const char **>(malloc(10 * sizeof(char*)));
    for (int i =0 ; i < 30; ++i)
        sub_str[i] = static_cast<const char *>(malloc(20 * sizeof(char)));
    int i = 0;
    while( (sub_str[i] = strsep((char **)&args," ")) != NULL ){
        i ++;

    }
    return sub_str;
}
static int exec_prog(const char **argv)
{
    pid_t   my_pid;
    int     status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;
    if (0 == (my_pid = fork())) {
        char buffer[256]; // <- danger, only storage for 256 characters.
        strncpy(buffer, "/bin/", sizeof(buffer));
        strncat(buffer,  argv[0], sizeof(buffer));
        if (-1 == execve(buffer, (char **)argv , NULL)) {
            perror("child process execve failed [%m]");
            return -1;
        }
    }

#ifdef WAIT_FOR_COMPLETION
    timeout = 1000;

    while (0 == waitpid(my_pid , &status , WNOHANG)) {
            if ( --timeout < 0 ) {
                    perror("timeout");
                    return -1;
            }
            sleep(1);
    }

    printf("%s WEXITSTATUS %d WIFEXITED %d [status %d]\n",
            argv[0], WEXITSTATUS(status), WIFEXITED(status), status);

    if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
            perror("%s failed, halt system");
            return -1;
    }

#endif
    return 0;
}