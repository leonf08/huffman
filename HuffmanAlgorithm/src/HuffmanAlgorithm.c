/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "HuffmanAlgorithm.h"

/**** TYPES DEFINITIONS *******************************************************/

struct HeapNode {
    unsigned char item;
    unsigned int freq;
    struct HeapNode *left, *right;
};

typedef struct HeapNode *HeapNodePtr;

struct Heap {
    unsigned int size;
    unsigned int capacity;
    HeapNodePtr *array;
};

typedef struct Heap *HeapPtr;
typedef struct {char *code;} Code_t;

/**** STATIC VARIABLES DEFINITIONS ********************************************/

static char buffer[MAX_CHARS];
static Code_t codesTable[MAX_CHARS];

/**** LOCAL FUNCTION DECLARATIONS *********************************************/

static HeapNodePtr createNewNode(unsigned char item, unsigned int freq);
static HeapPtr initHeap(unsigned int capacity);
static void swapNodes(HeapNodePtr *node_a, HeapNodePtr *node_b);
static void heapifyMin(HeapPtr heap, int index);
static bool checkIfSizeOne(HeapPtr heap);
static HeapNodePtr extractMin(HeapPtr heap);
static void insertNodeHeap(HeapPtr heap, HeapNodePtr node);
static void buildHeap(HeapPtr heap);
static bool isLeaf(HeapNodePtr root);
static HeapPtr createAndBuildHeap(const tableOfFrequencies_t *freqTable);
static HeapNodePtr buildHuffmanTree(const tableOfFrequencies_t *freqTable);
static void encodeItemsOfTree(HeapNodePtr root, Code_t *codesTable, unsigned int depth);
static void traverseTree(const tableOfFrequencies_t *freqTable, FILE *inputFilePtr, FILE *outputFilePtr, HeapNodePtr root);
static void freeMemoryOfHuffmanTree(HeapNodePtr *root);

/**** LOCAL FUNCTION DEFINITIONS **********************************************/

/*
Creates new node in binary heap

@param item: Character to be assigned to node
@param freq: Frequency of a character in input text file, to be assigned to node

@return pointer to node
*/

static HeapNodePtr createNewNode(unsigned char item, unsigned int freq)
{
    HeapNodePtr temp = (HeapNodePtr)malloc(sizeof(struct HeapNode));
    if (!isMemoryAllocated(temp)) {
        exit(EXIT_FAILURE);
    }

    temp->item = item;
    temp->freq = freq;
    temp->left = temp->right = NULL;

    return temp;
}

/*
Create and intialize heap with given capacity

@param capacity: Maximum size of heap to be created

@return Pointer to heap data structure
*/

static HeapPtr initHeap(unsigned int capacity)
{
    HeapPtr temp = (HeapPtr)malloc(sizeof(struct Heap));
    if (!isMemoryAllocated(temp)) {
        exit(EXIT_FAILURE);
    }

    temp->capacity = capacity;
    temp->size = 0;
    temp->array = (HeapNodePtr *)malloc(capacity*sizeof(HeapNodePtr));
    if (!isMemoryAllocated(temp->array)) {
        exit(EXIT_FAILURE);
    }

    return temp;
}

/*
Swaps nodes of the heap

@param node_a: Pointer to node a
@param node_b: Pointer to node b

@return no value
*/

static void swapNodes(HeapNodePtr *node_a, HeapNodePtr *node_b)
{
    HeapNodePtr temp = *node_a;

    *node_a = *node_b;
    *node_b = temp;
}

/*
Sorts the nodes in heap according to rules for min-heap data structure

@param heap: Pointer to heap
@param index: Number of the smallest node in heap

@return no value
*/

static void heapifyMin(HeapPtr heap, int index)
{
    int smallest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;

    /* If frequency of a left child node is smaller than the one of 
    a parent node, assign number of the child node to smallest */
    if (left < heap->size && heap->array[left]->freq < heap->array[smallest]->freq) {
        smallest = left;
    }
    /* If frequency of a right child node is smaller than the one of 
    a parent node, assign number of the child node to smallest */
    if (right < heap->size && heap->array[right] -> freq < heap->array[smallest]->freq) {
        smallest = right;
    }

    /* If smallest was changed, swap the child and parent nodes.
    Then sort the heap recursively*/
    if (index != smallest) {
        swapNodes(&heap->array[smallest], &heap->array[index]);
        heapifyMin(heap, smallest);
    }
}

