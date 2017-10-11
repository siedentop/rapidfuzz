#include <rapidcheck.h>

#include "MapParser.h"

using namespace rc;

//int main() {
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  rc::check("serializing and then parsing should yield original map",
            [](const std::map<std::string, std::string> &map) {
              
              // RC_PRE: handle by inverting condition and returning if true.
              if(map.find("") != map.end()) return true;
              assert(parseMap(mapToString(map)) == map);
              return true;
            }, Data, Size);
    return 0;
}


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//extern int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size);
__attribute__((weak)) extern int LLVMFuzzerInitialize(int *argc, char ***argv);
int main(int argc, char **argv) {
    fprintf(stderr, "StandaloneFuzzTargetMain: running %d inputs\n", argc - 1);
//    if (LLVMFuzzerInitialize)
//        LLVMFuzzerInitialize(&argc, &argv);
    for (int i = 1; i < argc; i++) {
        fprintf(stderr, "Running: %s\n", argv[i]);
        FILE *f = fopen(argv[i], "r");
        assert(f);
        fseek(f, 0, SEEK_END);
        size_t len = ftell(f);
        fseek(f, 0, SEEK_SET);
        unsigned char *buf = (unsigned char*)malloc(len);
        size_t n_read = fread(buf, 1, len, f);
        assert(n_read == len);
        LLVMFuzzerTestOneInput(buf, len);
        free(buf);
        fprintf(stderr, "Done:    %s: (%zd bytes)\n", argv[i], n_read);
    }
}
