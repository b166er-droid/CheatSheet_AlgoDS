#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <climits>    //INT_MIN, INT_MAX
using namespace std;

#include "binary_heap.h" // includes min-heap and max-heap implementations

/* =========================Helper declarations and functions ====================================*/

/* =======================Sort Array of Strings with Anagrams Next To Each Other==================*/
// From 'Cracking the Coding Interview'
void sortAnagrams (vector<string>& array)
{
    // O(N) time, O(N) space
    unordered_map<string, list<string> > anagrams;

    for (auto& e : array) {
        string key = e;
        sort(key.begin(), key.end());

        if (anagrams.find(key) == anagrams.end()) {
            anagrams[key] = list<string>();
        }
        anagrams[key].push_back(e);
    }

    int i = 0;
    for (auto& a : anagrams) {
        list<string>& lst = a.second;
        for (auto& e : lst) {
            array[i++] = e;
        }
    }
}

/* ======================All pairs differing by K in a sorted vector==============================*/
// http://www.careercup.com/question?id=5727804001878016
// Given an array of positive, unique, increasingly sorted numbers A,
//      e.g. A = [1, 2, 3, 5, 6, 8, 9, 11, 12, 13].
// Given a positive value K, e.g. K = 3. Output all pairs in A that differ exactly by K. No repeats

void pairsDifferingK (vector<int>& arr, int k) { // O(N) time, O(1) space
    int i = 0;
    int j = 1;
    while (j < arr.size()) {
        if      ( (arr[j] - arr[i]) > k)  i++;
        else if ( (arr[j] - arr[i]) < k)  j++;
        else {
            cout << "(" << i << ", " << j << ") ";
            i++;            // Note that elements must be unique, otherwise this step does not work
        }                   // Consider K = 20 and [ 60, 60, 80, 80] : we either advance i or recede
                            // j, so either indexes (0, 2), (1,2) are generated or (0, 2), (0, 3)
    }
}

/* =========================All pairs summing to value K in a vector==============================*/
// Not necessarily sorted vector, no repeating pairs

void pairsSummingK (vector<int>& arr, int k) {
    // O(N) time, O(N) space

    unordered_map<int, list<int> > val_pos;
    for (int i = 0; i < arr.size(); i++)
    {
        if (  val_pos.find( k - arr[i] ) != val_pos.end() ) {
            list<int>& l = val_pos[ k - arr[i]];
            for (auto& e: l)
                    cout << "(" << e << ", " << i << ") ";
        }
        val_pos[arr[i]].push_back(i);
    }
}

void pairsSummingK_ (vector<int>& arr, int k) {
    // O(N*logN) time, O(1) space, modifies vector!
    // WARNING: pair indexes are different to original arr before sorting

    std::sort(arr.begin(), arr.end());  // O(N log N) assumed
    int i = 0;
    int j = arr.size() - 1;
    while (i <= j) {
        if      (arr[i] + arr[j] > k) j--;
        else if (arr[i] + arr[j] < k) i++;
        else {
            cout << "(" << i << ", " << j << ") ";
            i++;            // Same comment as in pairsDifferingK
        }
    }
}


/* ===========================Given an array of pairs, find all symmetric pairs===================*/
// http://www.geeksforgeeks.org/given-an-array-of-pairs-find-all-symmetric-pairs-in-it/
// Assume first elements of all pairs are distinct !

void symmetricPairs (vector<pair<int, int> >& array) {
    // O(N) time, O(N) space

    unordered_map<int, pair<int, int> > mem; // key is first element of pair, and pair contains
                                             // second element and index position within array

    for (int i = 0; i < array.size(); i++) {
        pair<int,int> el = array[i];
        if ( mem.find(el.second) != mem.end() ) {
            if ( mem[el.second].first == el.first ) {
                cout << "(" << mem[el.second].second << ", " << i << ") ";
            }
        } else {
            // In case previous 'if' was executed, we don't need to add the element to mem
            // becase first elements of all pairs are unique
            pair<int, int> tmp (el.second, i);
            mem[el.first] = tmp;
        }
    }
}

/* =====================Unsorted Subarray Location That Sorting Makes Complete Sorted=============*/
// http://www.geeksforgeeks.org/minimum-length-unsorted-subarray-sorting-which-makes-the-complete-\
//        array-sorted/

