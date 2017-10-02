#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node{
    char character;
    int count;
    char *word;
    struct node *parentNode;
    struct node *correctNode;
    struct node *incorrectNode;
}node;

node initialNode;

node* initNewNode(char character, int count, node* parentNode, node *correctNode, node *incorrectNode) {
    node* outputNode = (node *)malloc(sizeof(node));
    outputNode->character = character;
    outputNode->count = count;
    outputNode->parentNode = parentNode;
    outputNode->incorrectNode = incorrectNode;
    outputNode->correctNode = correctNode;
    outputNode->word = NULL;
    return outputNode;
}

void compareWord(char *word, int charIndex, node *tree) {
    if (tree == NULL) { return; }

    char symbol = word[charIndex];
    node *nextNode;
    int nextCharIndex;
    if (symbol == tree->character) {
        if (tree->correctNode == NULL) {
            tree->correctNode = initNewNode(word[charIndex + 1], 0, tree, NULL, NULL);
        }
        nextNode = tree->correctNode;
        nextCharIndex = charIndex + 1;
    } else {
        if (tree->incorrectNode == NULL) {
            tree->incorrectNode = initNewNode(symbol, 0, tree->parentNode, NULL, NULL);
        }
        nextNode = tree->incorrectNode;
        nextCharIndex = charIndex;
    }
    if (charIndex + 1 > strlen(word) || word[charIndex + 1] == '\0') {
        tree->count++;
        tree->word = (char *)malloc(strlen(word) * sizeof(char) + 1);
        strcpy(tree->word, word);
    } else {
        compareWord(word, nextCharIndex, nextNode);
    }
}

char* reverseWord(char *word) {
    for (int i = 0, l = strlen(word); i < l / 2; i++) {
        char c = word[i];
        word[i] = word[l - i - 1];
        word[l - i - 1] = c;
    }
    return word;
}

void displayWord(node *tree, char *word, int count) {
    if (tree == NULL) { return; }
    char *tempWord = (char *)malloc(sizeof(word) + sizeof(char));
    strcpy(tempWord, word);
    strcat(tempWord, (char[]){tree->character, '\0'});
    if (tree->parentNode != NULL) {
        displayWord(tree->parentNode, tempWord, count);
    } else {
        printf("%s - %d\n", reverseWord(tempWord), count);
    }
}

void display(node *tree) {
    if (tree == NULL) { return; }
    if (tree->count > 0) {
        printf("%s - %d\n", tree->word, tree->count);
//        displayWord(tree, "\0", tree->count);
    }
    display(tree->correctNode);
    display(tree->incorrectNode);
}

int main(int argc, char *argv[]) {
    const int wordsCount = argv[1];
    const char *filePath = argv[2];

    printf("%s\n", filePath);

    const char endSentenceSymbols [4] = ".?!";

    FILE *file;
    file = fopen(filePath, "r");
    if (!file) {
        puts("Error open file");
    }

    char tempChar;
    char tempWord[1000] = {0};

    int index = 0;
    while ((tempChar = fgetc(file)) != EOF) {
        if (ispunct(tempChar)) {
            if (strlen(tempWord) > 0) {
                compareWord(tempWord, 0, &initialNode);
                memset(tempWord, 0, sizeof(tempWord));
                index = 0;
            }
        } else if (isspace(tempChar)) {
            if (strlen(tempWord) > 0) {
                compareWord(tempWord, 0, &initialNode);
                memset(tempWord, 0, sizeof(tempWord));
                index = 0;
            }
        } else {
            tempWord[index] = tempChar;
            index++;
        }
    }
    display(&initialNode);
}