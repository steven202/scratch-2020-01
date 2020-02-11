#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


void
assert_ok(long rv, char* call)
{
    if (rv < 0) {
        fprintf(stderr, "failed call: %s\n", call);
        perror("error");
        exit(1);
    }
}

int
main(int _ac, char* _av[])
{
    int size = 10 * sizeof(int);

    // Open a file
    int fd = open("./array.mem", O_CREAT|O_TRUNC|O_RDWR, 0644);
    assert_ok(fd, "open");

    int rv = ftruncate(fd, size);
    assert_ok(rv, "ftruncate");

    int* xs = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    assert_ok((long) xs, "mmap");

    for (int ii = 0; ii < 10; ++ii) {
        xs[ii] = ii * ii;
    }

    rv = munmap(xs, size);
    assert_ok(rv, "munmap");

    rv = close(fd);
    assert_ok(rv, "close");

    return 0;
}