#include "fnmtch.h"

/**
* Read the content of the file line by line, and trying to match the pattern
* @param filename to be open
* @param pattern to match
* @return void
*/
int matchPattern(const char *filename, const char *pattern)
{
    FILE *fp = fopen(filename, "r");
    if(!fp)
    {
        perror("Can not open file");
        return EXIT_FAILURE;
    }

    char * line = NULL;
    size_t len = 0;

    //Read line by line and trying to find the pattern
    while ( getline(&line, &len, fp) != -1)
    {
        int r = fnmatch(pattern,line,0);
        if( r==0 )
        {
            fprintf(stdout, "%s", line );
        }
        else if( r==FNM_NOMATCH )
        {
            continue;
        }
        else
        {
            fprintf(stderr,"fnmatch() error\n");
        }
    }

    if(line)
        free(line);
    fclose(fp);
    return EXIT_SUCCESS;
}

/**
* Parse the arguments passed from the terminal
* @param argument count
* @param array of pointers to the arguments
* @param pattern to match
* @param filename to open
* @return void
*/
void parseCmdArgs(int argc, char *argv[], char **pattern, char **filename)
{
    int c;

    while (1)
    {
        static struct option long_options[] =
            {
                /* These options set a flag. */
                {"pattern",  required_argument, 0, 'p'},
                {"file",    required_argument, 0, 'f'},
                {0, 0, 0, 0}
            };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "k:f:",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;

        case 'p':
            *pattern = optarg;
            break;

        case 'f':
            *filename = optarg;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            fprintf(stderr, "Usage: %s --key keyName --file inputFile.csv\n", argv[0]);
            exit(EXIT_FAILURE);
            break;

        default:
            abort ();
        }
    }
}
