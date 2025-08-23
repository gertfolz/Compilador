#include "hash.h"

hash_node* hash_table[TABLE_SIZE];

void init_table(void) {
    for (int i=0;i<TABLE_SIZE;i++) {
        hash_table[i] = NULL;
    }
}

int hash(char *entry) {
    int hash_code = 0;

    for (char ch=*entry;ch!='\0';ch=*entry++) {
        hash_code = ((int)ch + hash_code*37) % TABLE_SIZE;
    }

    return hash_code;
}

hash_node* insert_node(char *entry, int type) {
    hash_node* new_node;
    int code = hash(entry);

    new_node = get_node(entry);
    if (new_node != NULL) {
        return new_node;
    }
    
    new_node = (hash_node*)malloc(sizeof(hash_node));
    new_node->type = type;
    new_node->value = (char*)calloc(strlen(entry) + 1, sizeof(char));
    strcpy(new_node->value, entry);

    new_node->next_node = hash_table[code];
    hash_table[code] = new_node;
    return new_node;
}

hash_node* get_node(char *entry) {
    hash_node* node;
    int calculated_code = hash(entry);

    for (node=hash_table[calculated_code]; node!=NULL; node=node->next_node) {
        int found = strcmp(node->value, entry);
        if (found == 0) {
            return node;
        }        
    }

    return NULL;
}

hash_node* delete_table(void) {
    return NULL;
}

void print_table(void) {
    for (int i=0; i<TABLE_SIZE; i++) {
        for (hash_node* node=hash_table[i]; node != NULL; node=node->next_node) {
            printf("hash_table: %s value with %d type at %d index\n", node->value, node->type, i);
        }        
    }
} 