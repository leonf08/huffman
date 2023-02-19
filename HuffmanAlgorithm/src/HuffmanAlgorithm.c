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
static HeapPtr initHeap(unsigned capacity);
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

static HeapPtr initHeap(unsigned capacity)
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
    if (temp->array == NULL)
    {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
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
        exit(1);
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

static HeapPtr createAndBuildHeap(const char item[], const int freq[], int size)
{
    HeapPtr heap = initHeap(size);

    for (int i = 0; i < size; i++) {
        heap->array[i] = createNewNode(item[i], freq[i]);
    }

    heap->size = size;
    buildHeap(heap);

    return heap;
}

static HeapNodePtr buildHuffmanTree(const char item[], const int freq[], int size)
{
    HeapNodePtr left, right, top;
    HeapPtr heap = createAndBuildHeap(item, freq, size);

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

static void encodeItemsOfTree(HeapNodePtr root)
{
    
}