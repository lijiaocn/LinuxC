#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#define VERSION_MAJOR  0
#define VERSION_MINOR  1
#define ContainerNameSize 10
#define ContainerRootfsPathSize 20
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define DEFAULT_STACK_SIZE (1024*1024*100)

typedef struct Container{
    uuid_t uuid;
    char name[ContainerNameSize];
    char rootfs[ContainerRootfsPathSize];
    int stacksize;
    char stack[0];
}Container;

void version(){
    printf("MAJOR: %d, MINOR: %d\n", VERSION_MAJOR, VERSION_MINOR);
    printf("BUILD TTIME: %s, Gcc: %s\n", __TIME__, __VERSION__);
}

void usage(char *argv0 ){
    printf("usage: %s [OPTION] ROOTFSPATH\n", argv0);
    printf("\t -v, --version:  display the version number\n");
    printf("\t -h, --help:     display the user manual\n");
    printf("\t -n, --name:     set the cotainer name, if you don't set ,it will be NULL\n");
    printf("\t -s, --stack:    container's stack size\n");
}

Container * newContainer(char *name, char *rootfs, int stacksize){
    assert(rootfs != NULL);
    assert(stacksize > 0);
    int ret;

    if(name == NULL){
        name = "NoName";
    }

    if(strlen(name) > ContainerNameSize){
        fprintf(stderr, "the name is too long\n");
        return NULL;
    }

    if(strlen(rootfs) > ContainerRootfsPathSize){
        fprintf(stderr, "the rootfs path is too long\n");
        return NULL;
    }

    if(opendir(rootfs) == NULL){
        syslog(LOG_ERR,"%s %s %s %d\n",strerror(errno), __FILE__,__func__,__LINE__);
        return NULL;
    }

    Container *ct = malloc(sizeof(Container)+stacksize);
    if(ct == NULL){
        syslog(LOG_ERR,"malloc fail %s %s %d\n", __FILE__,__func__,__LINE__);
        return NULL;
    }

    memset(ct, 0, sizeof(Container));

    ret = uuid_generate_time_safe(ct->uuid);
    if(ret == -1){
        free(ct);
        syslog(LOG_ERR,"uuid generate fail %s %s %s %d\n",strerror(errno), __FILE__,__func__,__LINE__);
        return NULL;
    }
    snprintf(ct->name,strlen(name),"%s",name);
    snprintf(ct->rootfs,strlen(rootfs),"%s",rootfs);
    ct->stacksize = stacksize;
    return ct;
}

int startContainer(void *arg){
    Container *ct = arg;
    execlp("chroot", "chroot",ct->rootfs, (char *)NULL);
    //execlp("sleep", "sleep","600", (char *)NULL);
    syslog(LOG_ERR," %s %s %s %d\n",strerror(errno),__FILE__,__func__,__LINE__);
    return 0;
}

void init(int args, char* const argv[]){
    //openlog(argv[0] , LOG_PERROR|LOG_PID , LOG_USER);
    openlog(argv[0] , LOG_PID|LOG_NDELAY , LOG_USER);
    syslog(LOG_INFO,"Start");
}

int main(int argc, char* const argv[])
{
    int retcode = EXIT_SUCCESS;
    init(argc, argv);
    int choice;
    Container *ct = NULL;
    char *ctname = NULL;
    char *rootfs = NULL;
    int stacksize = DEFAULT_STACK_SIZE;
    while (1){
        static struct option long_options[] ={
            /* Argument styles: no_argument, required_argument, optional_argument */
            {"version", no_argument,	0,	'v'},
            {"help",	no_argument,	0,	'h'},
            {"name",	required_argument,	0,	'n'},
            {"stack",	required_argument,	0,	's'},
            {0,0,0,0}
        };

        int option_index = 0;
        choice = getopt_long( argc, argv, "vhn:s:",long_options, &option_index);
        if (choice == -1) { break; }
        switch( choice ){
            case 'v':
                version(); 
                exit(0);
            case 'h':
                usage(argv[0]);   
                exit(0);
            case 'n':
                ctname = optarg;
                break;
            case 's':
                stacksize = atoi(optarg);
                break;
            case '?':
                /* getopt_long will have already printed an error */
                break;
            default:
                /* Not sure how to get here... */
                retcode = EXIT_FAILURE;
                goto EXIT;
        }
    }

    /* Deal with non-option arguments here */
    if(optind + 1 != argc){
        usage(argv[0]);
        retcode = EXIT_FAILURE;
        goto EXIT;
    }else{
        rootfs = argv[optind];
    }

    ct = newContainer(ctname, rootfs, stacksize);
    if(ct == NULL){
        retcode = EXIT_FAILURE;
        goto EXIT;
    }

    pid_t child_pid;
    child_pid = clone(startContainer, ct->stack + ct->stacksize, \
            CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWNS | CLONE_NEWPID \
            | CLONE_NEWUTS|SIGCHLD, ct);

    if(child_pid == -1){
        syslog(LOG_ERR, "clone fail %s %s %d\n",__FILE__,__func__,__LINE__);
        retcode = EXIT_FAILURE;
        goto EXIT;
    }

    if(waitpid(child_pid, NULL, 0) == -1){
        syslog(LOG_ERR, "waitpid fail %s %s %s %d\n",strerror(errno),__FILE__,__func__,__LINE__);
        retcode = EXIT_FAILURE;
        goto EXIT;
    }
    fprintf(stdout,"Finished!\n");

EXIT:
    if(ct != NULL){
        free(ct);
    }
    return retcode;
}
