#include <signal.h>
#include <iostream>
#include <unistd.h> 

using namespace std;

static volatile sig_atomic_t sig_caught = 0;

// Signal handler
void signal_handler( int signum ){
    sig_caught = 1;
    if(signum == SIGUSR1){
        cout << "Signal 1 selected \n";
    } else if(signum == SIGUSR2){
        cout << "Signal 2 selected \n";
    } else if (signum == SIGINT){
        cout << "Process terminated \n";
        exit(signum);
    }
}

int main(){
    // signal handler call
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGINT, signal_handler);

    sigset_t newMask;
    sigdelset(&newMask, SIGUSR1);
    sigdelset(&newMask, SIGUSR2);
    sigdelset(&newMask, SIGINT);
    int wait_type;

    // Print this process PID
    cout << "PID: " << getpid() << "\n";

    cout << "Select waiting type: \n";
    cout << "1- Blocking wait \n";
    cout << "2- Busy wait \n";
    cin >> wait_type;
    cout << "Waiting type " << wait_type << " selected \n";

    while(1){
        // Block waiting
        if(wait_type == 1 ){
            while(sig_caught == 0){
                cout << "I am blocked now! \n";
                sigsuspend(&newMask);
            } 
            sig_caught = 0;

        } 
        // Busy waiting
        else if(wait_type == 2){
            while (sig_caught == 0){
                cout << "I am busy waiting! \n";
                sleep(1);
            }
            sig_caught = 0;
        }
    }
}
