
struct mmapping {
    void *ptr;
    int len;
    int fd;
};

struct mmapping mmap_input_file(int year, int day);
void unmap_input_file(struct mmapping m);
