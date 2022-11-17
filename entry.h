#ifndef _ENTRYH_
#define _ENTRYH_


typedef unsigned int ikey_t;

struct Entry_s {
    ikey_t key;              ///< unique identifier key
    char *cc;
    char *name;
    char *province;
    char *city;
};

typedef struct Entry_s * Entry;

void entry_init(Entry e, char* input, int tf);

void entry_destroy(Entry e);

void entry_print(Entry e, FILE *stream);

Entry entry_create(char* input, int tf);

Entry copy_entry(Entry n);


#endif
