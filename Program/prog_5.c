"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 5
Topic: Sorting Algorithms - Comparison and Implementation
"""

import time
import random


# ============================================================
# PART A: Bubble Sort
# ============================================================

def bubble_sort(arr):
    """
    Bubble Sort: Repeatedly compare and swap adjacent elements.
    Time: O(n²) worst/avg, O(n) best (already sorted with flag)
    Space: O(1) — in-place
    Stable: Yes
    """
    arr = arr.copy()
    n = len(arr)
    comparisons = swaps = 0

    for i in range(n - 1):
        swapped = False
        for j in range(n - 1 - i):
            comparisons += 1
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                swaps += 1
                swapped = True
        if not swapped:
            break  # Early exit: array is already sorted

    return arr, comparisons, swaps


# ============================================================
# PART B: Selection Sort
# ============================================================

def selection_sort(arr):
    """
    Selection Sort: Find the minimum element and place it at the front.
    Time: O(n²) all cases
    Space: O(1) — in-place
    Stable: No (can displace equal elements)
    """
    arr = arr.copy()
    n = len(arr)
    comparisons = swaps = 0

    for i in range(n - 1):
        min_idx = i
        for j in range(i + 1, n):
            comparisons += 1
            if arr[j] < arr[min_idx]:
                min_idx = j
        if min_idx != i:
            arr[i], arr[min_idx] = arr[min_idx], arr[i]
            swaps += 1

    return arr, comparisons, swaps


# ============================================================
# PART C: Insertion Sort
# ============================================================

def insertion_sort(arr):
    """
    Insertion Sort: Build sorted array one element at a time.
    Time: O(n²) worst/avg, O(n) best (nearly sorted)
    Space: O(1) — in-place
    Stable: Yes
    Adaptive: Yes (efficient for nearly-sorted data)
    """
    arr = arr.copy()
    n = len(arr)
    comparisons = shifts = 0

    for i in range(1, n):
        key = arr[i]
        j = i - 1
        while j >= 0 and arr[j] > key:
            comparisons += 1
            arr[j + 1] = arr[j]
            shifts += 1
            j -= 1
        if j >= 0:
            comparisons += 1  # The comparison that stopped the loop
        arr[j + 1] = key

    return arr, comparisons, shifts


# ============================================================
# PART D: Merge Sort
# ============================================================

merge_comparisons = 0

def merge_sort(arr):
    """
    Merge Sort: Divide and conquer — split, sort, merge.
    Time: O(n log n) all cases
    Space: O(n) — not in-place
    Stable: Yes
    """
    global merge_comparisons
    merge_comparisons = 0
    result = _merge_sort(arr.copy())
    return result, merge_comparisons

def _merge_sort(arr):
    global merge_comparisons
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = _merge_sort(arr[:mid])
    right = _merge_sort(arr[mid:])
    return _merge(left, right)

def _merge(left, right):
    global merge_comparisons
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        merge_comparisons += 1
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result


# ============================================================
# PART E: Quick Sort
# ============================================================

quick_comparisons = 0

def quick_sort(arr):
    """
    Quick Sort: Divide and conquer using a pivot element.
    Time: O(n log n) avg, O(n²) worst (poor pivot choice)
    Space: O(log n) avg recursion stack
    Stable: No
    In-place: Yes
    """
    global quick_comparisons
    quick_comparisons = 0
    arr = arr.copy()
    _quick_sort(arr, 0, len(arr) - 1)
    return arr, quick_comparisons

def _quick_sort(arr, low, high):
    if low < high:
        pi = _partition(arr, low, high)
        _quick_sort(arr, low, pi - 1)
        _quick_sort(arr, pi + 1, high)

def _partition(arr, low, high):
    global quick_comparisons
    pivot = arr[high]   # Pivot is last element
    i = low - 1
    for j in range(low, high):
        quick_comparisons += 1
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1


# ============================================================
# PART F: Heap Sort
# ============================================================

heap_comparisons = 0

def heap_sort(arr):
    """
    Heap Sort: Build a max-heap, then extract elements in order.
    Time: O(n log n) all cases
    Space: O(1) — in-place
    Stable: No
    """
    global heap_comparisons
    heap_comparisons = 0
    arr = arr.copy()
    n = len(arr)

    # Build max-heap
    for i in range(n // 2 - 1, -1, -1):
        _heapify(arr, n, i)

    # Extract elements one by one
    for i in range(n - 1, 0, -1):
        arr[0], arr[i] = arr[i], arr[0]
        _heapify(arr, i, 0)

    return arr, heap_comparisons

def _heapify(arr, n, i):
    global heap_comparisons
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2
    if left < n:
        heap_comparisons += 1
        if arr[left] > arr[largest]:
            largest = left
    if right < n:
        heap_comparisons += 1
        if arr[right] > arr[largest]:
            largest = right
    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        _heapify(arr, n, largest)


# ============================================================
# PART G: Counting Sort (non-comparison based)
# ============================================================

def counting_sort(arr, max_val=None):
    """
    Counting Sort: Count occurrences of each element.
    Time: O(n + k) where k = range of input
    Space: O(k)
    Stable: Yes
    Limitation: Only works with non-negative integers
    """
    if not arr:
        return arr
    if max_val is None:
        max_val = max(arr)
    count = [0] * (max_val + 1)
    for val in arr:
        count[val] += 1
    result = []
    for i, c in enumerate(count):
        result.extend([i] * c)
    return result


# ============================================================
# PART H: Performance Comparison
# ============================================================

def benchmark_sorts(data):
    """Compare all sorting algorithms on the same dataset."""
    algorithms = [
        ("Bubble Sort", bubble_sort),
        ("Selection Sort", selection_sort),
        ("Insertion Sort", insertion_sort),
        ("Merge Sort", merge_sort),
        ("Quick Sort", quick_sort),
        ("Heap Sort", heap_sort),
    ]
    print(f"\n{'Algorithm':<18} {'Time (ms)':>10} {'Comparisons':>13}")
    print("-" * 44)
    for name, func in algorithms:
        start = time.perf_counter()
        result, comps = func(data)[:2]
        elapsed = (time.perf_counter() - start) * 1000
        print(f"{name:<18} {elapsed:>10.3f} {comps:>13,}")


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Sorting Algorithms")
    print("=" * 60)

    # Small demonstration array
    arr = [64, 34, 25, 12, 22, 11, 90, 45, 7, 55]
    print(f"\nOriginal array: {arr}")
    print()

    print(f"Bubble Sort    : {bubble_sort(arr)[0]}")
    print(f"Selection Sort : {selection_sort(arr)[0]}")
    print(f"Insertion Sort : {insertion_sort(arr)[0]}")
    print(f"Merge Sort     : {merge_sort(arr)[0]}")
    print(f"Quick Sort     : {quick_sort(arr)[0]}")
    print(f"Heap Sort      : {heap_sort(arr)[0]}")
    print(f"Counting Sort  : {counting_sort(arr)}")

    # Stability test
    print("\n--- Stability Demonstration ---")
    pairs = [(3, 'a'), (1, 'b'), (2, 'c'), (1, 'd'), (3, 'e')]
    print(f"Input : {pairs}")
    # Merge sort is stable (preserves relative order of equal elements)
    stable_result, _ = merge_sort([p[0] for p in pairs])
    print(f"After stable merge sort on keys: {stable_result}")
    print("(1,b) must appear before (1,d) in stable sort")

    # Performance benchmark
    print("\n--- Performance Benchmark (n=1000) ---")
    random_data = random.sample(range(1, 10001), 1000)
    benchmark_sorts(random_data)

    # Complexity summary
    print("\n--- Time and Space Complexity Summary ---")
    summary = [
        ("Bubble Sort",    "O(n²)",     "O(n²)",     "O(n)",  "O(1)",     "Yes"),
        ("Selection Sort", "O(n²)",     "O(n²)",     "O(n²)", "O(1)",     "No"),
        ("Insertion Sort", "O(n)",      "O(n²)",     "O(n²)", "O(1)",     "Yes"),
        ("Merge Sort",     "O(n log n)","O(n log n)","O(n log n)","O(n)", "Yes"),
        ("Quick Sort",     "O(n log n)","O(n log n)","O(n²)", "O(log n)", "No"),
        ("Heap Sort",      "O(n log n)","O(n log n)","O(n log n)","O(1)", "No"),
    ]
    print(f"{'Algorithm':<16} {'Best':>10} {'Avg':>12} {'Worst':>12} {'Space':>8} {'Stable':>7}")
    print("-" * 70)
    for row in summary:
        print(f"{row[0]:<16} {row[1]:>10} {row[2]:>12} {row[3]:>12} {row[4]:>8} {row[5]:>7}")

    print("\n" + "=" * 60)
    print("All sorting algorithms executed successfully.")
    print("=" * 60)