#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Node {
    Point point;
    struct Node* left;
    struct Node* right;
} Node;

Point* read_in_points(FILE* file, int* count);
int compare_fxn(const void* a, const void* b);
Node* create_node(Point point);
Node* build_bst(Point* arr, int l, int r);
int collision_detect(Node* bst, int centerx, int centery, int radius, int collisions);
void free_bst(Node* bst);

// Read in x y from input file, assign first to head_point
// Create next point by reading in the rest of the file
// contents until there are no more items from the file to be read
Point* read_in_points(FILE* file, int* count) {
    int x;
    int y;
    
    Point* head_point = (Point*)malloc(sizeof(Point));
    assert(head_point != NULL);
    fscanf(file, "%d %d", &x, &y);
    head_point -> x = x;
    head_point -> y = y;
    
    int i = 1;
    while (fscanf(file, "%d %d", &x, &y) == 2) { // reads 2 ints at a time, fscanf returns 2
        head_point = (Point*) realloc(head_point, ((i + 1)* sizeof(Point)));
        assert(head_point != NULL);
        head_point[i].x = x;
        head_point[i].y = y;
        i++;
    }
     *count = i;
    return head_point;
}

// qsort returns -1 to move value to the left of array
// returns 1 to move value to right of array to get points in sorted order by x ascending first
// then by y value ascending
int compare_fxn(const void* a, const void* b) {
    Point* pointA = (Point*) a;
    Point* pointB = (Point*) b;
    
    // compare x
    if (pointA->x < pointB->x) return -1; // if less move to front
    if (pointA->x > pointB->x) return 1; // if more move to back
    
    // x is same, look at y
    if (pointA->y < pointB->y) return -1; // if less move to front
    if (pointA->y > pointB->y) return 1; // if more, move to back
    
    // if x and y same, do nothing
    return 0;
}

// Allocate memory for a point, assign x,y data to point, assign next of point to NULL
Node* create_node(Point point) {
    Node* node = (Node*) malloc(sizeof(Node));
    assert(node != NULL);
    node->point = point;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Build balanced binary search tree based on qsorted array of
// points, top node is the middle, and create balanced tree using the
// middle and either l or r most point
Node* build_bst(Point* arr, int l, int r) {
    if (l > r) return NULL;
    
    int m = (l + r) / 2; 
    
    Node* node = (Node*) malloc(sizeof(Node));
    assert(node != NULL);
    
    node->point = arr[m];
    node->left = build_bst(arr, l, m - 1);
    node->right = build_bst(arr, m + 1, r);

    return node;
}

// Determine how many points are on or within the given circle using circle
// equation (h-x)^2 + (k-y)^2 <= r^2 where h,k is the center coordinates
// If x is on left side of circle within max dist, move to left to keep checking until no longer in
// If x is on right side of circle within max dist, move to right to keep checking until no longer in
int collision_detect(Node* bst, int centerx, int centery, int radius, int collisions) {
    if (bst == NULL) return collisions; // reached end of branch
    
    // look at left side, if is greater than or equal to left most boundary or bottom most boundary
    if (bst->point.x >= centerx - radius) {
        collisions = collision_detect(bst->left, centerx, centery, radius, collisions);
    }
    // look at right side, if it less than or equal to right most boundary or top most boundary
    if (bst->point.x <= centerx + radius) {
        collisions = collision_detect(bst->right, centerx, centery, radius, collisions);
    }

    int circ_eq = (centerx - bst->point.x) * (centerx - bst->point.x) + (centery - bst->point.y) *
    (centery - bst->point.y);
    if (circ_eq <= radius * radius) collisions++;

    return collisions;
}

// Frees all left, then all right, and head node of binary search tree
void free_bst(Node* bst) {
    if (bst == NULL) {
        return;
    }

    free_bst(bst->left); // free all left
    free_bst(bst->right); // all right

    free(bst);
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 0; // args ./a5 and filename
    char* filename = argv[1];

    FILE* file = (FILE*) fopen(filename, "r"); // open txt file
    assert(file != NULL);
    
    int count;
    Point* file_point = read_in_points(file, &count); // returns head point
    fclose(file);
    Node* bst = NULL; // initialize BST

    qsort(file_point, count, sizeof(Point), compare_fxn);
    
    // Build BST with file points and head_point node
    bst = build_bst(file_point, 0, count - 1);

    int centerx;
    int centery;
    int radius;
    int collisions;
    // Take circle parameters until input is not valid (not 3 ints)
    while (scanf("%d %d %d", &centerx, &centery, &radius) == 3) {
        collisions = collision_detect(bst, centerx, centery, radius, 0);
        printf("%d\n", collisions);
    }

    free(file_point);
    free_bst(bst);
    return 0;
}