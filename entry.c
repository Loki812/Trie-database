//
// filename: entry.c
//
// author: Connor McRoberts cjm6653@rit.edu
//
// description: made to be the IPV4 'entry' into the trie
// and operations that can be done on it.
//
//////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "entry.h"


/// Creates an entry out of character input, the expected input
/// will create two entries, thus the 'tf' int
///
/// @param e the entry (which has already been dynamically alloced)
/// @param input a string which contains an ip address, 
/// @param tf if 1, stores the ip_to address, if 0, stores the ip_from
/// address
///

void entry_init(Entry e, char* input, int tf) {
    const char delim[2] = ",";
    char *token;
    char *ptr;
    char *buffer = (char*) malloc(sizeof(char) * 256);
    strcpy(buffer, input);


    token = strtok(buffer, delim);

    if(tf) // tf == 1
    e->key = (unsigned int) strtol(token + 1, &ptr, 10);

    token = strtok(NULL, delim);

    if(!tf)  // tf == 0  
    e->key = (unsigned int) strtol(token + 1, &ptr, 10);

    token = strtok(NULL, delim);

    e->cc = (char *) malloc(sizeof(char) * 3);
    strncpy(e->cc, token + 1, strlen(token) - 2);

    token = strtok(NULL, delim);

    e->name = (char *) malloc(sizeof(char) * (strlen(token) - 2));
    strncpy(e->name, token + 1, strlen(token) - 2);

    token = strtok(NULL, delim);

    e->province = (char *) malloc(sizeof(char) * (strlen(token) - 2));
    strncpy(e->province, token + 1, strlen(token) - 2);

    token = strtok(NULL, delim);

    

    e->city = (char *) malloc(sizeof(char) * (strlen(token) - 2));
    strncpy(e->city, token + 1, strlen(token) - 3);

    free(buffer);
}

/// Creates an entry by calling the above function, and 
/// dynamically allocating it.
/// @param e the entry (which has already been dynamically alloced)
/// @param input a string which contains an ip address, 
/// @param tf if 1, stores the ip_to address, if 0, stores the ip_from
/// address
///
/// @return the created entry
///

Entry entry_create(char* input, int tf) {
    Entry  tmp = (Entry) malloc( sizeof(struct Entry_s) );

    // fills the Entry data members with CSV input
    entry_init(tmp, input, tf);

    return tmp;
}


/// Frees all dynamically allocated parts of the entry
/// then frees the pointer to the entry itself 
/// 
/// @param e the entry to be destroyed

void entry_destroy(Entry e) {
    if(e != NULL) {
        free(e->cc);
        free(e->name);
        free(e->province);
        free(e->city);
        free(e);
    }
}


///
/// Prints off the entry in the format specified int the 
/// write up.
///
/// @param e the entry to be printed
/// @param stream the stream to which we are printing to

void entry_print(Entry e, FILE *stream) {
    unsigned char bytes[4];
    bytes[0] = e->key & 0xFF;
    bytes[1] = (e->key >> 8) & 0xFF;
    bytes[2] = (e->key >> 16) & 0xFF;
    bytes[3] = (e->key >> 24) & 0xFF;
    fprintf(stream, "%u:  ", e->key);
    fprintf( stream, "(%d.%d.%d.%d, %s:  %s, %s, %s)\n",
    bytes[3], bytes[2], bytes[1], bytes[0], e->cc, e->name, 
    e->city, e->province);
}

///
/// creates a new dynamically allocated entry. used for when collisions
/// happen in node_insert
///
/// @param n the entry to be copied
/// @return a node with datamembers with the same value as n

Entry copy_entry(Entry n) {
    Entry tmp = (Entry) malloc(sizeof(struct Entry_s));
    tmp->key = n->key;


    tmp->cc = (char*) malloc(sizeof( char ) * 3);
    strcpy(tmp->cc, n->cc);
    tmp->city = (char*) malloc(sizeof(char) * (strlen(n->city) + 1));
    strcpy(tmp->city, n->city);
    tmp->name = (char*) malloc(sizeof(char) * (strlen(n->name) + 1));
    strcpy(tmp->name, n->name);
    tmp->province = (char*) malloc(sizeof(char) * 
    (strlen(n->province) + 1));

    strcpy(tmp->province, n->province);

    return tmp;
}
