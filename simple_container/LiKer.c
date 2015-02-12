#define ContainerNameSize 10
#define ContainerRootfsPathSize 20

typedef struct Container{
    char name[ContainerNameSize];
    char rootfs[ContainerRootfsPathSize];
}Container;

Container * newContainer(char *name, char *rootfs){
    assert(name != NULL);
    assert(rootfs != NULL);

    if(strlen(name) > ContainerNameSize){
        
    }
    
}

void init(int args, const char *argv){
    
}

int main(int argc, const char *argv[])
{


    pid_t child_pid;
    child_pid = clone(childFunc,)
    return 0;
}
