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

static int exec_prog(const char **argv, bool waitForCompletion);
const char** getArgVfromString(const char* args);

void handlePromptLine(string raw_command);
void signal_callback_handler(int signum);

std::string trim(const std::string &str);

void execute_programs(const string &pString);

int splitParalell(const char *pString[], const string &somethingelse);

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_callback_handler);
    int opt;
    while ((opt = getopt(argc, argv, "")) != -1) {
        switch (opt) {
            default:
                fprintf(stderr, "Usage: %s [file...]\n", argv[0]);
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

void handlePromptLine(string raw_command) {
    if (raw_command == "exit") {
        exit(EXIT_SUCCESS);
    }
    else{
        execute_programs(raw_command);
    }

}

std::string trim(const std::string &str) {


    string whitespace = "\t";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) {
        return "";
    } // no content
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void execute_programs(const string &pString) {
    const char **programsToExecute = static_cast<const char **>(malloc(10 * sizeof(char *)));
    for (int i = 0; i < 30; ++i)
        programsToExecute[i] = static_cast<const char *>(malloc(20 * sizeof(char)));

    int numberAdded = splitParalell(programsToExecute, pString);
    int pids[55];
    if (numberAdded > 1) {
        cout << endl;
        printf("Added %i commands ", numberAdded);
        cout << endl;
        for (int i = 0; i < numberAdded; i++) {
            printf("Executing: [%s]", programsToExecute[i]);
            cout << endl;
        }
        for (int i = 0; i < numberAdded; i++) {
            const char **argV = getArgVfromString(programsToExecute[i]);
            for (int j = 0; j < sizeof(argV); j++) {
                cout << endl;
                printf("[%s]", argV[j]);
            }
            pids[i] = exec_prog(argV, false);
        }
    }
    for (int i = 0; i < numberAdded; i++) {
        wait(&pids[i]);
    }
    exec_prog(getArgVfromString(pString.c_str()), true);

}

int splitParalell(const char *pString[], const string &somethingelse) {
    int i = 0;
    char *string1 = strdup(somethingelse.c_str());
    while ((pString[i] = trim((string) strsep(&string1, "&")).c_str()) != NULL) {
        i++;

    }
    return i;

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

static int exec_prog(const char **argv, bool waitForCompletion)
{
    pid_t   my_pid;
    int     status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;
    if (0 == (my_pid = fork())) {
        char buffer[256]; // <- danger, only storage for 256 characters.
        strncpy(buffer, "/bin/", sizeof(buffer));
        strncat(buffer,  argv[0], sizeof(buffer));
        if (-1 == execve(buffer, (char **)argv , NULL)) {
            perror("child process execve failed");
            return -1;
        }
    }
    if (waitForCompletion)
        wait(&my_pid);

    return my_pid;
}