#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stack>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define iint int32_t

using namespace std;

void check(bool expr, const char *s) {
    if (expr) {
        perror(s);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./vm <progfile.sbc>" << endl;
        exit(EXIT_FAILURE);
    }

    // == Output is unbuffered ==
    setvbuf(stdout, NULL, _IONBF, 0);

    int ret;

    // == Map the input file ==
    char *addr;
    int fd;
    struct stat sb;

    // Open file.
    // TODO: Check O_DIRECT, O_SYNC:
    // http://stackoverflow.com/questions/5055859/how-are-the-o-sync-and-o-direct-flags-in-open2-different-alike
    fd = open(argv[1], O_RDONLY | O_NOATIME);
    check(fd == -1, "Couldn't open file.");

    // Obtain file size.
    ret = fstat(fd, &sb);
    check(ret < 0, "Couldn't fstat file.");

    // Do the mapping.
    // TODO: check other MAP flags
    addr = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0));
    check(MAP_FAILED == addr, "mmap failed.");
    
    //char op[5];
    //op[4] = '\0';
    for (int i = 0; i < sb.st_size; ++i) {
        printf("%02X", addr[i]);
        //memcpy(op, addr + 4 * i, 4);
        //printf("%ld ", strtol(op, NULL, 16));
    }
    printf("\n");

    // Create a stack using STL. Concerning the underlying
    // container, notice that <vector> uses a memory allocator
    // that forces all elements to be continuous,
    // while <deque> imposes no such constraints.
    stack<iint> s;

    ret = munmap(addr, sb.st_size);
    check(ret < 0, "Failure on unmapping.");
    ret = close(fd);
    check(ret < 0, "Failure on closing input file.");
}