/*
Checks if size of the heap equals to 1

@param heap: Pointer to heap

@return true if size of the heap is 1, otherwise false
*/

static bool checkIfSizeOne(HeapPtr heap)
{
  return (heap->size == 1);
}

/*
Pops out node with minimal value of frequency from sorted heap

@param heap: Pointer to heap

@return pointer to node
*/

static HeapNodePtr extractMin(HeapPtr heap)
{
    HeapNodePtr min = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    heapifyMin(heap, 0);

    return min;
}

/*
Inserts node in heap

@param heap: Pointer to heap
@param node: Pointer to node

@return no value
*/

static void insertNodeHeap(HeapPtr heap, HeapNodePtr node)
{
    if(heap->capacity == heap->size) {
        fprintf(stderr, "Heap overflow");
        exit(EXIT_FAILURE);
    }

    heap->size++;
    int i = heap->size - 1;

    /* If frequency of the given node is smaller than its supposed parent node, 
    continue looking for an appropriate place */
    while (i != 0 && node->freq < heap->array[(i - 1)/2]->freq) {
        heap->array[i] = heap->array[(i - 1)/2];
        i = (i - 1)/2;
    }

    /* Once the right place is found, put the node into array */
    heap->array[i] = node;
}

/*
Builds min-heap by calling sorting function heapifyMin

@param heap: Pointer to heap

@return no value
*/

static void buildHeap(HeapPtr heap)
{
    int n = heap->size - 1;

    /* Start sorting with the first parent node closest to leaf */
    for (int i = (n - 1)/2; i >= 0; i--) {
        heapifyMin(heap, i);
    }
}

/*
Checks if a node is a leaf

@param root: Pointer to node

@return true if the node is a leaf, otherwise false
*/

static bool isLeaf(HeapNodePtr root)
{
  return !(root->left) && !(root->right);
}

/*
Creates min-heap

@param freqTable: Pointer to the table of frequencies for each character 
to be encoded

@return pointer to the created heap
*/

static HeapPtr createAndBuildHeap(const tableOfFrequencies_t *freqTable)
{
    /* Create heap with defined capacity */
    unsigned int size = freqTable->numOfUniqueChars;
    HeapPtr heap = initHeap(size);

    /* Create array of nodes of the heap */
    unsigned int j = 0;
    for (unsigned int i = 0; i < MAX_CHARS; i++) {
        if (freqTable->freq[i] != 0) {
            heap->array[j] = createNewNode((unsigned char)i, freqTable->freq[i]);
            j++;
        }
    }

    /* Sort heap to make it min-heap */
    heap->size = size;
    buildHeap(heap);

    return heap;
}

/* 
Bilds Huffman tree based on min-heap where each node is a character 
to be encoded

@param freqTable: Pointer to the table of frequencies for each character 
to be encoded

@return pointer to root of the Huffman tree
*/

static HeapNodePtr buildHuffmanTree(const tableOfFrequencies_t *freqTable)
{
    /* Create and build min-heap */
    HeapNodePtr left, right, top, root;
    HeapPtr heap = createAndBuildHeap(freqTable);

    /* Build Huffman tree */
    while (!checkIfSizeOne(heap)) {
        left = extractMin(heap);
        right = extractMin(heap);

        top = createNewNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertNodeHeap(heap, top);
    }

    root = extractMin(heap);

    /* Deallocate memory used by heap */
    free(heap->array);
    heap->array = NULL;
    free(heap);
    heap = NULL;

    /* Return pointer to the root of the tree */
    return root;
}

/*
Traverses through the Huffman tree and encodes characters

@param root: Pointer to the root of the tree
@param codesTable: Pointer to the data structure with array of codes for 
each character
@param depth: Number of the node in the tree

@return no value
*/

