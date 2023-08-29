#include "bsearch.h"

/**
* Print the CSV data to screen
* @param CSV structure details
* @return void
*/
void printCSV(csvFile_t * csv)
{
    for(size_t i = csv->range.keyIdexStart ; i <= csv->range.keyIndexEnd; ++i )
    {
        fprintf(stdout, "%s", csv->key[i]);
    }
}

/**
* Free the allocated lines of data
* @param csvFile_t structure
* @return void
*/
static void freeCSVStruct( csvFile_t * csv)
{
    for(int i = 0 ; i < csv->numOflLines ; ++i )
    {
        free(csv->key[i]);
    }
    free(csv);
}


/**
* Read the CVS file line by line and saving in memory for further computation
* @param filename from where data are going to be read
* @return csvFile_t structure pointer
*/
static csvFile_t* readCSVFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(!fp)
    {
        perror("Can not open file");
        return NULL;
    }

    char * line = NULL;
    size_t len = 0;
    csvFile_t *csvFile;

    if(!(csvFile = (csvFile_t*)malloc(sizeof( csvFile_t))))
    {
        fprintf(stderr, "Can not allocate memory.");
        fclose(fp);
        return NULL;
    }

    csvFile->numOflLines = 0;

    while ( getline(&line, &len, fp) != -1 )
    {
        size_t lineLen = strlen(line) + 1;
        if(!(csvFile->key[csvFile->numOflLines] = malloc(lineLen)))
        {
            freeCSVStruct(csvFile);
            fclose(fp);
            return NULL;
        }

        strncpy(csvFile->key[csvFile->numOflLines], line, lineLen );
        csvFile->key[csvFile->numOflLines][lineLen - 1] = '\0';

        ++csvFile->numOflLines;
    }

    if(line)
        free(line);

    fclose(fp);
    return csvFile;
}

/**
* optimisation of binary search algo for finding the left most key when there are duplicate elements
* @param csvFile_t structure
* @param key to be search
* @return index if the key is located or -1 if not found
*/
static int findLeftMostKey( const csvFile_t* csv, const char *key)
{
    int low = 0;
    int high = csv->numOflLines-1;
    int mid = 0;
    int index = -1;

    while(low <= high)
    {
        mid = low + (high - low) / 2;
        char *tmp = csv->key[mid];
        size_t keyLen = strlen(key);
        int retVal = strncmp(tmp, key, keyLen);

        if(!retVal )
        {
            index = mid;
            high = mid - 1;
        }
        else
        {
            if(retVal >= 1)
            {
                high = mid -1;
            }
            else
            {
                low = mid + 1;
            }
        }
    }
    return index;
}

/**
* optimisation of binary search algo for finding the right most key when there are duplicate elements
* @param csvFile_t structure
* @param key to be search
* @return index if the key is located or -1 if not found
*/
static int findRightMostKey( const csvFile_t* csv, const char *key)
{
    int low = 0;
    int high = csv->numOflLines-1;
    int mid = 0;
    int index = -1;

    while(low <= high)
    {
        mid = low + (high - low) / 2;
        char *tmp = csv->key[mid];
        size_t keyLen = strlen(key);
        int retVal = strncmp(tmp, key, keyLen);

        if(!retVal )
        {
            index = mid;
            low = mid + 1;
        }
        else
        {
            if(retVal >= 1)
            {
                high = mid -1;
            }
            else
            {
                low = mid + 1;
            }
        }
    }
    return index;
}

/**
* Searching for a key in a csv file sorted by the first column
* @param filename that needs to be searched
* @param key to be search
* @return EXIT_SUCCESS if key/s are found or EXIT_FAILURE if key is not found
*/
int bSearch( const char* filename, const char *key )
{

    csvFile_t *csv =  readCSVFile(filename);
    if(!csv)    return EXIT_FAILURE;

    //Find left most key index
    int index = findLeftMostKey(csv, key);
    if( index == -1 )
    {
        freeCSVStruct(csv);
        return EXIT_FAILURE;
    }

    csv->range.keyIdexStart = index;

    //after locating the left most key index find the right one
    csv->range.keyIndexEnd = findRightMostKey(csv, key);

    printCSV(csv);

    freeCSVStruct(csv);

    return EXIT_SUCCESS;
}

/**
* Parse the arguments passed from the terminal
* @param argument count
* @param array of pointers to the arguments
* @param OUT key to find
* @param OUT filename to open
* @return void
*/
void parseCmdArgs(int argc, char *argv[], char **key, char **filename)
{
    int c;

    while (1)
    {
        static struct option long_options[] =
            {
                /* These options set a flag. */
                {"key",  required_argument, 0, 'k'},
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

        case 'k':
            *key = optarg;
            break;

        case 'f':
            *filename = optarg;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            fprintf(stderr, "Usage: %s --key keyName inputFile.csv\n", argv[0]);
            exit(EXIT_FAILURE);
            break;

        default:
            abort ();
        }
    }
}
