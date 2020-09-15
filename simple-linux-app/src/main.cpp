
// C++ headers.
#include <sstream>
#include <string>
#include <stdexcept>
#include <thread>

// C headers.
#include <math.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

/** Prints verious system info. */
void print_info() {
    // Get processor count.
    int processors = get_nprocs_conf();

    char name[32];
    gethostname(name, 32);
    name[31] = '\0';

    struct utsname platform;
    uname(&platform);

    // Get sysinfo struct.
    struct sysinfo info;
    sysinfo(&info);

    // Print info.
    printf("System info:\n");
    printf("%d CPU cores in total.\n", processors);
    printf("'%s' is the hostname of this computer.\n", name);
    printf("%s is the hardware platform.\n", platform.machine);
    printf("%lu MB of total system memory.\n", info.totalram/1000000);
}

/** Task run on each fork. */
void task() {
    long double sum = 0;
    for (int i = 1; i <= 25000; i++) {
        for (int j = 1; j <= i; j++) {
            sum += sqrtl((long double)(i*j));
        }
    }
    printf("Sum: %Lf\n", sum);
}

/** Initiates x forks to execute a task. */
void fork_master(int x) {
    if (x < 1) {
        printf("Invalid number of forks.\n");
        exit(EXIT_SUCCESS);
    }
    for (int i = 1; i <= x; i++) {
        // If child.
        if (fork() == 0) {
            printf("Hello I am fork I have PID: %d\n", getpid());
            task();
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 1; i <= x; i++) {
        wait(NULL);
    }
}

/** Initiates x threads to execute a task. */
void thread_master(int x) {
    if (x < 1) {
        printf("Invalid number of forks.\n");
        exit(EXIT_SUCCESS);
    }

    std::thread* threads = new std::thread[x]();
    
    for (int i = 0; i < x; i++) {
        threads[i] = std::thread(task);
        std::ostringstream id;
        id << threads[i].get_id();
        printf("Hello I am thread I have ID: %s\n", id.str().c_str());
    }
    for (int i = 0; i < x; i++) {
        threads[i].join();
    }

}

int main(int argc, char *argv[])
{
    if (argc == 1) {
            printf("Usage:\n");
            printf("    -i          Info about this computer.\n");
            printf("    -f <num>    Spawns number of worker forks.\n");
            printf("    -t <num>    Spawns number of worker threads.\n");
    }
    // For each arg in argv
    for (int i = 0; i < argc; i++) {

        //printf("%s\n", argv[i]);

        std::string flag = argv[i];

        // Info dump.
        if (flag == "-i" ) {
            print_info();
        }
        // Fork master.
        else if (flag == "-f") {
            // Sanity check.
            if (argc < i+2) {
                printf("Too few arguments.\n");
                break;
            }

            // Use try so conversion error can be caught.
            try {
                // Execute fork mastery.
                fork_master(std::stoi(argv[i+1]));
            }
            catch (const std::invalid_argument& ia) {
                printf("X is not a number.\n");
            }
        }
        // Thread master.
        else if (flag == "-t") {
            // Sanity check.
            if (argc < i+2) {
                printf("Too few arguments.\n");
                break;
            }

            // Use try so conversion error can be caught.
            try {

                // Execute fork mastery.
                thread_master(std::stoi(argv[i+1]));
            }
            catch (const std::invalid_argument& ia) {
                printf("X is not a number.\n");
            }
        }
    }
    exit(EXIT_SUCCESS);

}
