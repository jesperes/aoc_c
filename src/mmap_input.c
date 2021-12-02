#include "mmap_input.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct mmapping mmap_input_file(int year, int day) {
    struct mmapping m;
    char *filename;
    if (asprintf(&filename, "inputs/%d/input%02d.txt", year, day) == -1) {
        perror("asprintf");
        abort();
    }

    m.fd = open(filename, O_RDONLY);
    if (m.fd == -1) {
        perror("open");
        abort();
    }

    free(filename);

    struct stat st = {0};
    if (fstat(m.fd, &st) == -1) {
        perror("fstat");
        abort();
    }

    m.len = st.st_size;
    m.ptr = mmap(NULL, m.len, PROT_READ | PROT_WRITE, MAP_PRIVATE, m.fd, 0);
    if (m.ptr == MAP_FAILED) {
        perror("mmap");
        abort();
    }

    // file descriptor can be closed immediately
    if (close(m.fd) == -1) {
        perror("close");
        abort();
    }

    return m;
}

void unmap_input_file(struct mmapping m) {
    if (munmap(m.ptr, m.len) == -1) {
        perror("munmap");
        abort();
    }
}