void unsortedSubarray (vector<int>& arr, int& s, int& e) {
    // O(N) time, O(1) space

    s = 0; e = arr.size() - 1;

    // From left to right, find element that is smaller than the previous one. Include previous one
    // as initial starting subarray point (to be refined later, potentially including more elements)
    // We include it, because in the best case we would need to swap it with the next element
    for (s = 0; s < arr.size() - 1; s++) {
        if (arr[s] > arr[s+1]) break;
    }

    if (s == arr.size() - 1) {e = 0; return;}

    // From right to left, find element that is smaller than the previous one. Include previous one
    // as initial ending subarray point (to be refined later, potentially including more elements)
    // We include it, because in the best case we would need to swap it with the next element
    for (e = arr.size() - 1; e > 0; e--) {
        if (arr[e] < arr[e-1]) break;
    }

    // Find the minimum and maximum values in arr[s..e]
    int min = arr[s]; int max = arr[s];
    for (int i = s +1; i <= e; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    // Find first element in arr[0..s-1]  that is bigger than min,  make s to point to that element
    for (int i = 0; i <= s - 1; i++) {
        if (arr[i] > min) { s = i; break; }
    }

    // Find last element in arr[e+1..n-1] that is smaller than max, make e to point to that element
    for (int i = arr.size() - 1; i >= e+1; i--) {
        if (arr[i] < max) { e = i; break; }
    }
}

/* ======================Maximum of minimum values for every window in an array===================*/
// http://www.geeksforgeeks.org/find-the-maximum-of-minimums-for-every-window-size-in-a-given-array/

void maxOfMin (vector<int>& array) {
    // O(N^3) time, O(1) space

    for (int s = 1; s <= array.size(); s++) {  // Iterate  over possible sizes
        int window_max = array[0];
        for (int i = 0; i <= (array.size() - s); i++ )   // Iterate over starting points for a size
        {
            int window_min = array[i];
            for (int j = 1; j < s; j++) {
                window_min = min (window_min , array[i+j]);
            }
            window_max = max (window_max, window_min);
        }
        cout << window_max << ", ";
    }
}

/* =========================Maximum Sum in Contiguous Subarray====================================*/
// http://www.geeksforgeeks.org/largest-sum-contiguous-subarray/
// http://karmaandcoding.blogspot.com.es/2011/03/maximum-contiguous-sum-in-array.html

int maxSumSubarray (vector<int>& array, int& s, int& e) {
    // O(N) time, O(1) space
    // IDEA: when we find a negative sum at a given position, if we increase the subarray we will
    //       not find a biggest sum. Therefore we can start considering a new subarray in the next
    //       position

    int max_sum    = 0;  // maximum sum is 0 in the worst case (no subarray)
    int curr_sum   = 0;
    int curr_start = 0;
    int max_start  = 0;
    int max_end    = 0;

    for (int i = 0; i < array.size(); i++) {
        curr_sum = curr_sum + array[i];
        if (curr_sum < 0) {
            curr_sum = 0;
            curr_start = i + 1;
        } else {
            if (curr_sum > max_sum) {
                max_sum   = curr_sum;
                max_start = curr_start;
                max_end   = i;
            }
        }
    }
    s = max_start; e = max_end;
    return max_sum;
}

/* =======================Find Subarray with Given Sum V (non-negative contents)==================*/
// http://www.geeksforgeeks.org/find-subarray-with-given-sum/
// Note: non-negative numbers, non sorted array

void findSubarrayGivenSum_Positive (vector<int>& array, int v, int& s, int& e) {
    // O(N) time, O(1) space

    int curr_start = 0;
    int curr_sum   = 0;
    bool found     = false;

    for (int i = 0; i < array.size()  && !found; i++) {
        curr_sum += array[i];
        if (curr_sum > v) {
            // If curr_sum exceeds value, then when move curr_start pointer either until we find
            // the sum value, or until curr_start overpasses i.
            // curr_sum is increasing
            while ((curr_start <= i) && (curr_sum > v)) {
                curr_sum -= array[curr_start];
                curr_start++;
            }
        }
        if (curr_sum == v) { found = true; s = curr_start; e = i;}
    }

    if (!found) { s= array.size() - 1; e = 0; }
}

void findSubarrayGivenSum_Positive_ (vector<int>& array, int v, int& s, int& e) { //inefficient
    // O(N^2) time, O(1) space
    bool found    = false;

    for (int i = 0; i < array.size() && !found; i++) {
        int  curr_sum = 0;
        for (int j = i; (j < array.size()) && !found; j++) {
            curr_sum = curr_sum + array[j];
            if (curr_sum == v) {
                s = i; e = j;
                found = true;
            }
        }
    }
    if (!found ) { s = array.size() - 1; e = 0; }
}

/* ======================Find Subarray with Given Sum V (with negative contents)==================*/
// http://www.geeksforgeeks.org/find-subarray-with-given-sum-in-array-of-integers/
// Extension of above one

void findSubarrayGivenSum (vector<int>& array, int v, int& s, int& e ) {
    // O(N) time, O(N) space

    // IDEA: Iterate over all positions and accumulate the sum, if at a given index we have a sum x
    //       then we can find in a map if there has been a position whose accumulated value from pos
    //       0 is x-s . If we don't include that prefix subarray, then the sum would be x-(x-s) = s
    //

    unordered_map<int,int> prefix_sum; // key is accumulated sum and value is array index when this
                                       // accumulated sum was observed
    int accum_sum = 0;
    bool found    = false;

    for (int i = 0; i < array.size() && !found; i++) {
        accum_sum += array[i];
        if (accum_sum == v) { s = 0; e = i; found = true; }
        else {
            int offset = accum_sum - v;
            if (prefix_sum.find(offset) != prefix_sum.end()) {
                s = prefix_sum[offset] + 1;
                e = i;
                found = true; //break
            }  else {
                // We keep the longest subarray (first positions are memorized) )
                if (prefix_sum.find(accum_sum) == prefix_sum.end()) {
                    prefix_sum[accum_sum] = i;
                }
            }
        }
    }
    if (!found) { s = array.size() - 1 ; e = 0; }
}

/* =========================Find LARGEST Subarray with 0 Sum======================================*/
// Very similar to previous one, V=0, but we need the largest one
// http://www.geeksforgeeks.org/find-the-largest-subarray-with-0-sum/

int bigestSubarraySummingZero (vector<int>& array, int& s, int& e) {
    // O(N) time, O(N) space

    unordered_map<int,int> prefix_sum;
    int max_len   = 0;
    int accum_sum = 0;

    s = array.size() - 1;
    e = 0;

    for (int i = 0; i < array.size(); i++) {
        accum_sum += array[i];
        if (accum_sum == 0) { max_len = i; s = 0; e = i; }
        else {
            int offset = accum_sum - 0; //redundant, but added to show similarity wrt previous probl.
            if (prefix_sum.find(offset) != prefix_sum.end()) {
                if (  ( i - (prefix_sum[offset]+1) + 1)  > max_len) {
                    max_len =  i - (prefix_sum[offset] + 1) + 1;
                    s = prefix_sum[offset] + 1;
                    e = i;
                }
            } else {
                prefix_sum[accum_sum] = i;
            }
        }
    }
    return max_len;
}

int bigestSubarraySummingZero_ (vector<int>& array, int& s, int& e) {
    // O(N^2) time
    int max_len = 0;
    s = array.size() - 1;
    e = 0;

    for (int i = 0 ; i < array.size(); i++) {
        int curr_sum = 0;
        for (int j = i; j < array.size(); j++) {
            curr_sum += array[j];
            if (curr_sum == 0) {
                if ((j-i + 1) > max_len) { max_len = j-i + 1;  s = i; e = j; }
            }
        }
    }
}

/* =================Given 2 sorted arrays A & B, merge them into B (enough space provided)========*/
void mergeArrays (int a[], int b[], int sizeA, int sizeB) {
    // TIP: start from end
    int ptrM = sizeA + sizeB - 1;
    int ptrA = sizeA - 1;
    int ptrB = sizeB - 1;

    while (ptrA >= 0  && ptrB >= 0) {
        if (a[ptrA] > b[ptrB])  { b[ptrM] = a[ptrA]; ptrA--; }
        else                    { b[ptrM] = b[ptrB]; ptrB--; }
        ptrM--;
    }

    for (int i = ptrA; i >= 0; i--) {
        b[ptrM] = a[i];
        ptrM--;
    }
}

/* =================Given 2 sorted arrays A & B, merge them in-place O(1) space===================*/
// http://www.geeksforgeeks.org/merge-two-sorted-arrays-o1-extra-space/
// Initial numbers (after complete sorting) are in the first array and the remaining numbers are in
// the second array
void mergeInPlace(int* ar1, int* ar2, int m, int n)
{
    // O(m * n) time! That's why mergesort is not used for arrays, unless O(N/2) space used
    // The worst case occurs when all elements of ar1[] are greater than all elements of arb2[]

    /*
       The idea is to begin from last element of ar2[] and search it in ar1[]. If there is a greater
       element in ar1[], then we move last element of ar1[] to ar2[].
       To keep ar1[] and ar2[] sorted, we need to place last element of ar2[] at correct place in
       ar1[]. We can use Insertion Sort type of insertion for this
    */

    // Iterate through all elements of ar2[] starting from the last element
    for (int i = n - 1; i >= 0; i--) {
        /* Find the smallest element greater than ar2[i]. Move all elements one position ahead till
           the smallest greater element is not found */
        int j, last = ar1[m-1];
        for (j = m - 2; j >= 0 && ar1[j] > ar2[i]; j--)
            ar1[j + 1] = ar1[j];

        // If there was a greater element
        if (j != m - 2 || last > ar2[i]) {
            ar1[j + 1] = ar2[i];
            ar2[i]     = last;
        }
    }
}

/* ===========================Sort Huge File with One String Per Line=============================*/
// From 'Cracking the Coding Interview'

// SOLUTION:
//  - We don't want to bring the whole file into memory. Therefore use external sort.
//  - We can use a modified merge sort: we divide the huge file in diffent chunks that can be read
//    into memory. Each chunk has x MB, where x is the amount of available memory.
//  - We start with two chunks, and use the merge algorithm to sort them. We can traverse the chunks
//    from the end, decreasing the chunk sizes as we merge them. This avoids increasing the extra
//    space needed in the FS.
//  - Then, we keep merging new chunks.


/* ==========================Search in a row-wise and column-wise sorted matrix===================*/
// http://www.geeksforgeeks.org/search-in-row-wise-and-column-wise-sorted-matrix/

#define N 4
pair<int,int> searchSortedMatrix (int array[N][N], int val) {
    // O(N) time, O(1) space
    // TIP: start from top-right element
    int i = 0; int j = N - 1;
    while (i < N && j >= 0) {
        if       ( array[i][j] > val ) j--;
        else if  ( array[i][j] < val ) i++;
        else     return make_pair(i, j);
    }

    return make_pair(-1,-1);
}

/* ===============================Binary Search===================================================*/
// http://www.geeksforgeeks.org/problem-binary-search-implementations/
// Array is sorted

int binarySearch_ (vector<int>& array, int s, int e, int v) { //Recursive
    // O(log N) time

    if (s > e) return -1;

    int mid = s + ((e - s)/2);
    if      (array[mid] == v) return mid;
    else if (array[mid] > v)  return binarySearch_(array, s, mid-1, v);
    else                      return binarySearch_(array, mid+1, e, v);
}
int binarySearch (vector<int>& array, int v) {  // Iterative
    // O(log N) time
    int s = 0;
    int e = array.size() - 1;

    while (s <= e) {
        int mid = s + ((e-s)/2);
        if      (array[mid] == v) return mid;
        else if (array[mid] >  v) { e = mid - 1; }
        else                      { s = mid + 1; }
    }
    return -1;
}

/*============================Partition an Array around a value at position p=====================*/
// Reorders array based on pivot, returns position such that all previous elements are <= pivot
// and all folowing elements are > pivot

int partition_ (vector<int>& array, int l, int r, int p) {
    // O(N) time
    int pivot = array[p];
    array[p] = array[l]; array[l] = pivot; // swap array[p] and array[l]
    int i = l; int j = r+1;

    while (true) {
        do { ++i; } while (i <= r && array[i] <= pivot);
        do { --j; } while (array[j] > pivot);

        if (i >=j ) break;
        //else
        int t = array[i]; array[i] = array[j]; array[j] = t; //swap
    }

    int t = array[l]; array[l] = array[j]; array[j] = t;
    return j;
}

int partition(vector<int>& array, int l, int r) {
    return partition_(array, l, r, l);
}

/* ==============================0s to Left, 1s to Right==========================================*/
// http://www.geeksforgeeks.org/segregate-0s-and-1s-in-an-array-by-traversing-array-once/
// Solution 1: count 0s, count 1s in one pass, and in a second pass add them
// Solution 2: single pass, O(N) time. Similar to partition method of quicksort

void segregateO1 (vector<int>& array) {
    // O(N) time, 1 pass
    int left = 0; int right = array.size() - 1;

    while (left < right) {
        while ( (left < right) && (array[left] < array[right]) ) { left++; right--; }
        if (left < right) {
            array[left] = 0; array[right] = 1;
            left++; right --;
        }
    }
}

/* ==========================0s First, then 1s then 2s (Dutch Flag)===============================*/
// http://www.geeksforgeeks.org/sort-an-array-of-0s-1s-and-2s/
// https://sites.google.com/site/spaceofjameschen/home/sort-and-search/dutch-national-flag

void segregate012 (vector<int>& array) {
    // O(N) time, 1 pass
    // IDEA:  partition modification

    int lo = 0;  int hi = array.size() - 1; int mid = 0;
    // INVARIANT: [mid...hi] is unsorted, [0..lo) and (hi, N-1] are sorted
    // But mid is lo (0) at the beginning.

    while (mid <= hi) {
        if (array[mid] == 0) {
            int tmp = array[lo]; array[lo] = array[mid]; array[mid] = tmp;
            lo++; mid++;
        } else if (array[mid] == 1) {
            mid++;
        } else if (array[mid] == 2) {
            int tmp = array[mid]; array[mid] = array[hi]; array[hi] = tmp;
            hi--;
        }
    }
}

/* ====================== QuickSelect ( k'th Smallest Element) ===================================*/
// http://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-2-expected-linear\
//       -time/
bool quickSelect (vector<int>& array, int l, int r, int k, int& result) {

    // T(N) = T(N/2) + 1 --> O(N) linear in the average case (as long as pivot lies in the middle)
    // Worst case is O(N^2), when array is sorted in inverse order and partition chooses l as pivot!
    // Modifies array, as opposed to min/max heap approach, k is inside [0, array.size()-1]
    if (l > r) return false;

    int p = partition(array, l, r);
    if      (p == k) { result = array[p]; return true;}
    else if (p <  k) { return quickSelect(array, p+1, r, k, result); }
    else             { return quickSelect(array, l, p-1, k, result); }
}

/* ===================================Quick Sort==================================================*/
// http://www.geeksforgeeks.org/iterative-quick-sort/

void quicksort (vector<int>& array, int l, int r) {
    // T(N) = 2*T(N/2) + N --> O(N log N) average case
    // O(N^2) worst case, when pivot is chosen as value in position l, and array is sorted inversely
    // QuickSort is cache-friendly
    // https://www.nayuki.io/page/master-theorem-solver-javascript

    if (l >= r) return;
    int pivot = partition(array, l, r);
    quicksort(array, l, pivot - 1);
    quicksort(array, pivot + 1, r);
}

/* =================================Median of Two SORTED Arrays===================================*/
// http://www.geeksforgeeks.org/median-of-two-sorted-arrays/

int median (int array[], int n) {
    if (n % 2 == 0)  return (array[n/2] + array[n/2 - 1])/2;
    else             return  array[n/2];
}

bool getMedian (int array1[], int array2[], int n, int& result) {
    // Arrays have same size
    // T(N) = T(N/2) + 1 --> O(logN) time

    if (n <= 0) { return false; }
    if (n == 1) { result = ( array1[0]+array2[0] ) / 2;
                  return true; }
    if (n == 2) { result = ( max(array1[0], array2[0])  + min(array1[1], array2[1]) ) / 2;
                  return true; }

    int m1 = median (array1, n);
    int m2 = median (array2, n);

    if (m1 == m2) { result = m1; return true; }
    if (m1  < m2) {    /* If m1 < m2 then median must exist in array1[m1....] and array2[....m2] */
        if (n % 2 == 0)    return getMedian(array1 + n/2 - 1, array2, n - n/2 + 1, result);
        else               return getMedian(array1 + n/2    , array2, n - n/2    , result);
    } else {           /* If m1 > m2 then median must exist in array1[....m1] and array2[m2...] */
        if (n % 2 == 0)    return getMedian(array2 + n/2 - 1, array1, n - n/2 + 1, result);
        else               return getMedian(array2 + n/2    , array1, n - n/2    , result);
    }
}

bool getMedian_ (int array1[], int array2[], int n, int& result) {
    // Arrays have same size. This is a modification of the merge algorithm of two sorted arrays.
    // Idea is traverse both arrays in sorted order, and count traversed elements until they are n.
    // O(N) time

    if (n <= 0) return false;

    int i  =  0; int j = 0;
    int m1 = -1; int m2 = -1;  // stores last 2 visited (minimum) elements

    /* Since there are 2n elements, median will be average of elements at index n-1 and n in the
       array obtained after merging array1 and array2 */
    for (int count = 0; count <= n; count++) {
        if (i == n) {
            /* All elements of array1[] are < than smallest(or first) element of array2[]  */
            m1 = m2; m2 = array2[0];
            break;
        } else if (j == n) {
            /* All elements of array2[] are < than smallest(or first) element of array1[] */
            m1 = m2; m2 = array1[0];
            break;
        }

        if (array1[i] < array2[j]) {
            m1 = m2; m2 = array1[i];
            i++;
        } else {
            m1 = m2; m2 = array2[j];
            j++;
        }
    }

    result =  (m1 + m2)/2;
    return true;
}

/* ==============================Find rotation count in sorted rotated array =====================*/
// http://www.geeksforgeeks.org/find-rotation-count-rotated-sorted-array/

// ASSUMPTION: UNIQUE numbers

int rotationCount (vector<int>& array, int l, int r) { // ONLY for anti-clockwise rotations
    // O(log N) time

    // IDEA: Find index of minimum element, but using Binary Search
    // The minimum element is the only element whose previous is greater than it. If there is
    // no previous element element, then there is no rotation (first element is minimum). We check
    // this condition for middle element by comparing it with (mid-1)’th and (mid+1)’th elements.
    // If minimum element is not at middle (neither mid nor mid + 1), then minimum element lies in
    // either left half or right half. If middle element is smaller than last element, then the
    // minimum element lies in left half. Else minimum element lies in right half.

    if (r < l)  return 0; // Condition to handle the case when array is not rotated
    if (r == l) return r;

    int mid = l + (r - l) / 2;
    if (mid < r && array[mid]> array[mid+1]) return mid+1; // Check if mid+1 is minimum element.
    if (mid > l && array[mid]< array[mid-1]) return mid;   // Check if mid itself is minimum element

    // Decide whether we need to go to left half or right half
    if (array[r] > array[mid]) return rotationCount(array, l, mid - 1);
    else                       return rotationCount(array, mid + 1, r);

}

int rotationCount_ (vector<int>& array) {
    // O(N) time

    // First check for left-order rotation count
    int l = 0;
    for (l = 1; l < array.size(); l++) {
        if (array[l] < array[l-1]) break;
    }

    // Also check for right-order rotation count
    int r = 0;
    for (r = array.size() - 2; r >= 0; r--) {
        if (array[r] > array[r-1]) { r = array.size() - 1 - r; break;}
    }

    // Return minimum rotation
    return min(l, r);
}

/* ===========================Find minimum in sorted rotated array==============================*/
// http://www.geeksforgeeks.org/find-minimum-element-in-a-sorted-and-rotated-array/
// EQUAL TO PREVIOUS CODE
// ASSUMPTION: UNIQUE numbers

int minimumInRotated (vector<int>& array, int l, int r) {
    // O(log N) time

    if (r < l)  return array[0]; // Condition to handle the case when array is not rotated
    if (r == l) return array[r];

    int mid = l + (r - l) / 2;
    if (mid < r && array[mid] > array[mid+1]) return array[mid+1]; // Check for minimum element
    if (mid > l && array[mid] < array[mid-1]) return array[mid];   // Check for minimum element

    if (array[r] > array[mid]) return minimumInRotated(array, l, mid - 1);
    else                       return minimumInRotated(array, mid + 1, r);
}

/* ============================Find in a sorted and rotated array=================================*/
// http://www.geeksforgeeks.org/search-an-element-in-a-sorted-and-pivoted-array/
// From 'Cracking the Coding Interview'
// ASSUMPTION: UNIQUE numbers

int searchRotated (vector<int>& array, int l, int r, int k) {
    // O(log N)
    if (r < l) return -1;

    int mid = l + (r - l)/2;
    if (array[mid] == k) return mid;

    // Check which subarray [l...mid] or [mid+1...h] is sorted (both can be), check which subarray
    // k belongs to
    if (array[mid] >= array[l]) { // [l..mid] is sorted
        if (k < array[mid] && k >= array[l])     return searchRotated(array, l, mid-1, k);
        else                                     return searchRotated(array, mid+1, r, k);
    }
    // (else) :array[mid] <= array[r] HOLDS true
    if (k <= array[r] && k > array[mid])         return searchRotated(array, mid + 1, r, k);
    else                                         return searchRotated(array, l, mid - 1, k);
}

/* =========================Find a Fixed Point in a given array===================================*/
// http://www.geeksforgeeks.org/find-a-fixed-point-in-a-given-array/
// Assume SORTED & UNIQUE elements!

int fixedPoint (vector<int>& array, int s, int e) {
    // O(N log N), divide & conquer, based on binary search T(N) = T(N/2) + 1
    if (s > e) return -1;

    int h = s + (e - s + 1) / 2 ; // (2s + e -s + 1)/2 == (s + e +1 )/2

    if      (array[h] == h)       return h;
    else if (array[h] >  h)       return fixedPoint(array, s, h-1);
    else                          return fixedPoint(array, h+1, e);
}


/* ========================Given Two Arrays, Find Element That Has been Deleted===================*/
// http://www.geeksforgeeks.org/find-the-missing-number/

// Arrays are not in same order
int findMissing (vector<int>& array1, vector<int>& array2) {
    // O(N) time
    int checksumA = 0;
    for (int i = 0; i < array1.size(); i++)
        checksumA = checksumA ^ array1[i];

    int checksumB = 0;
    for (int i = 0; i < array2.size(); i++)
        checksumB = checksumB ^ array2[i];

    return checksumB ^ checksumA;
}

/* ===============Given Two Arrays, Find Element That Has been Deleted (Same Order)===============*/
// http://www.geeksforgeeks.org/find-lost-element-from-a-duplicated-array/

// Arrays are in same order, not necessarily in sorted order!
int findMissingSameOrder (vector<int>& array1, vector<int>& array2, int& pos) {
    //  O(log N) time (like binary search)
    if (array1.size() == 1)     { cout << "A"; pos = 0; return array1[0]; }
    if (array1[0] != array2[0]) { cout << "B"; pos = 0; return array1[0]; }

    int lo = 0; int hi = array1.size() - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;  // Notice no (lo + hi + 1) / 2;

        if (array1[mid] == array2[mid]) { lo = mid; }
        else                            { hi = mid; }  //array1[mid] < array2[mid] !! ALWAYS

        if (hi - 1 == lo) { pos = hi; break; }
    }

    return array1[pos];
}

