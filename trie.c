// 
// File: trie.c 
// Starter trie module for an integer-keyed trie ADT data type
// @author CS@RIT.EDU
// @author Connor McRoberts cjm6653@rit.edu
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"
#include "entry.h"

#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)
#define MAX(x,y) ((x>y) ? x:y)

/////////////////////// Constants and struct definition ////////////////////////

const size_t BITSPERBYTE = 8;        ///< number of bits in a byte
const size_t BITSPERWORD = 31;        ///< number of bits in a word
const size_t BYTESPERWORD = 4;       ///< number of bytes in a word
const size_t RADIX = 4294967295;    ///< number of possible key values

typedef
struct Node_s {
    //TODO define Node_s data members
    struct Node_s *left_child;
    Entry value;
    struct Node_s *right_child;
} * Node;


typedef Node * NodeH;  

struct Trie_s {
    size_t height;
    size_t num_leaf_nodes;
    size_t num_nodes_total;
    NodeH head;
};

////////////////////// Functions of Nodes ////////////////////////////////

/// Creates a dynamically allocated node
/// @param e The entry which will be the new nodes value.

Node create_node(Entry e) {

    Node tmp = (Node) malloc(sizeof(struct Node_s));

    tmp->value = e;
    tmp->left_child = NULL;
    tmp->right_child = NULL;

    return tmp;
}

/// Destroys and frees all nodes + entries within the trie
/// uses the head as a point of access, traverses in order from there.
///
/// @param n the head node of the trie to be destroyed 

void destroy_nodes( Node n) {
    if(n == NULL) {
        return;
    }
    destroy_nodes(n->left_child);
    destroy_nodes(n->right_child);

    entry_destroy(n->value);
    free(n);
}

///
/// Traverses the trie with two entries, obersing their bits to 
/// see where to insert them.
///
/// @param head a pointer to a pointer to an instance of Node_s
/// @param e1 the first entry to be inserted
/// @param e2 the second entry to be inserted 
/// @param index which bit will be observed in both entries.
///

void node_insert_w2(NodeH head, Entry e1, Entry e2, int index) {
    
    if(IS_BIT_SET(e1->key, index) != IS_BIT_SET(e2->key, index)) {
        if(IS_BIT_SET(e1->key, index)) {
            (*head)->right_child = create_node(e1);
            (*head)->left_child = create_node(e2);
        }
        else {
            (*head)->right_child = create_node(e2);
            (*head)->left_child = create_node(e1);
        }
        return;
    }
    else {
        if(IS_BIT_SET(e1->key, index)) {
            (*head)->right_child = create_node(NULL);
            Node right = (*head)->right_child;
            node_insert_w2(&right, e1, e2, index - 1);
        }
        else {
            (*head)->left_child = create_node(NULL);
            Node left = (*head)->left_child;
            node_insert_w2(&left, e1, e2, index - 1);
        }
        return;
    }
}


/// Inserts a node into the trie tree recursively.
///
/// Three main cases:
/// - node is a leaf node without value, insert the entry into node.
/// 
/// - node is a body node, traverse the tree dependent of index -1
///   bit of entry->key.
///
/// - node collides with another node, read documentation on
///   node_insert_w_2.
///
/// @param node a pointer to a pointer to an instance of Node_s
/// @param e the entry to be inserted amongst the Trie
/// @param index which bit to observe in e->key
/// 
/// @return either a Node wil NULL value, or a node with Entry e
/// as its value.

Node node_insert(NodeH node, Entry e, int index) {


    //comes to an empty leaf node
    if(*node == NULL) {
        return create_node(e);
    }
    // comes to a body node, must traverse the tree
    if((*node)->value == NULL) {
        if(IS_BIT_SET(e->key, index)) {
            //problem lyes here
            Node right = (*node)->right_child;
            (*node)->right_child = node_insert(&right, e, index - 1);
        }
        else {
            //problem
            Node left = (*node)->left_child;
            (*node)->left_child = node_insert(&left, e, index - 1);
        }
        return *node;
    }
    // runs into collision with previous key, must traverse with both
    else {
        Entry cpy = copy_entry((*node)->value);
        entry_destroy((*node)->value);
        (*node)->value = NULL;

        // not moving to child node so index stays the same
        node_insert_w2(node, e, cpy, index);
        return *node;
    }
}

///
/// Gets the height of the trie, should not return
/// numbers larger than 31.
///
/// @param node node which is being traversed inside the trie
///

size_t get_height(Node node) {
    if(node != NULL) {
        int left_height = get_height(node->left_child);
        int right_height = get_height(node->right_child);
        
        return MAX(left_height, right_height) + 1;
    }
    else {
        return 0;
    }
}

/// Recursive function that only counts the BODY nodes.
/// i.e; Nodes that do note have a value
///
/// @param node node that is being checked if it has a value or not.

size_t get_internal_nodes(Node node) {
    size_t total = 0;
    if(node != NULL) {
        if(node->value == NULL) {
            total++;
        }
        total += get_internal_nodes(node->left_child);
        total += get_internal_nodes(node->right_child);
    }
    return total;
}

/// Recursive function that gets the total number of
/// leaf nodes inside the Trie.
///
/// @param node the node that is being observed if it has a value

