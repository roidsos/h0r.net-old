#ifdef HEAP_ACCESSABLE
#include "tar.h"
#include "libk/vector.h"
// turns out, the idiots who designed tar, encoded the size into a base 8 string
// :facepalm:
uint32_t parse_size(uint8_t *in) {

    uint32_t size = 0;
    uint32_t j;
    uint32_t count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}
vector_static(struct tar_header *, headers);

struct tar_contents parse_tar(struct tar_header *header,
                              __attribute__((unused)) uint64_t file_size) {
    vector_init(&headers);

    uint32_t i = 0;

    struct tar_header *_header = header;

    while (true) {
        uint64_t traversed_size = (uint64_t)_header - (uint64_t)header;
        if (_header->filename[0] == 0 || traversed_size >= file_size) {
            struct tar_contents contents;
            contents.hdr_num = i;
            contents.headers = headers.data;
            return contents;
        }

        uint64_t size = parse_size(_header->size);

        vector_push_back(&headers, _header);

        _header =
            (struct tar_header *)((uint64_t)_header + ((size / 512) + 1) * 512);

        if (size % 512)
            _header = (struct tar_header *)((uint64_t)_header + 512);

        i++;
    }
}
struct tar_header *find_file(struct tar_contents *findin, char *name) {
    for (size_t i = 0; i < findin->hdr_num; i++) {
        struct tar_header *hdr = findin->headers[i];
        if (strcmp((char*)hdr->filename, name) == 0) {
            return hdr;
        }
    }
    return 0;
}
#endif