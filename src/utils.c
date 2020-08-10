#include "utils.h"

char *read_file(const char *file_name)
{
    FILE *file;
    char *data = NULL;

    file = fopen(file_name, "rb");
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    u32 length = ftell(file);
    rewind(file);

    data = (char*)malloc(sizeof(char) * (length+1));
    fread(data, sizeof(char), length, file);
    data[length] = '\0';

    fclose(file);

    return data;
}
