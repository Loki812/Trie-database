// 
// File: trie.h 
// Starter trie module for an integer-keyed trie data type ADT
// @author CS@RIT.EDU
// @author Connor McRoberts cjm6653@rit.edu
// // // // // // // // // // // // // // // // // // // // // // // // 

#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include "entry.h"


typedef unsigned int ikey_t;


/// TODO define the data structure to represent the payload


/// Trie is a pointer to the Trie ADT

typedef struct Trie_s * Trie;





// constant values for bit processing available to application

extern const size_t BITSPERBYTE;        ///< number of bits in a byte
extern const size_t BITSPERWORD;        ///< number of bits in a word
extern const size_t BYTESPERWORD;       ///< number of bytes in a word
extern const size_t RADIX;              ///< number of possible key values

/// Create a Trie instance and initialize its fields.
/// @param TODO ...
/// @return pointer to the Trie object instance or NULL on failure
/// @post Trie is NULL on failure or initialized with a NULL trie root

Trie ibt_create( void );

/// Destroy the trie and free all storage.
/// Uses Trie's Delete_value function to free app-specific (key and) value;
/// If the Trie's Delete_value function is NULL,
/// then call free() on the value in (key,value).
/// @param trie a pointer to a Trie instance
/// @pre trie is a valid Trie instance pointer
/// @post the storage associated with the Trie and all data has been freed

void ibt_destroy( Trie trie);

/// insert an entry into the Trie as long as the entry is not already present
/// @param trie a pointer to a Trie instance
/// @param e the entry to be inserted into the trie
/// @post the trie has grown to include a new entry IFF not already present

void ibt_insert( Trie trie, Entry e);



/// search for the key in the trie by finding
/// the closest entry that matches key in the Trie.
/// @param trie a pointer to a Trie instance
/// @param key the key to find 
/// @return entry representing the found entry or a null entry for not found

Entry ibt_search( Trie trie, ikey_t key);

/// get the size of the trie or number of leaf elements
/// @param trie a pointer to a Trie instance
/// @return size of trie 

size_t ibt_size( Trie trie);

/// get the node count of the trie: the number of internal nodes
/// @param trie a pointer to a Trie instance
/// @return the count of internal nodes

size_t ibt_node_count( Trie trie);

/// get height of the trie
/// @param trie a pointer to a Trie instance
/// @return height of trie

size_t ibt_height( Trie trie);


/// Perform an in-order traversal to show each (key, value) in the trie.
/// Uses Trie's Show_value function to show each leaf node's data,
/// and if the function is NULL, output each key and value in hexadecimal.
///
/// @param trie a pointer to a Trie instance
/// @param stream the stream destination of output

void ibt_show( Trie trie, FILE * stream);


/// Runs functions mentioned above to update the datamembers
/// inside the trie.
///
/// @param trie a pointer to a Trie instance
/// @post all data members like total_nodes, height, etc. have
/// been updated to current values.

void ibt_update(Trie trie);



#endif // TRIE_H
