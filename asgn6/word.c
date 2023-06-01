#include <stdlib.h>

#include "code.h"
#include "word.h"

Word *word_create(uint8_t *syms, uint32_t len) {
    // allocate memory for the Word struct
    Word *new_word = (Word *) malloc(sizeof(Word));
    if (new_word == NULL) {
        return NULL; // error: could not allocate memory
    }

    // allocate memory for the symbols array and copy the symbols
    new_word->syms = (uint8_t *) malloc(len * sizeof(uint8_t));
    if (new_word->syms == NULL) {
        free(new_word); // clean up memory
        return NULL; // error: could not allocate memory
    }
    for (uint32_t i = 0; i < len; i++) {
        new_word->syms[i] = syms[i];
    }
    new_word->len = len; // set the length of the word
    return new_word; // return the created Word struct
}

Word *word_append_sym(Word *w, uint8_t sym) {
    // allocate memory for the new Word
    Word *new_word = (Word *) malloc(sizeof(Word));
    // allocate memory for the new array of symbols
    uint8_t *new_syms = (uint8_t *) malloc((w->len + 1) * sizeof(uint8_t));
    // copy the old array of symbols to the new array
    for (uint32_t i = 0; i < w->len; i++) {
        new_syms[i] = w->syms[i];
    }
    // append the new symbol to the new array
    new_syms[w->len] = sym;
    // update the new Word's syms and len fields
    new_word->syms = new_syms;
    new_word->len = w->len + 1;
    return new_word;
}

void word_delete(Word *w) {
    if (w) {
        free(w->syms);
        free(w);
    }
}

WordTable *wt_create(void) {
    // allocate memory for the WordTable struct
    WordTable *wt = (WordTable *) calloc(MAX_CODE + 1, sizeof(Word *));
    if (wt == NULL) {
        return NULL; // could not allocate memory
    }
    // create the first word in the table
    wt[EMPTY_CODE] = word_create(NULL, 0);
    if (wt[EMPTY_CODE] == NULL) {
        free(wt); // clean up memory
        return NULL; // error: could not allocate memory
    }
    return wt;
}

void wt_reset(WordTable *wt) {
    // delete all words in the table except the first one
    for (int i = START_CODE; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
    // reset the first word to the empty word
    if (wt[EMPTY_CODE] == NULL) {
        wt[EMPTY_CODE] = word_create(NULL, 0);
    } else {
        Word *empty_word = word_create(NULL, 0);
        if (empty_word != NULL) {
            word_delete(wt[EMPTY_CODE]);
            wt[EMPTY_CODE] = empty_word;
        }
    }
}

void wt_delete(WordTable *wt) {
    // delete all words in the table except the first one
    for (int i = 0; i <= MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
    // delete the first word in the table
    word_delete(wt[EMPTY_CODE]);
    // free the memory allocated for the table
    free(wt);
}
