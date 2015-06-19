#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

#define EXIT_FAILURE 1

int main(int argc, const char *argv[])
{
    int choice;
    while (1)
    {
        static struct option long_options[] =
        {
            /* Use flags like so:
            {"verbose",	no_argument,	&verbose_flag, 'V'}*/
            /* Argument styles: no_argument, required_argument, optional_argument */
            {"version", no_argument,	0,	'v'},
            {"help",	no_argument,	0,	'h'},
            {"name",	required_argument,	0,	'n'},
            {0,0,0,0}
        };

        int option_index = 0;

        /* Argument parameters:
            no_argument: " "
            required_argument: ":"
            optional_argument: "::" */

        choice = getopt_long( argc, argv, "vhn:",
                    long_options, &option_index);

        if (choice == -1)
            break;
        switch( choice )
        {
            case 'v':
                break;
            case 'h':
                break;
            case 'n':
                printf("name: %s\n",optarg);
            case '?':
                /* getopt_long will have already printed an error */
                break;
            default:
                /* Not sure how to get here... */
                return EXIT_FAILURE;
        }
    }

    printf("optind: %d[%s] argc: %d\n", optind,argv[optind],argc);

    /* Deal with non-option arguments here */
    if ( optind < argc )
    {
        while ( optind < argc ){
            optind++;
        }
    }
    return 0;
}