/* ==============================Max Heap and Min Heap (Binomial Heaps) ==========================*/
// https://en.wikipedia.org/wiki/Binary_heap
// http://www.geeksforgeeks.org/why-is-binary-heap-preferred-over-bst-for-priority-queue/
// http://geeksquiz.com/binary-heap/
// http://www.geeksforgeeks.org/g-fact-85/
// http://www.geeksforgeeks.org/applications-of-heap-data-structure/

/*
    Supports operations:
        - get TOP priority element:    O(1)
        - insert element:              O(log N)
        - remove TOP priority element: O(log N)
        - decrease/increase key:       O(log N)

    - Since Binary Heap is implemented using arrays,there is always better locality of reference and
      operations are more cache friendly than balanced BST.
    - Although operations are of same time complexity, constants in Binary Search Tree are higher.
    - We can build a Binary Heap in O)n= time. Self Balancing BSTs require O)nLogn= time to
      construct.
    - Binary Heap doesn’t require extra space for pointers.
    - There are variations of Binary Heap like Fibonacci Heap that can support insert and
      decrease-key in Θ(1) time
    - Used for order statistics, and Prim and Dijkstra's Graphs Algorithms

    BUT:
    - Searching an element in self-balancing BST is O(Logn) which is O(n) in Binary Heap
    - We can print all elements of BST in sorted order in O(n) time, but Binary Heap requires
      O(nLogn) time
    - Floor and ceil can be found in O(Logn) time in balanced BST.
    - K’th largest/smallest element be found in O(Logn) time by augmenting tree with an additional
      fields: http://www.geeksforgeeks.org/find-k-th-smallest-element-in-bst-order-statistics-in-bst
*/

