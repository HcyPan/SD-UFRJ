#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// convert int to string
string int2str(int x){
    stringstream ss;
    ss << x;
    return ss.str();
}

// check if number is prime
bool isPrime(int num){
    bool flag=true;
    for(int i = 2; i <= num / 2; i++) {
       if(num % i == 0) {
          flag = false;
          break;
       }
    }
    return flag;
}

int main(){
    int     fd[2], nbytes, num, ni = 1, nf, nm;
    pid_t   childpid;
    char    stringbuff[20];
    char    stringzero[] = "0";
    char    readbuffer[20];
    string  strni = int2str(ni);

    // create pipe
    pipe(fd);

    cout << "Insert how many numbers to send: ";
    cin >> num;
    cout << "Ok! " << num << " random numbers will be sent \n";

    // check for fork error
    if((childpid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    for (int i = 0; i <= num; i++){
        if(childpid == 0)
        {
            // chield process -- producer

            // prevent read end
            // close(fd[0]);

            // delta changes every iteration
            int dt = rand() % 100;
            if (i == 0){
                // first number always is 1
                nf = ni + dt;
                nm = nf;
            } else{
                nf = nm + dt;
                nm = nf;
            }

            // create string with number
            string nstr = int2str(nf);
            
            // create the char buffer and send it
            if(i != num && i == 0){
                strni.copy(stringbuff, strni.size());
                cout << "The Producer sent number: " << nf << " \n";
                write(fd[1], stringbuff, 20);
            } else if(i != num && i != 0){
                nstr.copy(stringbuff, nstr.size());
                cout << "The Producer sent number: " << nf << " \n";
                write(fd[1], stringbuff, 20);
                // exit(0);
            } else if (i == num ){
                // last number always is 0
                write(fd[1], stringzero, 20);
            }
        }
        else
        {
            // father process - consumer

            // prevent write end
            // close(fd[1]);

            nbytes = read(fd[0], readbuffer, 20);

            // transform buffer array to int
            int res = atoi(readbuffer);
            if(res == 1){
                // first number is always 1
                cout << "First number received \n";
            } else if (res == 0 ){
                // finish when 0 is consumed
                cout << "Program finished \n";
                exit(1);
            } else {
                cout << "The Consumer received number: " << res << "\n";            
            }
            // check if is prime
            if (isPrime(res) && res != 0 && res != 1){
                cout << res << " is Prime! \n";
            }
        }
    }
}