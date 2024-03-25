#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int extraMemoryAllocated;

void *Alloc(size_t sz) {
  extraMemoryAllocated += sz;
  size_t *ret = malloc(sizeof(size_t) + sz);
  *ret = sz;
  printf("Extra memory allocated, size: %ld\n", sz);
  return &ret[1];
}

void DeAlloc(void *ptr) {
  size_t *pSz = (size_t *)ptr - 1;
  extraMemoryAllocated -= *pSz;
  printf("Extra memory deallocated, size: %ld\n", *pSz);
  free((size_t *)ptr - 1);
}

size_t Size(void *ptr) { return ((size_t *)ptr)[-1]; }

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) {
  
  
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
  if (l < r) {
    // calculate midpoint
    int m = (l + r) / 2;

    // sort the lower and upper halves
    mergeSort(pData, l, m);
    mergeSort(pData, m + 1, r);

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // create temp arrays
    int *L = (int *)Alloc(n1 * sizeof(int));
    int *R = (int *)Alloc(n2 * sizeof(int));

    // copy data to L and R
    for (i = 0; i < n1; i++) {
      L[i] = pData[l + i];
    }
    for (j = 0; j < n2; j++) {
      R[j] = pData[m + 1 + j];
    }

    // merge the temp arrays
    i = 0; // first subarray
    j = 0; // second subarray
    k = l; // merged subarray
    while (i < n1 && j < n2) {
      if (L[i] <= R[j]) {
        pData[k] = L[i];
        i++;
      } else {
        pData[k] = R[j];
        j++;
      }
      k++;
    }

    // copy remaining elements of L, if any
    while (i < n1) {
      pData[k] = L[i];
      i++;
      k++;
    }

    // copy remaining elements of R, if any
    while (j < n2) {
      pData[k] = R[j];
      j++;
      k++;
    }

    // free up temp arrays
    DeAlloc(L);
    DeAlloc(R);
  }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int *pData, int n) {
  int i, j, key;

  for (i = 1; i < n; i++)
  {
    key = pData[i];
    j = i - 1;

    //move until sorted
    while(j>=0 && pData[j]>key)
    {
      pData[j+1] = pData[j];
      j = j-1;
    }
    pData[j+1] = key;
  }
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int *pData, int n) {
  int swapped;

  for (int i=0; i<n; i++)
  {
    swapped = 0;

    for (int j=0; j<n-i-1; j++)
    {
      if(pData[j] > pData[j+1])
      {
        int temp = (pData[j]);
        (pData[j]) = (pData[j+1]);
        (pData[j+1]) = temp;
        swapped = 1;
      }
    }

    if(swapped == 0)
    {
      break;
    }
  }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int *pData, int n) {
  int min;

  for (int i=0; i<n-1; i++)
    {
      min = i;
      for (int j=i+1; j<n; j++)
        if(pData[j] < pData[min])
          min = j;

      if(min != i)
      {
        int temp = (pData[i]);
        (pData[i]) = (pData[min]);
        (pData[min]) = temp;
      }
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (int *)Alloc(sizeof(int) * dataSz);
    // Implement parse data block

    for (int i = 0; i < dataSz; i++) {
      fscanf(inFile, "%d", &(*ppData)[i]);
    }
  }

  return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
  int i, sz = dataSz - 100;
  printf("\tData:\n\t");
  for (i = 0; i < 100; ++i) {
    printf("%d ", pData[i]);
  }
  printf("\n\t");

  for (i = sz; i < dataSz; ++i) {
    printf("%d ", pData[i]);
  }
  printf("\n\n");
}

int main(void) {
  clock_t start, end;
  int i;
  double cpu_time_used;
  char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

  for (i = 0; i < 3; ++i) {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)Alloc(sizeof(int) * dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n", dataSz);
    printf("---------------------------\n");

    printf("Selection Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    selectionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Insertion Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    insertionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Bubble Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    bubbleSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Heap Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    heapSort(pDataCopy, dataSz - 1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    DeAlloc(pDataCopy);
    DeAlloc(pDataSrc);
  }
}