// Min and Max Heaps Using STL Libraries:
//    http://stackoverflow.com/questions/2786398/is-there-an-easy-way-to-make-a-min-heap-in-c
//    https://codeconnect.wordpress.com/2013/09/05/max-min-heap-using-c-stl/
//    http://stackoverflow.com/questions/7681779/easy-way-to-maintain-a-min-heap-with-stl


/* ====================== k'th Smaller Element Using Heaps =======================================*/
// http://www.geeksforgeeks.org/k-largestor-smallest-elements-in-an-array/
// http://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array/
// http://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-2-expected
//        -linear-time/
// http://www.geeksforgeeks.org/kth-smallestlargest-element-unsorted-array-set-3-worst-case\
//        -linear-time/

int kThSmallest (int array[], int n, int k) {
    // O(K + (N-K)logK) time (worst case) .  Note that QuickSelect has worst case time c. of O(N^2)
    // O(K) space

    MaxHeap mh(array, k); // Builds a max heap of first k elements: O(K) time
    // Idea: the root holds the k-th smallest element. If we find a new element that is smaller than
    //       the root, then the root is no longer the k-th smallest element, therefore we replace it

    for (int i = k; i < n; i++) {
        if ( array[i] < mh.getMax() )  mh.replaceMax(array[i]);
    }
    return mh.getMax();
}

