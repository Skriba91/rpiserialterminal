#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


int main(int argc, char *argv[])
{
    //If there are no arguments
    if(argc < 2)
    {
        //Check if configuration file is provided
        FILE* conf = fopen("rpiserial.conf", "r");
        //Check if the file exists
        if(conf == NULL)
        {
            //Error message and guidance
            printf("Error: rpiserial.conf not found.\nPlease provide a"
                    "configuration file or give command line argument"
                    "erguments.\nType 'defaultconf' to create default"
                    "configuration file.\n");
            return 1;
        }
        else {
            //TODO: read configuration file
        }
    }
    //If there is one input argument
    else if(argc == 2)
    {
        if(strcmp(argv[1], "defaultconf") == 1)
        {
            FILE* conf = fopen("rpiserial.conf", "w");
            if(conf == NULL)
            {
                printf("Error: rpiserial.conf not found.\n");
                return 1;
            }
            else {
                fputs("{ \"device\" : \"/dev/tty0\",\n"
                        "\t\"baud\" : 9600,\n"
                        "\t\"databits\" : 8,\n"
                        "\t\"stopbits\" : 1,\n"
                        "\t\"parity\" : \"none\",\n"
                        "\t\"flowcontrol\" : \"none\",\n"
                        "}", conf);
            }
        }
    }
    return 0;
}