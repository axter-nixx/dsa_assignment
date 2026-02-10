"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 8
Topic: Searching Algorithms - Linear, Binary, and Advanced Search
"""

import math
import time
import random


# ============================================================
# PART A: Linear Search (Sequential Search)
# ============================================================

def linear_search(arr, target):
    """
    Linear Search: Scan each element one by one.
    Time: O(n) worst/average, O(1) best (first element)
    Space: O(1)
    Works on: Unsorted AND sorted arrays
    """
    comparisons = 0
    for i, val in enumerate(arr):
        comparisons += 1
        if val == target:
            return i, comparisons
    return -1, comparisons


def linear_search_all(arr, target):
    """Find ALL occurrences of target. Returns list of indices."""
    indices = []
    for i, val in enumerate(arr):
        if val == target:
            indices.append(i)
    return indices


# ============================================================
# PART B: Binary Search (Iterative)
# ============================================================

def binary_search_iterative(arr, target):
    """
    Binary Search (Iterative): Divide the search space in half each step.
    Precondition: Array MUST be sorted.
    Time: O(log n), Space: O(1)
    """
    low, high = 0, len(arr) - 1
    comparisons = 0

    while low <= high:
        mid = (low + high) // 2
        comparisons += 1

        if arr[mid] == target:
            return mid, comparisons
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1

    return -1, comparisons


def binary_search_recursive(arr, target, low=None, high=None, comparisons=0):
    """
    Binary Search (Recursive).
    Time: O(log n), Space: O(log n) recursion stack
    """
    if low is None:
        low = 0
    if high is None:
        high = len(arr) - 1

    if low > high:
        return -1, comparisons

    mid = (low + high) // 2
    comparisons += 1

    if arr[mid] == target:
        return mid, comparisons
    elif arr[mid] < target:
        return binary_search_recursive(arr, target, mid + 1, high, comparisons)
    else:
        return binary_search_recursive(arr, target, low, mid - 1, comparisons)


# ============================================================
# PART C: Binary Search Variants
# ============================================================

def binary_search_first_occurrence(arr, target):
    """Find FIRST occurrence in sorted array with duplicates."""
    low, high = 0, len(arr) - 1
    result = -1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == target:
            result = mid
            high = mid - 1      # Continue left
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    return result


def binary_search_last_occurrence(arr, target):
    """Find LAST occurrence in sorted array with duplicates."""
    low, high = 0, len(arr) - 1
    result = -1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == target:
            result = mid
            low = mid + 1       # Continue right
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    return result


def binary_search_floor(arr, target):
    """
    Floor: Largest element <= target.
    E.g., floor([1,3,5,7,9], 6) = 5 (at index 2)
    """
    low, high = 0, len(arr) - 1
    result = -1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] <= target:
            result = mid
            low = mid + 1
        else:
            high = mid - 1
    return result


def binary_search_ceil(arr, target):
    """
    Ceiling: Smallest element >= target.
    E.g., ceil([1,3,5,7,9], 6) = 7 (at index 3)
    """
    low, high = 0, len(arr) - 1
    result = -1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] >= target:
            result = mid
            high = mid - 1
        else:
            low = mid + 1
    return result


# ============================================================
# PART D: Jump Search
# ============================================================

def jump_search(arr, target):
    """
    Jump Search: Jump ahead by fixed steps, then linear search backward.
    Optimal step size: √n
    Time: O(√n), Space: O(1)
    Requires sorted array.
    """
    n = len(arr)
    step = int(math.sqrt(n))
    prev = 0
    comparisons = 0

    # Jump forward
    while prev < n and arr[min(step, n) - 1] < target:
        comparisons += 1
        prev = step
        step += int(math.sqrt(n))
        if prev >= n:
            return -1, comparisons

    # Linear search in the block
    for i in range(prev, min(step, n)):
        comparisons += 1
        if arr[i] == target:
            return i, comparisons
        if arr[i] > target:
            break

    return -1, comparisons


# ============================================================
# PART E: Interpolation Search
# ============================================================

def interpolation_search(arr, target):
    """
    Interpolation Search: Estimates position based on value distribution.
    Works best for uniformly distributed sorted data.
    Time: O(log log n) average for uniform data, O(n) worst case
    Space: O(1)
    """
    low, high = 0, len(arr) - 1
    comparisons = 0

    while (low <= high and arr[low] <= target <= arr[high]):
        if low == high:
            comparisons += 1
            if arr[low] == target:
                return low, comparisons
            return -1, comparisons

        # Interpolation formula: estimate position
        pos = low + ((target - arr[low]) * (high - low) //
                     (arr[high] - arr[low]))
        comparisons += 1

        if arr[pos] == target:
            return pos, comparisons
        elif arr[pos] < target:
            low = pos + 1
        else:
            high = pos - 1

    return -1, comparisons


# ============================================================
# PART F: Exponential Search
# ============================================================

def exponential_search(arr, target):
    """
    Exponential Search: Find range, then binary search within range.
    Useful for unbounded/infinite arrays.
    Time: O(log n), Space: O(1)
    """
    n = len(arr)
    if arr[0] == target:
        return 0, 1

    comparisons = 0
    i = 1
    while i < n and arr[i] <= target:
        comparisons += 1
        i *= 2

    # Binary search in found range
    idx, bc = binary_search_iterative(arr[i // 2: min(i, n)], target)
    comparisons += bc
    if idx != -1:
        return idx + i // 2, comparisons
    return -1, comparisons


# ============================================================
# PART G: Ternary Search
# ============================================================

def ternary_search(arr, target, low=None, high=None):
    """
    Ternary Search: Divide search space into thirds.
    Time: O(log₃ n) — slightly more comparisons per step than binary search.
    Primarily used for finding min/max of unimodal functions.
    """
    if low is None:
        low = 0
    if high is None:
        high = len(arr) - 1

    if low > high:
        return -1

    mid1 = low + (high - low) // 3
    mid2 = high - (high - low) // 3

    if arr[mid1] == target:
        return mid1
    if arr[mid2] == target:
        return mid2
    if target < arr[mid1]:
        return ternary_search(arr, target, low, mid1 - 1)
    elif target > arr[mid2]:
        return ternary_search(arr, target, mid2 + 1, high)
    else:
        return ternary_search(arr, target, mid1 + 1, mid2 - 1)


# ============================================================
# PART H: Performance Benchmark
# ============================================================

def benchmark_searches(arr, target):
    """Benchmark all search algorithms."""
    methods = [
        ("Linear Search",        lambda: linear_search(arr, target)),
        ("Binary (Iterative)",   lambda: binary_search_iterative(arr, target)),
        ("Binary (Recursive)",   lambda: binary_search_recursive(arr, target)),
        ("Jump Search",          lambda: jump_search(arr, target)),
        ("Interpolation Search", lambda: interpolation_search(arr, target)),
        ("Exponential Search",   lambda: exponential_search(arr, target)),
    ]
    print(f"\n{'Algorithm':<22} {'Index':>6} {'Comparisons':>12} {'Time(μs)':>10}")
    print("-" * 55)
    for name, func in methods:
        start = time.perf_counter()
        idx, comps = func()
        elapsed = (time.perf_counter() - start) * 1e6
        print(f"{name:<22} {idx:>6} {comps:>12} {elapsed:>10.2f}")


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Searching Algorithms")
    print("=" * 60)

    # --- Basic demos ---
    arr_unsorted = [64, 34, 25, 12, 22, 11, 90, 45, 7, 55, 33]
    arr_sorted = sorted(arr_unsorted)
    target = 33

    print(f"\nUnsorted array: {arr_unsorted}")
    print(f"Sorted array  : {arr_sorted}")
    print(f"Target        : {target}\n")

    idx, comps = linear_search(arr_unsorted, target)
    print(f"Linear Search (unsorted): index={idx}, comparisons={comps}")

    idx, comps = binary_search_iterative(arr_sorted, target)
    print(f"Binary Search (sorted)  : index={idx}, comparisons={comps}")

    idx, comps = jump_search(arr_sorted, target)
    print(f"Jump Search   (sorted)  : index={idx}, comparisons={comps}")

    idx, comps = interpolation_search(arr_sorted, target)
    print(f"Interpolation (sorted)  : index={idx}, comparisons={comps}")

    # --- Binary Search Variants ---
    print("\n--- Binary Search Variants ---")
    dup_arr = [1, 2, 4, 4, 4, 5, 7, 9, 9, 11]
    print(f"Array with duplicates: {dup_arr}")
    print(f"First occurrence of 4 : index {binary_search_first_occurrence(dup_arr, 4)}")
    print(f"Last  occurrence of 4 : index {binary_search_last_occurrence(dup_arr, 4)}")
    print(f"All  occurrences of 9 : {linear_search_all(dup_arr, 9)}")

    floor_arr = [1, 3, 5, 7, 9, 11, 13]
    print(f"\nArray: {floor_arr}")
    print(f"Floor of 6 : {floor_arr[binary_search_floor(floor_arr, 6)]} at index {binary_search_floor(floor_arr, 6)}")
    print(f"Ceil  of 6 : {floor_arr[binary_search_ceil(floor_arr, 6)]} at index {binary_search_ceil(floor_arr, 6)}")

    # --- Performance Benchmark ---
    print("\n--- Performance Benchmark (n=10,000) ---")
    large_sorted = list(range(0, 100000, 10))    # 10,000 uniformly spaced elements
    benchmark_searches(large_sorted, 49990)

    # --- Comparison Summary ---
    print("\n--- Algorithm Complexity Summary ---")
    summary = [
        ("Linear Search",     "O(1)",     "O(n)",     "O(n)",     "O(1)", "Any"),
        ("Binary Search",     "O(1)",     "O(log n)", "O(log n)", "O(1)", "Sorted"),
        ("Jump Search",       "O(1)",     "O(√n)",    "O(√n)",    "O(1)", "Sorted"),
        ("Interpolation",     "O(1)",     "O(log log n)","O(n)", "O(1)", "Sorted+Uniform"),
        ("Exponential",       "O(1)",     "O(log n)", "O(log n)", "O(1)", "Sorted"),
        ("Ternary Search",    "O(1)",     "O(log₃ n)","O(log₃ n)","O(log n)","Sorted"),
    ]
    print(f"{'Algorithm':<18} {'Best':>6} {'Average':>14} {'Worst':>10} {'Space':>8} {'Requirement'}")
    print("-" * 75)
    for row in summary:
        print(f"{row[0]:<18} {row[1]:>6} {row[2]:>14} {row[3]:>10} {row[4]:>8} {row[5]}")

    print("\n" + "=" * 60)
    print("All Searching algorithms completed successfully.")
    print("=" * 60)