int kThSmallest_ (int array[], int n, int k) {
    // O(N + KlogN) time, O(N) space
    MinHeap mh (array, n); // Attention, whole array turned into min heap (O(N) time)

    for (int i = 0; i < k - 1; i++) {
        mh.extractMin();
    }
    return mh.getMin();
}

/* =============================Sort a Nearly sorted (k-Sorted) Array ============================*/
// http://www.geeksforgeeks.org/nearly-sorted-algorithm/
// Each element is at most k away from its target position
//

void sortNearlySorted (vector<int>& array, int k) {
    // O(K + (N-K)logK) time,  O(K) space
    // IDEA: Create min-heap of k+1 elements (O(k+1)). Obtain minimum element and insert into array,
    //       add new element into  heap and repeat

    assert (k < array.size());

    int* newarray = new int[k+1];
    for (int i = 0; i < k+1; i++) newarray[i] = array[i];

    MinHeap mh (newarray, k+1);
    int np = 0;
    for (int i = k+1; i < array.size(); i++) {
        int val = mh.extractMin();
        array[np++] = val;
        mh.insertKey(array[i]);
    }

    for (int i = 0; i < k+1; i++)
        array[np++] = mh.extractMin();

    delete newarray;
}

void insertionSort (vector<int>& array) {
    // O(N*k) time, O(1) space
   int i, key, j;
   for (i = 1; i < array.size(); i++) {
       key = array[i];
       j = i-1;

       /* Move elements of A[0..i-1], that are greater than key, to one position ahead of their
          current position. IMPORTANT: This loop will run at most k times !! */
       while (j >= 0 && array[j] > key) {
           array[j+1] = array[j];
           j = j-1;
       }
       array[j+1] = key;
   }
}
void sortNearlySorted_ (vector<int>& array, int k) {
    insertionSort(array);
    k++; // so that compiler does not warn
}