static void encodeItemsOfTree(HeapNodePtr root, Code_t *codesTable, unsigned int depth)
{
    /* Go to the left node and add 0 to the code */
    if (root->left) {
        buffer[depth] = '0';
        buffer[depth + 1] = '\0';
        encodeItemsOfTree(root->left, codesTable, depth + 1);
    }

    /* Go to the right node and add 1 to the code */
    if (root->right) {
        buffer[depth] = '1';
        buffer[depth + 1] = '\0';
        encodeItemsOfTree(root->right, codesTable, depth + 1);
    }

    /* Once leaf (i.e character) is reached copy created code to 
    the array of codes*/
    if (isLeaf(root)) {
        codesTable[root->item].code = (char *)malloc((depth + 1)*sizeof(char));
        if (!isMemoryAllocated(codesTable[root->item].code)) {
            exit(EXIT_FAILURE);
        }
        strcpy(codesTable[root->item].code, buffer);
    }
}

/*
Reads encoded data from archive file, traverses through the tree 
to decode data and write decoded data into output text file

@param freqTable: Pointer to the table of frequencies for each character 
to be encoded
@param inputFilePtr: Pointer to FILE object associated with input file with
compressed data
@param outputFilePtr: Pointer to FILE object associated with text file 
with decoded data
@param root: Pointer to the root of the tree

@return no value
*/

static void traverseTree(const tableOfFrequencies_t *freqTable, FILE *inputFilePtr, FILE *outputFilePtr, HeapNodePtr root)
{
    HeapNodePtr currentNode = root;
    unsigned char byte, bit;
    long long chNum = freqTable->numOfAllChars;

    /* Read byte by byte from archive file until end-of-file */
    while (!feof(inputFilePtr)) {
        if (1 != fread(&byte, sizeof(unsigned char), 1, inputFilePtr)) {
            if (feof(inputFilePtr))
                break;

            if (ferror(inputFilePtr)) {
            fprintf(stderr, "Reading error\n");
                exit(EXIT_FAILURE);
            }
        }

        /* Analyze each bit in the byte: if 0, go to the left node, 
        if 1 - to the right node */
        for (int i = 7; i >= 0; i--) {
            bit = (byte >> i) & 0x01;
            if (bit == 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }

            /* Once leaf is reached, write corresponding character into 
            output text file */
            if (isLeaf(currentNode)) {
                fwrite(&currentNode->item, sizeof(unsigned char), 1, outputFilePtr);
                currentNode = root;
                chNum--;
                if (chNum == 0)
                    break;
            }
        }
    }
}

/*
Deallocates memory used by Huffman tree

@param root: Pointer to pointer to the root of the tree

@return no value
*/

static void freeMemoryOfHuffmanTree(HeapNodePtr *root)
{
    HeapNodePtr *current = root;
    if ((*current)->left) {
        freeMemoryOfHuffmanTree(&(*current)->left);
    }

    if ((*current)->right) {
        freeMemoryOfHuffmanTree(&(*current)->right);
    }

    if (isLeaf(*current)) {
        free(*current);
        *current = NULL;
    }
}

/**** GLOBAL FUNCTION DEFINITIONS *********************************************/

/*
Returns huffman code for a character

@param ch: character whose code is to be returned

@return code represented as string
*/

char *getCodeForChar(unsigned char ch)
{
    return codesTable[ch].code;
}

/*
Interface to invoke huffman algorithm for data compressing

@param freqTable: Pointer to the table of frequencies for each character 
to be encoded

@return no value
*/

void compressData(const tableOfFrequencies_t *freqTable)
{
    HeapNodePtr root = buildHuffmanTree(freqTable);    
    encodeItemsOfTree(root, codesTable, 0);

    freeMemoryOfHuffmanTree(&root);
}

/*
Interface to invoke huffman algorithm for data decompressing

@param freqTable: Pointer to the table of frequencies for each character 
to be encoded
@param inputFilePtr: Pointer to FILE object associated with input file with
compressed data
@param outputFilePtr: Pointer to FILE object associated with text file 
with decoded data

@return no value
*/

void decompressData(const tableOfFrequencies_t *freqTable, FILE *inputFilePtr, FILE *outputFilePtr)
{
    HeapNodePtr root = buildHuffmanTree(freqTable);
    traverseTree(freqTable, inputFilePtr, outputFilePtr, root);

    freeMemoryOfHuffmanTree(&root);
}

/*
Deallocate memory used by codesTable data structure

@param no params

@return no value
*/

void freeCodesTable(void)
{
    for (int i = 0; i < MAX_CHARS; i++)
    {
        if (NULL != codesTable[i].code) {
            free(codesTable[i].code);
            codesTable[i].code = NULL;
        }
    }
}