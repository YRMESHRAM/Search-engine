#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
    int wordPosition; // To store the position of the word in the list
};

// Function prototypes
struct TrieNode *createNode();
void insert(struct TrieNode *root, const char *word, int position);
bool search(struct TrieNode *root, const char *word, int *position);
struct TrieNode *deleteWord(struct TrieNode *root, const char *word, int depth);
void autoComplete(struct TrieNode *root, const char *prefix);
void displayAllWords(struct TrieNode *root, char prefix[], int length);
void interactiveSearch(struct TrieNode *root);

struct TrieNode *createNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    node->isEndOfWord = false;
    node->wordPosition = -1; // Initialize with -1 indicating no position assigned yet
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insert(struct TrieNode *root, const char *word, int position) {
    struct TrieNode *currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL) {
            currentNode->children[index] = createNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->isEndOfWord = true;
    currentNode->wordPosition = position; // Store the position of the word
}

bool search(struct TrieNode *root, const char *word, int *position) {
    struct TrieNode *currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL) {
            return false;
        }
        currentNode = currentNode->children[index];
    }
    if (currentNode != NULL && currentNode->isEndOfWord) {
        *position = currentNode->wordPosition; // Retrieve the word position
        return true;
    }
    return false;
}

bool isEmpty(struct TrieNode *root) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            return false;
        }
    }
    return true;
}

struct TrieNode *deleteWord(struct TrieNode *root, const char *word, int depth) {
    if (!root) {
        return NULL;
    }

    if (word[depth] == '\0') {
        if (root->isEndOfWord) {
            root->isEndOfWord = false;
        }
        if (isEmpty(root)) {
            free(root);
            root = NULL;
        }
        return root;
    }

    int index = word[depth] - 'a';
    root->children[index] = deleteWord(root->children[index], word, depth + 1);

    if (isEmpty(root) && root->isEndOfWord == false) {
        free(root);
        root = NULL;
    }
    return root;
}

void displaySuggestions(struct TrieNode *root, char prefix[], int length) {
    if (root->isEndOfWord) {
        prefix[length] = '\0';
        printf("%s\n", prefix);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            prefix[length] = i + 'a';
            displaySuggestions(root->children[i], prefix, length + 1);
        }
    }
}

void autoComplete(struct TrieNode *root, const char *prefix) {
    struct TrieNode *currentNode = root;
    int length = strlen(prefix);
    char buffer[100];
    strcpy(buffer, prefix);

    for (int i = 0; i < length; i++) {
        int index = prefix[i] - 'a';
        if (currentNode->children[index] == NULL) {
            printf("No suggestions found for prefix '%s'\n", prefix);
            return;
        }
        currentNode = currentNode->children[index];
    }
    displaySuggestions(currentNode, buffer, length);
}

void displayAllWords(struct TrieNode *root, char prefix[], int length) {
    if (root->isEndOfWord) {
        prefix[length] = '\0';
        printf("%s\n", prefix);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            prefix[length] = i + 'a';
            displayAllWords(root->children[i], prefix, length + 1);
        }
    }
}

void interactiveSearch(struct TrieNode *root) {
    char input[50];
    int choice;
    int position;
    
    do {
        printf("\n--- Search Engine Menu ---\n");
        printf("1. Search a word\n");
        printf("2. Insert a word\n");
        printf("3. Delete a word\n");
        printf("4. Auto-complete suggestions\n");
        printf("5. Display all words\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline character after the number input
        
        switch (choice) {
            case 1:
                printf("Enter a word to search: ");
                scanf("%49s", input);
                for (int i = 0; input[i]; i++) {
                    input[i] = tolower(input[i]);
                }
                if (search(root, input, &position)) {
                    printf("The word '%s' is found in the search engine at position %d.\n", input, position);
                } else {
                    printf("The word '%s' is not found in the search engine.\n", input);
                }
                break;

            case 2:
                printf("Enter a word to insert: ");
                scanf("%49s", input);
                for (int i = 0; input[i]; i++) {
                    input[i] = tolower(input[i]);
                }
                printf("Enter the position of the word: ");
                scanf("%d", &position);
                insert(root, input, position);
                printf("Word '%s' inserted successfully at position %d.\n", input, position);
                break;

            case 3:
                printf("Enter a word to delete: ");
                scanf("%49s", input);
                for (int i = 0; input[i]; i++) {
                    input[i] = tolower(input[i]);
                }
                deleteWord(root, input, 0);
                printf("Word '%s' deleted successfully.\n", input);
                break;

            case 4:
                printf("Enter a prefix for auto-complete suggestions: ");
                scanf("%49s", input);
                for (int i = 0; input[i]; i++) {
                    input[i] = tolower(input[i]);
                }
                printf("Auto-complete suggestions for prefix '%s':\n", input);
                autoComplete(root, input);
                break;

            case 5:
                printf("List of all words in the search engine:\n");
                char buffer[100];
                displayAllWords(root, buffer, 0);
                break;

            case 6:
                printf("Exiting the search engine. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

int main() {
    struct TrieNode *root = createNode();
    const char *words[] = {"apple", "banana", "cat", "dog", "elephant", "fish", "goat", "hat", "ice", "jelly", 
                           "kite", "lion", "monkey", "night", "owl", "panda", "queen", "rabbit", "snake", 
                           "tiger", "umbrella", "vase", "wolf", "xerox", "yak", "zebra"};
    int numWords = sizeof(words) / sizeof(words[0]);
    
    for (int i = 0; i < numWords; i++) {
        insert(root, words[i], i + 1); // Insert each word with its position (1-based index)
    }

    interactiveSearch(root);

    return 0;
}