/* ===========================Median in Stream of Numbers=========================================*/
// http://www.geeksforgeeks.org/median-of-stream-of-integers-running-integers/
// http://stackoverflow.com/questions/10657503/find-running-median-from-a-stream-of-integers
// http://karmaandcoding.blogspot.com.es/2012/03/stream-median-in-stream-of-numbers.html
//
// OBJECTIVE: For each element in stream, return median
// Not using Quickselect on N/2 (but can store the data)

// SOLUTION:
//   Use Min and Max heap, max heap to the left storing elements smaller than median, min heap to
//   the right, storing elements bigger than the median.
//   The max heap allows inspecting biggest element that is smaller or equal to the median
//   The min heap allows inpsecting smallest element that is bigger or equal to the median
//   The process has to ensure both heaps have same size.
// PSEUDOCODE:
//   Obtain an element:
//    -If it smaller than the root of the max heap, insert it.
//      - If it is unbalanced, move root from max-heap to min heap
//    - If it is bigger than the root of the max, insert to min-heap.
//      - If min-heap is unbalanced, move root from min-heap to max heap
//    At the end, if both heaps have same #elements, median is average of root elements
//    If max heap has one more element, median is the root of the max heap
//    If min heap has one more element, median is the root of the min heap
//  O(N * logN) time complexity, O(N) space

