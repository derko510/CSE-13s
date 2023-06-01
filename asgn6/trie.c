#include <stddef.h>
#include <stdlib.h>

#include "trie.h"
#include "code.h"

// create a new TrieNode with the given code
TrieNode *trie_node_create(uint16_t code) {
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    if (node == NULL) {
        return NULL;
    }
    // initialize children pointers to NULL
    for (int i = 0; i < ALPHABET; i++) {
        node->children[i] = NULL;
    }
    node->code = code; // set the node's code
    return node;
}

// delete a TrieNode
void trie_node_delete(TrieNode *n) {
    free(n);
}

// create a new Trie with an empty root node
TrieNode *trie_create(void) {
    TrieNode *root = trie_node_create(EMPTY_CODE);
    return root;
}

void trie_reset(TrieNode *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i] != NULL) { // if the child is not NULL, reset it and delete it
            trie_node_delete(root->children[i]); // delete the current child
            root->children[i] = NULL; // set the current child to NULL
        }
    }
}

// delete a Trie by recursively deleting all of its nodes
void trie_delete(TrieNode *n) {
    if (n == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET; i++) {
        trie_delete(n->children[i]);
    }
    trie_node_delete(n);
}

// navigate a Trie by returning the child node corresponding to the given symbol
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
