#include <stdio.h>
#include <stdlib.h>

/**** TYPES *******************************************************************/
struct HeapNode {
    char item;
    unsigned freq;
    struct HeapNode *left, *right;
};

typedef struct HeapNode *HeapNodePtr;

struct Heap {
    unsigned size;
    unsigned capacity;
    HeapNodePtr *array;
};

typedef struct Heap *HeapPtr;

/**** LOCAL FUNCTION DECLARATIONS *********************************************/
static HeapNodePtr createNewNode(char item, unsigned freq);
static HeapPtr createHeap(unsigned capacity);
static void swapNodes(HeapNodePtr *node_a, HeapNodePtr *node_b);

/**** LOCAL FUNCTION DEFINITIONS **********************************************/
static HeapNodePtr createNewNode(char item, unsigned freq)
{
    HeapNodePtr temp = (HeapNodePtr)malloc(sizeof(struct HeapNode));
    if (temp == NULL)
    {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }

    temp->item = item;
    temp->freq = freq;
    temp->left = temp->right = NULL;

    return temp;
}

static HeapPtr createHeap(unsigned capacity)
{
    HeapPtr temp = (HeapPtr)malloc(sizeof(struct Heap));
    if (temp == NULL)
    {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }

    temp->capacity = capacity;
    temp->size = 0;
    temp->array = (HeapNodePtr *)malloc(capacity*sizeof(HeapNodePtr));

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