/* ===========================EXAMPLE FUNCTIONS TO DEMO FUNCTIONS ABOVE===========================*/
void sortAnagrams_example() {
    vector<string> v;
    v.push_back("javi"); v.push_back("is"); v.push_back("ugly"); v.push_back("ulgu");
    v.push_back("ie");   v.push_back("ivaj");
    sortAnagrams(v);

    cout << "Sorting anagrams in array results in : [";
    for (auto& e: v) {
        cout << e << ", ";
    } cout << "]" << endl;
}
void pairsDifferingK_example() {
    vector<int> v = {1,2,3,5,6,7,9,11,12,13};
    cout << "All pairs differing 3 in array are: "; pairsDifferingK(v, 3); cout << endl;
}
void pairsSummingK_example() {
    vector<int> v = {1,1,0,9,8,8,2};
    cout << "All pairs summing 10 in array are: "; pairsSummingK(v, 10);  cout << endl;
    cout << "All pairs summing 10 in array are: "; pairsSummingK_(v, 10); cout << endl;
}
void symmetricPairs_example() {
    vector<pair<int,int> > v= { make_pair(1,0), make_pair(2,3), make_pair(0,1), make_pair(4,5),
                                make_pair(5,4)};
    cout << "Symmetric pairs are : "; symmetricPairs(v);
    cout << endl;
}
void unsortedSubarray_example() {
    vector<int> v = {10, 12, 20, 30, 25, 40, 32, 31, 35, 50, 60};
    //vector<int> v = {0,1,2,3,4,5,6};
    int s = 0;
    int e = -1;
    unsortedSubarray(v, s, e);
    cout << "Unsorted subarray lies between : " << s << " and " << e << endl;
}
void maxOfMin_example() {
    vector<int> v = {10, 20, 30, 50, 10, 70, 30};
    cout << "Maxim value of minimum value for all window sizes are : "; maxOfMin(v);
    cout << endl;
}
void maxSumSubarray_example() {
    vector<int> v1 = {5,7,-3,1,-11,8,12};
    vector<int> v2 = {3,-2,4,-2,-3,5,-6,7,-3,-2,-1,-8};
    int s, e;
    int res = maxSumSubarray(v1, s, e);
    cout << "Max Contiguous Sum is : " << res << " starting at " << s << " ending at " << e << endl;
        res = maxSumSubarray(v2, s, e);
    cout << "Max Contiguous Sum is : " << res << " starting at " << s << " ending at " << e << endl;
}
void findSubarrayGivenSum_Positive_example() {
    vector<int> v = {15, 2, 4, 8, 9, 5, 10, 23};
    int s, e;
    findSubarrayGivenSum_Positive(v, 23, s, e);
    cout << "Subarray with sum 23 is between : " << s << " and " << e << endl;
}
void findSubarrayGivenSum_example() {
    vector<int> v = {1, -3, 4, 8, 2, -14, 3, -1, 10, 6};
    int s, e;
    findSubarrayGivenSum(v, 9, s, e);
    cout << "Subarray with sum 9 is between : " << s << " and " << e << endl;
}
void bigestSubarraySummingZero_example() {
    vector<int> v = {15, -2, 2, -8, 1, 7, 10, 23};
    int s, e;
    bigestSubarraySummingZero (v, s, e);
    cout << "Biggest subarray with sum 0 is between : " << s << " and " << e << endl;
}
void mergeArrays_example() {
    int a[] = {3,4,5,22,40};
    int b[] = {1,2,11,15,20,54,-1,-1,-1,-1,-1};
    int n = sizeof(a)/sizeof(a[0]);
    int m = (sizeof(b)/sizeof(b[0])) - n;
    mergeArrays(a,b,n,m);
    cout << "Merged arrays is: ";
    for (int i = 0; i < n+m; i++) {
        cout << b[i] << " ";
    }
    cout << endl;
}
void mergeInPlace_example() {
    int a[] = {3,4,5,22,40};
    int b[] = {1,2,11,15,20,54};
    mergeInPlace(a,b,5,6);
    cout << "Merged arrays are: ";
    for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++) cout << a[i] << " ";
    cout << " | ";
    for (int i = 0; i < sizeof(b)/sizeof(b[0]); i++) cout << b[i] << " ";
    cout << endl;
}
void searchSortedMatrix_example() {
    int mat[N][N] = { {10, 20, 30, 40},
                      {15, 25, 35, 45},
                      {27, 29, 37, 48},
                      {32, 33, 39, 50},
                    };
    pair<int,int> res = searchSortedMatrix(mat, 29);
    cout << "Found element 29 in matrix position : (" << res.first << ", " << res.second << ")";
    cout << endl;
}
void binarySearch_example() {
    vector<int> v = {1,3,6,233,123,125};
    int res = binarySearch_(v, 0, v.size() - 1, 125);
    cout << "Element 1225 is in position (binary search) : " << res << endl;
    cout << "Element 6 is in position (binary search) : " << binarySearch(v, 6) << endl;
}
void partition_example() {
    vector<int> v = {7, 5, 3, 12, 4, 26, 19};
    int res = partition(v, 0, v.size() - 1);
    cout << "Partitioning [7, 5, 3, 12, 4, 26, 19] returns: position " << res << " and array [ ";
    for (auto& e : v) {
        cout << e << ", ";
    } cout << " ]" << endl;
}
void segregateO1_example() {
    vector<int> v = {1,0,1,1,0,0,1};
    segregateO1(v);
    cout << "Seggregating 0s and 1s for [1,0,1,1,0,0,1] is : [";
    for (auto& e : v) {
        cout << e << ", ";
    } cout << "]" << endl;
}
void segregate012_example() {
    vector<int> v = {0,2,2,1,0,0,1};
    segregate012(v);
    cout << "Seggregating 0s and 1s and 2s for [0,2,2,1,0,0,1] is : [";
    for (auto& e : v) {
        cout << e << ", ";
    } cout << "]" << endl;
}
void quickselect_example() {
    vector<int> v  = {12, 3, 5, 7, 4, 19, 26};
    vector<int> v2 = {12, 3, 5, 7, 4, 19, 26};

    int result; int k = 3;
    bool found = quickSelect(v, 0, v.size() - 1, k, result);
    cout << "K'th smallest element for k = 3 is : " << result << endl;
             k = 1;
         found = quickSelect(v2, 0, v2.size() - 1, v2.size() - 1 - k, result);
    cout << "K'th biggest  element for k = 2 is : " << result << endl;
}
void quicksort_example() {
    vector<int> v  = {12, 3, 5, 7, 4, 19, 26};
    quicksort(v, 0, v.size()-1);
    cout << "QuickSort sorted array is : [";
    for (auto& e: v) {
        cout << e << ", ";
    } cout << "]" << endl;
}
void getMedian_example() {
    int v1[] = {1, 2, 3, 6, 7};
    int v2[] = {4, 6, 8, 10, 9};
    int result, result2;
    bool found = getMedian (v1, v2,  5, result);
         found = getMedian_(v1, v2, 5, result2);
    assert(result == result2);

    cout << "Median of two sorted arrays is : " << result << endl;
}
void rotationCount_example() {
    vector<int> v1 = {10,15,20,0,5};
    vector<int> v2 = {50,5,20,30,40};
    int res = rotationCount(v1,0,4);
        res = min(int(v1.size() - res), res);
    cout << "Rotation count for v1 is: " << res << endl;
    cout << "Rotation count for v2 is: " << rotationCount_(v2) << endl;
}
void minimumInRotated_example() {
    vector<int> v = {10,15,20,0,5};
    cout << "Minimum element in rotated array is: " << minimumInRotated(v,0,4) << endl;
}
void searchRotated_example() {
    vector<int> v = {4, 5, 6, 7, 8, 9, 1, 2, 3};
    cout << "Element 9 is at position in rotated array : " << searchRotated(v, 0, 8, 9) << endl;
}
void fixedPoint_example() {
    vector<int> v= {-10, -1, 0, 3, 10, 11, 30, 50, 100};
    int res = fixedPoint(v, 0, v.size() - 1);
    cout << "Fixed point for array is : " << res << endl;
}
void findMissing_example() {
    vector<int> v1 = {9,1,3,7,2,-2,-3};
    vector<int> v2 = {-2,7,-3,2,9,1};
    cout << "Missing element between two arrays is: " << findMissing(v1, v2) << endl;
}
void findMissingSameOrder_example() {
    vector<int> v1 = {9,1,3,7,2,-2,-3};
    vector<int> v2 = {9,1,3,7,2,-2};
    int pos;
    int res = findMissingSameOrder(v1,v2, pos);
    cout << "Missing element between two arrays is: " << res << " at position " << pos << endl;
}
void kThSmallest_example() {
    int v1[] = {12, 3, 5, 7, 19};
    int v2[] = {12, 3, 5, 7, 19};
    cout << "K'th smallest element in array (using max heap) is: " << kThSmallest(v1, 5, 4) << endl;
    cout << "K'th smallest element in array (using max heap) is: " << kThSmallest(v2, 5, 4) << endl;
}
void sortNearlySorted_example() {
    vector<int> v = {2, 6, 3, 56, 8, 12};
    sortNearlySorted(v, 3);
    cout << "Sorting nearly sorted array (k=3) is : [";
    for (auto& e : v) {
        cout << e << ", ";
    }   cout << "]" << endl;
}

