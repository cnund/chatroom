#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, const char *argv[])
{
    char u[10] = "100";
    int a = atoi(u);
    printf("%d\n", a);
    return 0;
}
