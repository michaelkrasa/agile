#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include "sorting.h"

void sort(struct darray* arr, int select){

  switch(select){
    case BINARY_SEARCH_ONE   : insertion_sort(arr); break;
    case BINARY_SEARCH_TWO   : quick_sort(arr, 0, arr->size - 1); break;
    case BINARY_SEARCH_THREE :
    case BINARY_SEARCH_FOUR  :
    case BINARY_SEARCH_FIVE  :
    default:
      fprintf(stderr,
              "The value %d is not supported in sorting.c\n",
              select);
      // Keep this exit code as the tests treat 23 specially
      exit(23);
  }
}


// You may find this helpful
void swap(char* *a, char* *b)
{
        char* temp = *a;
        *a = *b;
        *b = temp;
}

// Most important part of the quick sort algorithm
int partition(struct darray* arr, int low, int high)
{
  // Set initial pivot point to be the highest element in the array
  Value_Type pivot = arr->cells[high];
  int i = low - 1;
  int j;

  for (j = low; j <= high - 1; j++)
  {
    // If element is smaller than the pivot
    if (compare(arr->cells[j], pivot) <= 0)
    {
      i++; // Increment the index of the smaller element
      swap(&arr->cells[i], &arr->cells[j]);
    }
  }
  // Swap pivot and i+1
  swap(&arr->cells[i + 1], &arr->cells[high]);
  return (i + 1); // Our new pivot for next pass
}

void insertion_sort(struct darray* arr)
{
  // The element to be inserted into the sorted list
  Value_Type key;
  // The index in the original list
  int index;
  // The index in the sorted list
  int sortedListIndex;

  // Start with the second element in the original list
  for (index = 1; index < arr->size; index++)
  {
    // Use that element as key
    key = arr->cells[index];

    // Start looping from the previous element until the start of the list
    // And move any element greater than the key to the right of the list
    sortedListIndex = index-1;
    while (sortedListIndex >= 0 && compare(arr->cells[sortedListIndex], key) > 0)
    {
      arr->cells[sortedListIndex+1] = arr->cells[sortedListIndex];
      sortedListIndex = sortedListIndex - 1;
    }
    // Insert the key after the first element that is not greater than itself
    arr->cells[sortedListIndex + 1] = key;
  }
}

// Quick sort implementation
void quick_sort(struct darray* arr, int low, int high)
{
  if (low < high)
  {
    // Partition and decide the pivot
    int pivot = partition(arr, low, high);

    // Recursive call for each side of the pivot
    quick_sort(arr, low, pivot - 1);
    quick_sort(arr, pivot + 1, high);
  }
}
