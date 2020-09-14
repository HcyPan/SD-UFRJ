#include <signal.h>
#include <iostream>

using namespace std;

// Select and send signal
void sendSig(pid_t pid){
    int signal;
    int signum;

    cout << "Select singal: \n";
    cout << "1- Signal 1 \n";
    cout << "2- Signal 2 \n";
    cout << "3- Signal 3 - finish process! \n";
    cin >> signal;
    cout << "Signal " << signal << " sent!\n";

    switch (signal)
    {
    case 1:
        signum = SIGUSR1;
        break;

    case 2:
        signum = SIGUSR2;
        break;
    
    case 3:
        signum = SIGINT;
        break;
    
    default:
        break;
    }

    kill(pid, signum);
    if(signum == SIGINT){
        exit(0);
    }
}

// Target process PID input
pid_t getPid(){
    pid_t pid;
    cout << "Type target process id: ";
    cin >> pid;
    cout << "Process number " << pid << " selected.\n";
    return pid;
}

int main(){
    pid_t pid = getPid();

    // recursive call for valid target PID
    while(kill(pid, 0) == -1){
        cout << "No process with given PID \n";
        pid = getPid();
    }
    
    // Send signal loop;
    while(1){
        sendSig(pid);
    }
}