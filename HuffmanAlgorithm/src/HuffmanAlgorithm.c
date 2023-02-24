/**** INCLUDES ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static HeapNodePtr createNewNode(char item, unsigned freq);
static HeapPtr initHeap(unsigned capacity);
static void swapNodes(HeapNodePtr *node_a, HeapNodePtr *node_b);

/**** LOCAL FUNCTION DEFINITIONS **********************************************/

static HeapNodePtr createNewNode(char item, unsigned freq)
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

static HeapPtr initHeap(unsigned capacity)
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

static void swapNodes(HeapNodePtr *node_a, HeapNodePtr *node_b)
{
    HeapNodePtr temp = *node_a;

    *node_a = *node_b;
    *node_b = temp;
}

static void heapifyMin(HeapPtr heap, int index)
{
    int smallest = index;
    int left = 2*index + 1;
    int right = 2*index + 2;

    if (left < heap->size && heap->array[left]->freq < heap->array[smallest]->freq) {
        smallest = left;
    }

    if (right < heap->size && heap->array[right] -> freq < heap->array[smallest]->freq) {
        smallest = right;
    }

    if (index != smallest) {
        swapNodes(&heap->array[smallest], &heap->array[index]);
        heapifyMin(heap, smallest);
    }
}

static int checkIfSizeOne(HeapPtr heap) {
  return (heap->size == 1);
}

static HeapNodePtr extractMin(HeapPtr heap)
{
    HeapNodePtr min = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    heapifyMin(heap, 0);

    return min;
}

static void insertNodeHeap(HeapPtr heap, HeapNodePtr node)
{
    if(heap->capacity == heap->size) {
        fprintf(stderr, "Heap overflow");
        exit(EXIT_FAILURE);
    }

    heap->size++;
    int i = heap->size - 1;

    while (i != 0 && node->freq < heap->array[(i - 1)/2]->freq) {
        heap->array[i] = heap->array[(i - 1)/2];
        i = (i - 1)/2;
    }

    heap->array[i] = node;
}

static void buildHeap(HeapPtr heap)
{
    int n = heap->size - 1;

    for (int i = (n - 1)/2; i >= 0; i--) {
        heapifyMin(heap, i);
    }
}

static int isLeaf(HeapNodePtr root) {
  return !(root->left) && !(root->right);
}

static HeapPtr createAndBuildHeap(const tableOfFrequencies_t *freqTable)
{
    unsigned int size = freqTable->numOfUniqueChars;
    HeapPtr heap = initHeap(size);

    unsigned int j = 0;
    for (unsigned int i = 0; i < MAX_CHARS; i++) {
        if (freqTable->freq[i] != 0) {
            heap->array[j] = createNewNode((char)i, freqTable->freq[i]);
            j++;
        }
    }

    heap->size = size;
    buildHeap(heap);

    return heap;
}

static HeapNodePtr buildHuffmanTree(const tableOfFrequencies_t *freqTable)
{
    HeapNodePtr left, right, top;
    HeapPtr heap = createAndBuildHeap(freqTable);

    while (!checkIfSizeOne(heap)) {
        left = extractMin(heap);
        right = extractMin(heap);

        top = createNewNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertNodeHeap(heap, top);
    }

    return extractMin(heap);
}

static void encodeItemsOfTree(HeapNodePtr root, Code_t *codesTable, unsigned int depth)
{
    if (root->left) {
        buffer[depth] = '0';
        buffer[depth + 1] = '\0';
        encodeItemsOfTree(root->left, codesTable, depth + 1);
    }

    if (root->right) {
        buffer[depth] = '1';
        buffer[depth + 1] = '\0';
        encodeItemsOfTree(root->right, codesTable, depth + 1);
    }

    if (isLeaf(root)) {
        codesTable[root->item].code = (char *)malloc((depth + 1)*sizeof(char));
        if (!isMemoryAllocated(codesTable[root->item].code)) {
            exit(EXIT_FAILURE);
        }
        strcpy(codesTable[root->item].code, buffer);
    }
}

/**** GLOBAL FUNCTION DEFINITIONS *********************************************/

char *getCodeForChar(unsigned char ch)
{
    return codesTable[ch].code;
}

void compressData(const tableOfFrequencies_t *freqTable)
{
    HeapNodePtr root = buildHuffmanTree(freqTable);    
    encodeItemsOfTree(root, codesTable, 0);
}
