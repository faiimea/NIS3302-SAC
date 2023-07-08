#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#define logpath "./bootlog"

using namespace std;

int main() {
    FILE* fp1,*fp2;
    char buffer1[1024],buffer2[1024],placeholder[1024];
    string week,month,boot_time,close_time,span;
    int day;

    fp1 = popen("last -x reboot", "r");
    fp2 = popen("last -x shutdown", "r");
    if (fp1 == NULL || fp2 == NULL) {
        std::cerr << "Error executing command" << std::endl;
        return 1;
    }

    FILE *logfile;
    logfile=fopen(logpath, "w+");
    if (logfile == NULL) {
        printf("Waring: cannot create log file\n");
        exit(1);
    }
    fprintf(logfile, "Operation\tDoW\tMonth\tDay\tTime\n\n");
    
    while (fgets(buffer1, sizeof(buffer1), fp1) != NULL && fgets(buffer2, sizeof(buffer2), fp2)) {
        istringstream iss1(buffer1),iss2(buffer2);
        if(iss1>>placeholder>>placeholder>>placeholder>>placeholder>>week>>month>>day>>boot_time>>placeholder>>close_time) {
            if (close_time != "running") {
                iss1 >> span;
                fprintf(logfile, "Boot:\t\t%s\t%s\t%d\t%s \n", week.c_str(), month.c_str(), day, boot_time.c_str());
            } else {
                fprintf(logfile, "Boot:\t\t%s\t%s\t%d\t%s\t|\tstill running\n", week.c_str(), month.c_str(), day,
                        boot_time.c_str());
            }
        }
        if(iss2>>placeholder>>placeholder>>placeholder>>placeholder>>week>>month>>day>>close_time>>placeholder>>boot_time>>span){
            fprintf(logfile,"Shutdown:\t%s\t%s\t%d\t%s\n",week.c_str(),month.c_str(),day,close_time.c_str());

        }

    }

    pclose(fp1);
    pclose(fp2);
    return 0;
}
