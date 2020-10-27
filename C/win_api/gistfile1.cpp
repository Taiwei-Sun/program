#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <algorithm>
#include <iterator>

template <class InIter1, class InIter2, class OutIter>
void find_all(unsigned char *base, InIter1 buf_start, InIter1 buf_end, InIter2 pat_start, InIter2 pat_end, OutIter res) {
    for (InIter1 pos = buf_start;
        buf_end!=(pos=std::search(pos, buf_end, pat_start, pat_end));
        ++pos)
    {
        *res++ = base+(pos-buf_start);
    }
}

template <class outIter>
void find_locs(HANDLE process, std::string const &pattern, outIter output) {

    unsigned char *p = NULL;
    MEMORY_BASIC_INFORMATION info;

    for ( p = NULL;
        VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info);
        p += info.RegionSize ) 
    {
        std::vector<char> buffer;
        std::vector<char>::iterator pos;

        if (info.State == MEM_COMMIT && 
            (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE)) 
        {
            DWORD bytes_read;
            buffer.resize(info.RegionSize);
            ReadProcessMemory(process, p, &buffer[0], info.RegionSize, &bytes_read);
            buffer.resize(bytes_read);
            find_all(p, buffer.begin(), buffer.end(), pattern.begin(), pattern.end(), output);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <process ID> <pattern>", argv[0]);
        return 1;
    }

    int pid;
    sscanf(argv[1], "%i", &pid);

    std::string pattern(argv[2]);

    HANDLE process = OpenProcess( 
        PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, 
        false,
        pid);

    find_locs(process, pattern, 
        std::ostream_iterator<void *>(std::cout, "\n"));

    return 0;
}