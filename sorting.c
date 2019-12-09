#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "sorting.h"

void sort(struct darray* arr, int select){

  switch(select){
    case BINARY_SEARCH_ONE   : insertion_sort(arr); break;
    case BINARY_SEARCH_TWO   : quick_sort(arr, 0, arr->size-1); break;
    case BINARY_SEARCH_THREE : bubble_sort(arr); break;
    case BINARY_SEARCH_FOUR  : merge_sort(arr, 0, arr->size-1); break;
    case BINARY_SEARCH_FIVE  : bucket_sort(arr); break;
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
  int i;
  // The index in the sorted list
  int j;

  // Start with the second element in the original list
  for (i = 1; i < arr->size; i++)
  {
    // Use that element as key
    key = arr->cells[i];

    // Loop from the previous element until the end of the list
    // Move any element greater than the key to the right of the list
    j = i-1;
    while (j >= 0 && compare(arr->cells[j], key) > 0)
    {
      arr->cells[j+1] = arr->cells[j];
      j--;
    }
    // Insert the key after the first element that is not greater than itself
    arr->cells[j + 1] = key;
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

// Bubble sort implementation
void bubble_sort(struct darray* arr)
{
  int n = arr->size;
  int i, j;
  for(i=0; i < n; i++)
    for(j=0; j < n-i-1; j++)
      if(compare(arr->cells[j], arr->cells[j+1]) > 0)
        swap(&arr->cells[j], &arr->cells[j+1]);
}

void merge_sort(struct darray* arr, int low, int high)
{
  // High and low stand for high and low indexes of the subarrays
  if(low < high)
  {
    // The middle
    int m = low+(high-low)/2;

    // Sort the first and second arrays
    merge_sort(arr, low, m);
    merge_sort(arr, m+1, high);

    // Final merge
    merge(arr, low, m, high);
  }
}

// Helper function of bucket sort
void merge(struct darray* arr, int low, int m, int high)
{
  int i,j,k;
  // Lenghts of the two arrays
  int len1 = m - low + 1;
  int len2 = high - m;

  // Create subarrays
  Value_Type L[len1];
  Value_Type R[len2];

  // Add the corresponing elements to subarrays
  for(i=0; i<len1; i++)
    L[i] = arr->cells[low + i];
  for(j=0; j<len2; j++)
    R[j] = arr->cells[m + 1 + j];

  // Merging the arrays back into our orig array
  i = 0; // index of first subarrays
  j = 0; // index of second subarray
  k = low; // index of merged subarray
  while(i < len1 && j < len2)
  {
    // Copy whatever element is smaller to the orig array
    if(compare(L[i], R[j]) <= 0)
    {
      arr->cells[k] = L[i];
      i++;
    }
    else
    {
      arr->cells[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy the elements from L to arr
  while(i < len1)
  {
    arr->cells[k] = L[i];
    i++;
    k++;
  }

  // Copy the elements from R to arr
  while(j < len2)
  {
    arr->cells[k] = R[j];
    j++;
    k++;
  }
}

void bucket_sort(struct darray* arr)
{
  // Bucket for each letter of the alphabet
  struct darray* bucket[26];
  int i, j;
  for(i=0; i < 26; i++)
    bucket[i] = initialize_set(10);

  // Taking the ascii value of the first character and assigning it
  // to a corresponing bucket
  for(i=0; i < arr->size; i++)
    insert(arr->cells[i], bucket[(int)arr->cells[i][0] % 32 - 1]);

  // Use bubble sort to sort each bucket
  for(i=0; i < 26 ; i++)
    bubble_sort(bucket[i]);

  // Swap the values of the sorted buckets into the array
  int index = 0;
  for(i=0; i < 26 ; i++)
    for(j=0; j < bucket[i]->size; j++)
      swap(&arr->cells[index++], &bucket[i]->cells[j]);

  // Freeing up memory
  for(i=0; i < 26 ; i++)
    tidy(bucket[i]);
}
