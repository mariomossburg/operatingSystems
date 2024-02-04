
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


//declared functions
int inputUser();
void lineBreak();
void switchCase(int childNumber);
void resultsSwitchCase(int childNumber);
void createChildProcess(int childNumber);
void waitChildProcess(int processes);
void parallelProcess(int processes);

int factorial(int n);
int fibonacci(int n);
int areaOfTriangle(int base, int height);
int cubed(int n);

//main
int main(){
    int userValue;

    printf("This application is live");
    lineBreak();


    userValue = inputUser(userValue);
    lineBreak();

    createChildProcess(userValue);
    lineBreak();
    waitChildProcess(userValue);
   
    lineBreak();
    return 0;
}//end main

//functions
int inputUser(int userInput){
    int inputCheck = 0;

while(inputCheck == 0){
     printf("Please provide the number of children processes to create(less than 5): ");
    lineBreak();

    if(scanf("%d", &userInput) == 1 && userInput < 5){
        inputCheck = 1;
    }else{
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}
    return userInput;
}

void lineBreak(){
    printf("\n");
}

void switchCase(int childNumber) {
    // Child process
    switch (childNumber) {
        case 1:
            printf("Child %d (PID: %d) is computing the factorial of 5.\n", childNumber, getpid());
            break;
        case 2:
            printf("Child %d (PID: %d) is finding fibonacci with value of 5.\n", childNumber, getpid());
            break;
        case 3:
            printf("Child %d (PID: %d) is performing area of triangle with base and height of 5.\n", childNumber, getpid());
            break;
        case 4:
            printf("Child %d (PID: %d) is finding the value of 5 cubed.\n", childNumber, getpid());
            break;
        default:
            break;
    }
}
void resultsSwitchCase(int childNumber) {
    int a, b, c, d;
    // Child process
    switch (childNumber) {
        case 1:
            a = factorial(5);
            printf("Child %d (PID: %d) completed its task. Result: %d\n", childNumber, getpid(), a);
            break;
        case 2:
            b = fibonacci(5);
            printf("Child %d (PID: %d) completed its task. Result: %d\n", childNumber, getpid(), b);
            break;
        case 3:
            c = areaOfTriangle(5,5);
            printf("Child %d (PID: %d) completed its task. Result: %d.\n", childNumber, getpid(), c);
            break;
        case 4:
            d = cubed(5);
        printf("Child %d (PID: %d) completed its task. Result: %d.\n", childNumber, getpid(), d);
            break;
        default:
            break;
    }
    exit(0);
}

void createChildProcess(int childNumber) {
    // Parent process
    printf("Parent process (PID: %d) is creating %d child processes.", getpid(), childNumber);
    lineBreak();

    pid_t pid;
    for (int i = 1; i <= childNumber; i++) {
        pid = fork();
        if (pid == 0) {
            //child process
            switchCase(i);
            resultsSwitchCase(i);
        }
    }

    //Wait for all child processes to finish
    for (int i = 0; i < childNumber; i++) {
        wait(NULL);
    }
}

void waitChildProcess(int processes){
    
        wait(NULL);
        printf("All children processes have completed. Parent (PID: %d) is displaying the final message, ", getpid());
        exit(0);
    
}


//different computationally intensive operations listed below
int factorial(int n){
    int result = 1;
    //int i;

    for(int i = 2; i <= n; i++){
        result *= i;
    }
    return result;
}

int fibonacci(int n){
    int a=0;
    int b=1;
    int c;
    int i;

    if (n == 0)
        return a;
    for(i = 2; i <=n; i++){
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}
int areaOfTriangle(int base, int height){
    int sum = (base * height) * 0.5;
    return sum;
}

int cubed(int n){
    int sum = (n * n * n);
    return sum;
}