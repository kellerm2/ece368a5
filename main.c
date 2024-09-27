#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

typedef struct Point {
    int x;
    int y;
    struct Point* next;
} Point;

Point* read_in_points(FILE* file);
Point* create_point(int x, int y);
int collisions_detect(Point* point, int centerx, int centery, int radius);

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
    head_point -> next = NULL;
    Point* prev = head_point; // set previous to be head
    Point* current_point;
    
    while (fscanf(file, "%d %d", &x, &y) == 2) { // reads 2 ints at a time, fscanf returns 2
        current_point = create_point(x, y);
        prev -> next = current_point; // next of previous is new point
        prev = current_point; // current point is now prev point
    }

    return head_point;
}

// Allocate memory for a point, assign x,y data to point, assign next of point to NULL
Point* create_point(int x, int y) {
    Point* point = (Point*) malloc(sizeof(Point));
    assert(point != NULL);
    point -> x = x;
    point -> y = y;
    point -> next = NULL;

    return point;
}

// Determine how many points are on or within the given circle using circle
// equation (h-x)^2 + (k-y)^2 <= r^2 where h,k is the center coordinates
int collision_detect(Point* point, int centerx, int centery, int radius) {
    int collisions = 0;
    while (point != NULL) {
        int circ_eq = pow((centerx - point->x), 2) + pow((centery - point->y), 2);
        if (circ_eq <= pow(radius, 2)) collisions++;
        point = point -> next;
    }

    return collisions;
}

int main() {
    const char* filename;
    scanf("%s", &filename); // get txt file name
    FILE* file = (FILE*) fopen(filename, "r"); // open txt file
    assert(file != NULL);
    
    Point* head_point = read_in_points(file); // returns head point

    int centerx;
    int centery;
    int radius;
    int collisions;
    // Take circle parameters until input is not valid (not 3 ints)
    while (scanf("%d %d %d", &centerx, &centery, &radius) == 3) {
        collisions = collision_detect(head_point, centerx, centery, radius);
        printf("%d\n", collisions);
    }
    
    Point* point = head_point;
    Point* to_free;
    while (point != NULL) { // free points until linked list is empty/NULL
        to_free = point; // point to be freed
        point = point -> next; // assign point to be next point
        free(to_free); // free to be freed
    }
    return 0;
}