size_t get_leaf_nodes(Node node) {
    int leaf_nodes = 0;
    if(node != NULL) {
        if(node->value != NULL) {
            leaf_nodes++;
        }
        leaf_nodes += get_leaf_nodes(node->left_child);
        leaf_nodes += get_leaf_nodes(node->right_child);
    }
    return leaf_nodes;
}


/// Works as a recursively called function, travering
/// the nodes of the trie in-order and printing them off as such. 
///
/// @param node the node which is being observed and will have
/// its children called inside the function.
/// @param stream the file the contents of the Trie are being 
/// printed to.
///

void show_nodes( Node node, FILE * stream) {

    if(node != NULL) {
        show_nodes(node->left_child, stream);
        if(node->value != NULL) {
            entry_print(node->value, stream);
        }
        show_nodes(node->right_child, stream);
    }
}

Entry node_search( Node node, ikey_t key, int index) {
    // at a leaf node
    if(node != NULL) {
        // we are at a leaf node, return the entry
        if(node->value != NULL) {
            return node->value;
        }
        // we are in a body node, traverse the tree accordingly
        else {
            Entry tmp;
            if(IS_BIT_SET(key, index)) {
                tmp = node_search(node->right_child, key, index -1);
                if(tmp != NULL) {
                    return tmp;
                }
                tmp = node_search(node->left_child, key, index -1);
                if(tmp != NULL) {
                    return tmp;
                }
                printf("We should not have reached here...\n");
            }
            else {
                tmp = node_search(node->left_child, key, index - 1);
                if(tmp != NULL) {
                    return tmp;
                }
                tmp = node_search(node->right_child, key, index - 1);
                if(tmp != NULL) {
                    return tmp;
                }
                printf("We should not have made it this far\n"); 
            }
        }
    }
    return NULL;
}


/////////////////////// Functions of tries ///////////////////////////////

/// Create a Trie instance and initialize its fields.
/// @return pointer to the Trie object instance or NULL on failure
/// @post Trie is NULL on failure or initialized with a NULL trie root

Trie ibt_create( void ) {
    Trie tmp = (Trie) malloc( sizeof(struct Trie_s) );

    tmp->height = -1;
    tmp->num_leaf_nodes = 0;
    tmp->num_nodes_total = 0;
    tmp->head = (NodeH) malloc(sizeof(struct Node_s));
    *(tmp->head) = NULL;
    return tmp;
}

/// Destroy the trie and free all storage.
/// Uses Trie's Delete_value function to free app-specific (key and) value;
/// If the Trie's Delete_value function is NULL,
/// then call free() on the value in (key,value).
/// @param trie a pointer to a Trie instance
/// @pre trie is a valid Trie instance pointer
/// @post the storage associated with the Trie and all data has been freed

void ibt_destroy( Trie trie) {
    destroy_nodes(*(trie->head));
    free(trie);
}


/// insert an entry into the Trie as long as the entry is not already present
/// @param trie a pointer to a Trie instance
/// @param e the entry to be inserted into the trie
/// @post the trie has grown to include a new entry IFF not already present

void ibt_insert( Trie trie, Entry e) {
    *(trie->head) = node_insert(trie->head, e, BITSPERWORD);
}

/// get height of the trie
/// @param trie a pointer to a Trie instance
/// @return height of trie

size_t ibt_height( Trie trie ) {
    trie->height = get_height(*(trie->head));
    return trie->height;
}

/// get the node count of the trie: the number of internal nodes
/// @param trie a pointer to a Trie instance
/// @return the count of internal nodes

size_t ibt_node_count( Trie trie ) {
    return get_internal_nodes(*(trie->head));
}

/// get the size of the trie or number of leaf elements
/// @param trie a pointer to a Trie instance
/// @return size of trie 

size_t ibt_size( Trie trie) {
    trie->num_leaf_nodes = get_leaf_nodes(*(trie->head));

    return trie->num_leaf_nodes;
}

/// Perform an in-order traversal to show each (key, value) in the trie.
/// Uses Trie's Show_value function to show each leaf node's data,
/// and if the function is NULL, output each key and value in hexadecimal.
///
/// @param trie a pointer to a Trie instance
/// @param stream the stream destination of output

void ibt_show( Trie trie, FILE * stream) {
    show_nodes(*(trie->head), stream);
    return;
}

/// Runs functions mentioned above to update the datamembers
/// inside the trie.
///
/// @param trie a pointer to a Trie instance
/// @post all data members like total_nodes, height, etc. have
/// been updated to current values.

void ibt_update( Trie trie ) {
    trie->height = ibt_height(trie);
    trie->num_leaf_nodes = ibt_size(trie);
    trie->num_nodes_total = ibt_node_count(trie);
    printf("height:   %ld\n", trie->height);
    printf("size:   %ld\n", trie->num_leaf_nodes);
    printf("node_count:   %ld\n", trie->num_nodes_total); 
}

/// search for the key in the trie by finding
/// the closest entry that matches key in the Trie.
/// @param trie a pointer to a Trie instance
/// @param key the key to find 
/// @return entry representing the found entry or a null entry for not found

Entry ibt_search( Trie trie, ikey_t key) {
    int index = BITSPERWORD;
    Entry e = node_search(*trie->head, key, index);
    return e;
}