/* ===============================================================================================*/
int main () {
    sortAnagrams_example();
    pairsDifferingK_example();
    pairsSummingK_example();
    symmetricPairs_example();
    unsortedSubarray_example();
    maxOfMin_example();
    maxSumSubarray_example();
    findSubarrayGivenSum_Positive_example();
    findSubarrayGivenSum_example();
    bigestSubarraySummingZero_example();
    mergeArrays_example();
    mergeInPlace_example();
    searchSortedMatrix_example();
    binarySearch_example();
    partition_example();
    segregateO1_example();
    segregate012_example();
    quickselect_example();
    quicksort_example();
    getMedian_example();
    rotationCount_example();
    minimumInRotated_example();
    searchRotated_example();
    fixedPoint_example();
    findMissing_example();
    findMissingSameOrder_example();
    kThSmallest_example();
    sortNearlySorted_example();
}

/* =======================================TODO====================================================*/
// http://quiz.geeksforgeeks.org/heap-sort/
// http://www.geeksforgeeks.org/merge-k-sorted-arrays/

// SEARCH AND SORTING: http://www.geeksforgeeks.org/fundamentals-of-algorithms/#SearchingandSorting
// NICE link: http://www.nayuki.io/page/master-theorem-solver-javascript
//            http://www.cs.unipr.it/purrs/
