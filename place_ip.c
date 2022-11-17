//
// file-name: place_ip.c
// @author: Connor McRoberts cjm6653@rit.edu
// 
// flags to compile: -std=c99 -ggdb -Wall -Wextra
//
// description: place_ip.c is a file that takes a single command line
// arguement, that is suppose to be a filename. From there it builds 
// a trie struct with each line from the file being to entrys
// (showing the range of ip addresses). The user can then query the
// tree with IP addresses either in numberical notation '10234106'
// or IPV4 notation '120.0.0.255'
//
////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "entry.h"
#include "trie.h"

///
/// Converts character input from the user into 
/// a key used to grab IP addresses.
///
/// @param input: the user input from stdin (most likely)
/// @return a 32 bit unsigned integer meant to represent an ip address

ikey_t convert_to_key(char *input) {
    ikey_t key = 0;

    int is_ipv4 = 0;

    for(int i = 0; i < (int) strlen(input); i++) {
        if(input[i] == '.') {
            is_ipv4++;
        }
    }
    if(is_ipv4 != 0){
        char delim[2] = ".";
        char *token;
        u_int32_t val;
        token = strtok(input, delim);
        val = (u_int32_t) atoi(token);

        key |= (val << 24);

        token = strtok(NULL, delim);
        if(token == NULL) {
            return key;
        }
        val = (u_int32_t) atoi(token);
        key |= (val << 16);

        token = strtok(NULL, delim);
        if(token == NULL) {
            return key;
        }
        val = (u_int32_t) atoi(token);
        key |= (val << 8);

        token = strtok(NULL, delim);
        if(token == NULL) {
            return key;
        }
        val = (u_int32_t) atoi(token);
        key |= val;

        return key;
    }
    key = (ikey_t) atoi(input);
    return key;
    
}

///
/// main() takes a file, constructs the trie,
/// the allows the user to query either integer representations
/// or IPV4 representations of ip addresses
///
/// @param argc: the number of command line arguements
/// @param argv: the command line arguements
///
/// @return zero if successful, 1 if not
///
int main(int argc, char* argv[]) {

    if(argc != 2) {
        perror("usage: place_ip filename \n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if(fp == NULL) {
        fprintf(stderr, "%s: No such file or directory \n", argv[1]);
        return 1;
    }

    char *buffer;
    int bufsize = 256;
    buffer = (char*) malloc(sizeof(char) * bufsize);
    Trie trie = ibt_create();
    
    if(fgets(buffer, bufsize, fp) == NULL) {
        perror("error: empty dataset\n");
        return 1;
    }


    for(;;) {
        ibt_insert(trie, entry_create(buffer, 0));
        ibt_insert(trie, entry_create(buffer, 1));
        if(fgets(buffer, bufsize, fp) == NULL) {
            break;
        }
    }
    printf("\n");
    ibt_update(trie);
    printf("\n");
    printf("\n");

    int invalid = 0;
    ikey_t key;

    printf("Enter an ipv4 string or a number (or a blank line to quit).\n");
    printf("> ");

    if(fgets(buffer, bufsize, stdin) == NULL) {
        return 0;
    }
    while(strcmp(buffer, "\n") != 0) {
        for(int i = 0; i < (int) strlen(buffer) - 1; i++) {
            if(isalpha(buffer[i])) {
                printf("(INVALID, -: -, -, -)\n");
                invalid = 1;
                break;
            }
        }
        if(invalid == 0) {
            key = convert_to_key(buffer);
            entry_print(ibt_search(trie, key), stdout);
        }
        
        invalid = 0;
        printf("> ");
        if(fgets(buffer, bufsize, stdin) == NULL) {
            break;
        }
    }
    printf("\n");
    ibt_destroy(trie);

    return 0;
}
