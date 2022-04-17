#include<stdio.h>
#include<stdlib.h>

// Two global counters
int g = 0, k = 0;

// Structure for free list
struct free {
    int tag;
    int size;
    struct free* next;
}* free_head = NULL, *prev_free = NULL;

// Structure for allocated list
struct alloc {
    int block_id;
    int tag;
    int size;
    struct alloc* next;
}* alloc_head = NULL, *prev_alloc = NULL;

void create_free(int c)
{
    struct free* p = (struct free*)
        malloc(sizeof(struct free));
    p->size = c;
    p->tag = g;
    p->next = NULL;
    if (free_head == NULL)
        free_head = p;
    else
        prev_free->next = p;
    prev_free = p;
    g++;
}

void print_free()
{
    struct free* p = free_head;
    printf("Tag\tSize\n");
    while (p != NULL) {
        printf("%d \t %d\n",p->tag,p->size);
        p = p->next;
    }
}

void create_alloc(int c)
{
    // create node for process of given size
    struct alloc* q = (struct alloc*)malloc(sizeof(struct alloc));
    q->size = c;
    q->tag = k;
    q->next = NULL;
    struct free* p = free_head;

    // Iterate to find first memory
    // block with appropriate size
    while (p != NULL) {
        if (q->size <= p->size)
            break;
        p = p->next;
    }

    // Node found to allocate
    if (p != NULL) {
        // Adding node to allocated list
        q->block_id = p->tag;
        p->size -= q->size;
        if (alloc_head == NULL)
            alloc_head = q;
        else {
            prev_alloc = alloc_head;
            while (prev_alloc->next != NULL)
                prev_alloc = prev_alloc->next;
            prev_alloc->next = q;
        }
        k++;
    }
    else // Node found to allocate space from
        printf("Block of size %d can't be allocated\n",c);
}

void print_alloc()
{
    struct alloc* p = alloc_head;
    printf("Tag\tBlock ID\tSize\n");
    while (p != NULL) {
        printf( "%d\t %d\t\t %d\n",p->tag,p->block_id,p->size);
        p = p->next;
    }
}

void free_alloc(int t)
{
    // Standard delete function
    // of a linked list node
    struct alloc *p = alloc_head, *q = NULL;

    // First, find the node according
    // to given tag id
    while (p != NULL) {
        if (p->tag == t)
            break;
        q = p;
        p = p->next;
    }
    if (p == NULL)
        printf("Tag ID doesn't exist \n");
    else if (p == alloc_head)
        alloc_head = alloc_head->next;
    else
        q->next = p->next;
    struct free* temp = free_head;
    while (temp != NULL) {
        if (temp->tag == p->block_id) {
            temp->size += p->size;
            break;
        }
        temp = temp->next;
    }
}

int main()
{
    int blockSize[] = { 100, 500, 200 };
    int processSize[] = { 417, 112, 426, 95 };
    int m = sizeof(blockSize)
            / sizeof(blockSize[0]);
    int n = sizeof(processSize)
            / sizeof(processSize[0]);

    for (int i = 0; i < m; i++)
        create_free(blockSize[i]);

    for (int i = 0; i < n; i++)
        create_alloc(processSize[i]);

    print_alloc();

    // Block of tag id 0 deleted
    // to free space for block of size 426
    free_alloc(0);

    create_alloc(426);
    printf("After deleting block with Tag ID = 0\n");
    print_alloc();
}
