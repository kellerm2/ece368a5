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

Point* read_in_points(FILE* file);
Node* create_node(Point* point);
Node* build_bst(Node* head_point, Point point, int height);
int collision_detect(Node* bst, int centerx, int centery, int radius, int collisions);
void free_bst(Node* bst);

// Read in x y from input file, assign first to head_point
// Create next point of prev point by reading in the rest of the file
// contents until there are no more items from the file to be read
Point* read_in_points(FILE* file) {
    int x;
    int y;
    
    Point* head_point = (Point*)malloc(sizeof(Point));
    assert(head_point != NULL);
    fscanf(file, "%d %d", &x, &y);
    head_point -> x = x;
    head_point -> y = y;
    
    int i = 1;
    while (fscanf(file, "%d %d", &x, &y) == 2) { // reads 2 ints at a time, fscanf returns 2
        head_point = (Point*) realloc(head_point, (++i * sizeof(Point)));
        assert(head_point != NULL);
        head_point[i] -> x = x;
        head_point[i] -> y = y;
    }

    return head_point;
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

// Build binary search tree using balance between -1 and 1 to maintain height between
// branches
Node* build_bst(Node* head_point, Point point, int height) {
    if (head_point == NULL) { // found place, need to create new node
        Node* node = create_node(point, x, y);
        return node;
    }
    
    int top_dist = height % 2; // working in 2 dimensions, so if height is even need to 
    // look to x coordinate, if height odd, need to look to y coordinate to balance

    if (top_dist == 0) { 
        if (point.x < head_point->point.x) // if x less than head x, move to left of head
            head_point->left = build_bst(head_point->left, point, top_dist + 1);
    } 
    else {
        if (point.y < head_point->point.y) // if y less then head y, move to left of head
            head_point->left = build_bst(head_point->left, point, top_dist + 1);
    }

    return head_point;
}

// Determine how many points are on or within the given circle using circle
// equation (h-x)^2 + (k-y)^2 <= r^2 where h,k is the center coordinates
int collision_detect(Node* bst, int centerx, int centery, int radius, int collisions) {
    if (bst == NULL) return 0;
    
    int circ_eq = pow((centerx - point->x), 2) + pow((centery - point->y), 2);
    if (circ_eq <= pow(radius, 2)) collisions++;
    
    // is x coordinate within max range of x across circle across center
    if (bst->point.x >= centerx - radius && bst->point.x <= centerx + radius) {
        // look at left of current to check if within max range
        collision_detect(bst->left, centerx, centery, radius, collisions);
        // look at right to check if within max range
        collision_detect(bst->right, centerx, centery, radius, collisions);
    } 
    // is x coordinate within max range of x across circle across center
    else if (bst->point.y >= centery - radius && bst->point.y <= centery + radius) {
        // look at left of current to check if within max range
        collision_detect(bst->left, centerx, centery, radius, collisions);
        // look at right to check if within max range
        collision_detect(bst->right, centerx, centery, radius, collisions);
    }

    return collisions;
}

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
    
    // Build BST with file points and head_point node
    while (file_point[i]) {
        bst = build_bst(bst, file_point[i], 0); // start at height 0 since bst is empty
        i++;
    }

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