"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 7
Topic: Hashing and Hash Table Implementations
"""


# ============================================================
# PART A: Hash Functions
# ============================================================

def hash_division(key, table_size):
    """
    Division Method: h(k) = k mod m
    Choose m as a prime not close to a power of 2.
    """
    return key % table_size


def hash_multiplication(key, table_size):
    """
    Multiplication Method: h(k) = floor(m * (k*A mod 1))
    A = (sqrt(5) - 1) / 2 ≈ 0.6180339887 (Knuth's suggestion)
    """
    A = 0.6180339887
    return int(table_size * ((key * A) % 1))


def hash_string(key, table_size):
    """
    Polynomial rolling hash for strings.
    Uses Horner's method for efficiency.
    h = (c0 * p^(n-1) + c1 * p^(n-2) + ... + c(n-1)) mod m
    """
    p = 31    # Prime base
    h = 0
    power = 1
    for char in key:
        h = (h + ord(char) * power) % table_size
        power = (power * p) % table_size
    return h


def hash_folding(key, table_size, group_size=3):
    """
    Folding Method: Split key into equal parts and sum them.
    """
    key_str = str(key)
    groups = [key_str[i:i + group_size] for i in range(0, len(key_str), group_size)]
    total = sum(int(g) for g in groups)
    return total % table_size


# ============================================================
# PART B: Hash Table with Chaining (Separate Chaining)
# ============================================================

class ChainHashTable:
    """
    Hash Table with Separate Chaining (each slot is a linked list).
    - Average search/insert/delete: O(1 + α) where α = load factor
    - Worst case (all keys hash to same slot): O(n)
    """

    def __init__(self, size=11):
        self.size = size
        self.table = [[] for _ in range(size)]
        self.count = 0

    def _hash(self, key):
        if isinstance(key, str):
            return hash_string(key, self.size)
        return hash_division(key, self.size)

    def insert(self, key, value=None):
        idx = self._hash(key)
        for i, (k, v) in enumerate(self.table[idx]):
            if k == key:
                self.table[idx][i] = (key, value)  # Update existing
                return
        self.table[idx].append((key, value))
        self.count += 1
        print(f"Inserted: [{key}={value}] at slot {idx}")

    def search(self, key):
        idx = self._hash(key)
        for k, v in self.table[idx]:
            if k == key:
                return v
        return None

    def delete(self, key):
        idx = self._hash(key)
        for i, (k, v) in enumerate(self.table[idx]):
            if k == key:
                self.table[idx].pop(i)
                self.count -= 1
                print(f"Deleted: {key} from slot {idx}")
                return True
        raise KeyError(f"Key '{key}' not found.")

    def load_factor(self):
        return self.count / self.size

    def display(self):
        print(f"\nChaining Hash Table (size={self.size}, load={self.load_factor():.2f}):")
        for i, chain in enumerate(self.table):
            if chain:
                items = " -> ".join(f"({k},{v})" for k, v in chain)
                print(f"  [{i:2}]: {items}")
            else:
                print(f"  [{i:2}]: empty")


# ============================================================
# PART C: Hash Table with Open Addressing - Linear Probing
# ============================================================

class LinearProbingHashTable:
    """
    Hash Table with Linear Probing (Open Addressing).
    On collision: h(k, i) = (h(k) + i) mod m
    - Suffers from primary clustering
    - Load factor should be kept below 0.7
    """

    DELETED = "__DELETED__"

    def __init__(self, size=11):
        self.size = size
        self.table = [None] * size
        self.count = 0

    def _hash(self, key):
        if isinstance(key, str):
            return hash_string(key, self.size)
        return hash_division(key, self.size)

    def insert(self, key, value=None):
        if self.load_factor() >= 0.7:
            self._resize()
        idx = self._hash(key)
        for i in range(self.size):
            probe = (idx + i) % self.size
            if self.table[probe] is None or self.table[probe] == self.DELETED:
                self.table[probe] = (key, value)
                self.count += 1
                print(f"Inserted: [{key}={value}] at slot {probe} (probe={i})")
                return
            if self.table[probe][0] == key:
                self.table[probe] = (key, value)  # Update
                return
        raise Exception("Hash table is full.")

    def search(self, key):
        idx = self._hash(key)
        for i in range(self.size):
            probe = (idx + i) % self.size
            if self.table[probe] is None:
                return None
            if self.table[probe] != self.DELETED and self.table[probe][0] == key:
                return self.table[probe][1]
        return None

    def delete(self, key):
        idx = self._hash(key)
        for i in range(self.size):
            probe = (idx + i) % self.size
            if self.table[probe] is None:
                raise KeyError(f"Key '{key}' not found.")
            if self.table[probe] != self.DELETED and self.table[probe][0] == key:
                self.table[probe] = self.DELETED
                self.count -= 1
                print(f"Deleted: {key} from slot {probe}")
                return
        raise KeyError(f"Key '{key}' not found.")

    def load_factor(self):
        return self.count / self.size

    def _resize(self):
        old_table = self.table
        self.size = self.size * 2 + 1
        self.table = [None] * self.size
        self.count = 0
        for entry in old_table:
            if entry and entry != self.DELETED:
                self.insert(entry[0], entry[1])

    def display(self):
        print(f"\nLinear Probing Hash Table (size={self.size}, load={self.load_factor():.2f}):")
        for i, entry in enumerate(self.table):
            if entry is None:
                print(f"  [{i:2}]: empty")
            elif entry == self.DELETED:
                print(f"  [{i:2}]: [DELETED]")
            else:
                print(f"  [{i:2}]: ({entry[0]}, {entry[1]})")


# ============================================================
# PART D: Quadratic Probing
# ============================================================

class QuadraticProbingHashTable:
    """
    Hash Table with Quadratic Probing.
    h(k, i) = (h(k) + i²) mod m
    Reduces primary clustering (but causes secondary clustering).
    Table size must be prime and load factor < 0.5 for guaranteed insertion.
    """

    DELETED = "__DELETED__"

    def __init__(self, size=11):
        self.size = size
        self.table = [None] * size
        self.count = 0

    def _hash(self, key):
        if isinstance(key, str):
            return hash_string(key, self.size)
        return hash_division(key, self.size)

    def insert(self, key, value=None):
        idx = self._hash(key)
        for i in range(self.size):
            probe = (idx + i * i) % self.size
            if self.table[probe] is None or self.table[probe] == self.DELETED:
                self.table[probe] = (key, value)
                self.count += 1
                print(f"Inserted: [{key}={value}] at slot {probe} (probe_seq={i})")
                return
            if self.table[probe][0] == key:
                self.table[probe] = (key, value)
                return
        raise Exception("Hash table full (consider resizing).")

    def search(self, key):
        idx = self._hash(key)
        for i in range(self.size):
            probe = (idx + i * i) % self.size
            if self.table[probe] is None:
                return None
            if self.table[probe] != self.DELETED and self.table[probe][0] == key:
                return self.table[probe][1]
        return None


# ============================================================
# PART E: Double Hashing
# ============================================================

class DoubleHashingTable:
    """
    Double Hashing: Uses two hash functions.
    h(k, i) = (h1(k) + i * h2(k)) mod m
    h2(k) must never equal 0; use h2(k) = 1 + (k mod (m-1))
    Eliminates both primary and secondary clustering.
    """

    def __init__(self, size=11):
        self.size = size
        self.table = [None] * size
        self.count = 0

    def _h1(self, key):
        return key % self.size

    def _h2(self, key):
        return 1 + (key % (self.size - 1))

    def insert(self, key, value=None):
        h1 = self._h1(key)
        h2 = self._h2(key)
        for i in range(self.size):
            probe = (h1 + i * h2) % self.size
            if self.table[probe] is None:
                self.table[probe] = (key, value)
                self.count += 1
                print(f"Inserted: [{key}={value}] at slot {probe}")
                return
        raise Exception("Hash table is full.")

    def search(self, key):
        h1 = self._h1(key)
        h2 = self._h2(key)
        for i in range(self.size):
            probe = (h1 + i * h2) % self.size
            if self.table[probe] is None:
                return None
            if self.table[probe][0] == key:
                return self.table[probe][1]
        return None


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Hashing and Hash Tables")
    print("=" * 60)

    # --- Hash Functions ---
    print("\n--- Hash Function Comparison (key=12345, size=100) ---")
    key, size = 12345, 100
    print(f"  Division Method     : {hash_division(key, size)}")
    print(f"  Multiplication Method: {hash_multiplication(key, size)}")
    print(f"  Folding Method       : {hash_folding(key, size)}")
    print(f"  String Hash 'hello'  : {hash_string('hello', size)}")

    # --- Chaining ---
    print("\n--- Separate Chaining ---")
    cht = ChainHashTable(7)
    data = [('Alice', 25), ('Bob', 30), ('Carol', 22), ('Dave', 28),
            ('Eve', 35), ('Frank', 26), ('Grace', 29)]
    for name, age in data:
        cht.insert(name, age)
    cht.display()
    print(f"\nSearch 'Bob': {cht.search('Bob')}")
    print(f"Search 'Zara': {cht.search('Zara')}")
    cht.delete('Bob')
    print(f"After delete 'Bob': {cht.search('Bob')}")

    # --- Linear Probing ---
    print("\n--- Linear Probing ---")
    lp = LinearProbingHashTable(11)
    for val in [10, 22, 31, 4, 15, 28, 17, 88, 59]:
        lp.insert(val, val * 10)
    lp.display()
    print(f"\nSearch 22: {lp.search(22)}")
    lp.delete(22)
    print(f"Search 22 after delete: {lp.search(22)}")

    # --- Quadratic Probing ---
    print("\n--- Quadratic Probing ---")
    qp = QuadraticProbingHashTable(11)
    for val in [10, 22, 31, 4, 15]:
        qp.insert(val, f"v{val}")

    # --- Double Hashing ---
    print("\n--- Double Hashing ---")
    dh = DoubleHashingTable(11)
    for val in [10, 22, 31, 4, 15, 28]:
        dh.insert(val, f"d{val}")

    # --- Load Factor Analysis ---
    print("\n--- Load Factor & Collision Analysis ---")
    print("Load Factor (α) | Expected comparisons (successful search)")
    print("-" * 55)
    import math
    for alpha in [0.1, 0.25, 0.5, 0.7, 0.9, 0.99]:
        # Expected for open addressing (linear probing): 1/2 * (1 + 1/(1-α))
        if alpha < 1:
            expected_lp = 0.5 * (1 + 1 / (1 - alpha))
            # Expected for chaining: 1 + α/2
            expected_chain = 1 + alpha / 2
            print(f"  α = {alpha:.2f}    | Linear Probing: {expected_lp:5.2f}  | Chaining: {expected_chain:.2f}")

    print("\n" + "=" * 60)
    print("All Hashing operations completed successfully.")
    print("=" * 60)