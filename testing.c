#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int * poop(void) {
    int arr[100];
    for (int i = 0; i < 100; i ++) {
        arr[i] = 9;
    }
    return arr;
}

int main(void) {
    int arr[100] = poop());
    memset(arr, 0, sizeof(int) * 10);
    for (int i = 0; i < 10; i++) {
        printf("%d\n", arr[i]);
    }
    
}