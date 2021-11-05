#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "./src/rpiserial.h"


int main(int argc, char *argv[]) {

    //If there are no arguments
    if(argc < 2) {
        //Check if configuration file is provided
        FILE* conf = fopen("rpiserial.conf", "r");
        //Check if the file exists
        if(conf == NULL) {
            //Error message and guidance
            printf("\nError: rpiserial.conf not found.\n\nPlease provide a"
                    "configuration file or give command line argument"
                    "erguments.\n\nType 'defaultconf' to create default"
                    "configuration file.\n\nUsage: <default>\n"
                    "1: device\n"
                    "2: baud <9600>\n"
                    "3: databits <8>\n"
                    "4: stopbits <1>\n"
                    "5: parity <none>\n"
                    "6: flowcontrol <none>\n");
            return 1;
        }
        else {
            //TODO: read configuration file
        }
    }
    //If there is one input argument
    else if(argc == 2) {
        //Check if the argument is 'defaultconf'. If it is create default
        //configuration file
        if(strcmp(argv[1], "defaultconf") == 0) {
            FILE* conf = fopen("rpiserial.conf", "w");
            if(conf == NULL) {
                printf("Error: rpiserial.conf not found.\n");
                return 1;
            }
            else {
                const char configuration[] =
                        "{\n\t\"device\" : \"/dev/tty0\",\n"
                        "\t\"baud\" : 9600,\n"
                        "\t\"databits\" : 8,\n"
                        "\t\"stopbits\" : 1,\n"
                        "\t\"parity\" : \"none\",\n"
                        "\t\"flowcontrol\" : \"none\",\n"
                        "}";
                fprintf(conf, "%s", configuration);
                printf("Configuration file created: rpiserial.conf\n%s\n",
                        configuration);
                fclose(conf);
            }
        }
        //If the argument is not 'defaultconf' then it is a device
        else {
            struct rpiserial_conf config;
            strcpy(config.device, argv[1]);
            config.baud = 9600;
            config.databits = 8;
            config.stopbits = 1;
            strcpy(config.parity, "none");
            strcpy(config.flowcontrol, "none");
        }
    }
    //If there are two input arguments, device and baud
    else if(argc == 3) {
        struct rpiserial_conf config;
        strcpy(config.device, argv[1]);
        config.baud = atoi(argv[2]);
        config.databits = 8;
        config.stopbits = 1;
        strcpy(config.parity, "none");
        strcpy(config.flowcontrol, "none");
    }
    //If there are three input arguments, device, baud and databits
    else if(argc == 4) {
        struct rpiserial_conf config;
        strcpy(config.device, argv[1]);
        config.baud = atoi(argv[2]);
        config.databits = atoi(argv[3]);
        config.stopbits = 1;
        strcpy(config.parity, "none");
        strcpy(config.flowcontrol, "none");
    }
    //If there are four input arguments, device, baud, databits and stopbits
    else if(argc == 5) {
        struct rpiserial_conf config;
        strcpy(config.device, argv[1]);
        config.baud = atoi(argv[2]);
        config.databits = atoi(argv[3]);
        config.stopbits = atoi(argv[4]);
        strcpy(config.parity, "none");
        strcpy(config.flowcontrol, "none");
    }
    //If there are five input arguments, device, baud, databits, stopbits and
    //parity
    else if(argc == 6) {
        struct rpiserial_conf config;
        strcpy(config.device, argv[1]);
        config.baud = atoi(argv[2]);
        config.databits = atoi(argv[3]);
        config.stopbits = atoi(argv[4]);
        strcpy(config.parity, argv[5]);
        strcpy(config.flowcontrol, "none");
    }
    //If there are six input arguments, device, baud, databits, stopbits, parity
    //and flowcontrol
    else if(argc == 7) {
        struct rpiserial_conf config;
        strcpy(config.device, argv[1]);
        config.baud = atoi(argv[2]);
        config.databits = atoi(argv[3]);
        config.stopbits = atoi(argv[4]);
        strcpy(config.parity, argv[5]);
        strcpy(config.flowcontrol, argv[6]);
    }
    return 0;
}