"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 4
Topic: Binary Search Tree (BST) - Full Implementation
"""


# ============================================================
# BST Node
# ============================================================

class BSTNode:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None


# ============================================================
# Binary Search Tree
# ============================================================

class BinarySearchTree:
    """
    Binary Search Tree with insertion, deletion, search,
    traversals, height, and various utility operations.

    BST Property: For any node n,
        all keys in left subtree  < n.key
        all keys in right subtree > n.key
    """

    def __init__(self):
        self.root = None

    # --------------------------------------------------------
    # Insertion
    # --------------------------------------------------------

    def insert(self, key):
        """Insert key into BST. Time: O(h), Space: O(h) where h = height."""
        self.root = self._insert(self.root, key)
        print(f"Inserted: {key}")

    def _insert(self, node, key):
        if node is None:
            return BSTNode(key)
        if key < node.key:
            node.left = self._insert(node.left, key)
        elif key > node.key:
            node.right = self._insert(node.right, key)
        # Duplicate keys are ignored
        return node

    # --------------------------------------------------------
    # Search
    # --------------------------------------------------------

    def search(self, key):
        """Search for key. Returns node or None. Time: O(h)."""
        return self._search(self.root, key)

    def _search(self, node, key):
        if node is None or node.key == key:
            return node
        if key < node.key:
            return self._search(node.left, key)
        return self._search(node.right, key)

    def contains(self, key):
        return self.search(key) is not None

    # --------------------------------------------------------
    # Deletion
    # --------------------------------------------------------

    def delete(self, key):
        """
        Delete key from BST. Three cases:
          1. Node is a leaf (no children) → simply remove
          2. Node has one child → replace with child
          3. Node has two children → replace with inorder successor
        Time: O(h)
        """
        self.root = self._delete(self.root, key)
        print(f"Deleted: {key}")

    def _delete(self, node, key):
        if node is None:
            raise ValueError(f"Key {key} not found in BST.")
        if key < node.key:
            node.left = self._delete(node.left, key)
        elif key > node.key:
            node.right = self._delete(node.right, key)
        else:
            # Case 1 & 2: No children or one child
            if node.left is None:
                return node.right
            if node.right is None:
                return node.left
            # Case 3: Two children — find inorder successor (min in right subtree)
            successor = self._find_min(node.right)
            node.key = successor.key
            node.right = self._delete(node.right, successor.key)
        return node

    # --------------------------------------------------------
    # Min / Max
    # --------------------------------------------------------

    def _find_min(self, node):
        while node.left:
            node = node.left
        return node

    def _find_max(self, node):
        while node.right:
            node = node.right
        return node

    def find_min(self):
        if self.root is None:
            return None
        return self._find_min(self.root).key

    def find_max(self):
        if self.root is None:
            return None
        return self._find_max(self.root).key

    # --------------------------------------------------------
    # Tree Traversals
    # --------------------------------------------------------

    def inorder(self):
        """Left → Root → Right. Produces sorted output."""
        result = []
        self._inorder(self.root, result)
        return result

    def _inorder(self, node, result):
        if node:
            self._inorder(node.left, result)
            result.append(node.key)
            self._inorder(node.right, result)

    def preorder(self):
        """Root → Left → Right."""
        result = []
        self._preorder(self.root, result)
        return result

    def _preorder(self, node, result):
        if node:
            result.append(node.key)
            self._preorder(node.left, result)
            self._preorder(node.right, result)

    def postorder(self):
        """Left → Right → Root."""
        result = []
        self._postorder(self.root, result)
        return result

    def _postorder(self, node, result):
        if node:
            self._postorder(node.left, result)
            self._postorder(node.right, result)
            result.append(node.key)

    def level_order(self):
        """Breadth-First Traversal (BFS). Uses a queue."""
        if self.root is None:
            return []
        result = []
        queue = [self.root]
        while queue:
            node = queue.pop(0)
            result.append(node.key)
            if node.left:
                queue.append(node.left)
            if node.right:
                queue.append(node.right)
        return result

    # --------------------------------------------------------
    # Tree Properties
    # --------------------------------------------------------

    def height(self):
        """Height = number of edges on the longest path from root to leaf."""
        return self._height(self.root)

    def _height(self, node):
        if node is None:
            return -1
        return 1 + max(self._height(node.left), self._height(node.right))

    def count_nodes(self):
        return self._count(self.root)

    def _count(self, node):
        if node is None:
            return 0
        return 1 + self._count(node.left) + self._count(node.right)

    def is_bst(self):
        """Verify the BST property holds for the entire tree."""
        return self._is_bst(self.root, float('-inf'), float('inf'))

    def _is_bst(self, node, min_val, max_val):
        if node is None:
            return True
        if not (min_val < node.key < max_val):
            return False
        return (self._is_bst(node.left, min_val, node.key) and
                self._is_bst(node.right, node.key, max_val))

    def print_tree(self, node=None, level=0, prefix="Root: "):
        """Print a visual representation of the BST."""
        if node is None and level == 0:
            node = self.root
        if node:
            print(" " * (level * 4) + prefix + str(node.key))
            if node.left or node.right:
                if node.left:
                    self.print_tree(node.left, level + 1, "L--- ")
                else:
                    print(" " * ((level + 1) * 4) + "L--- None")
                if node.right:
                    self.print_tree(node.right, level + 1, "R--- ")
                else:
                    print(" " * ((level + 1) * 4) + "R--- None")


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Binary Search Tree")
    print("=" * 60)

    bst = BinarySearchTree()

    # Insert elements
    print("\n--- Insertions ---")
    keys = [50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45]
    for key in keys:
        bst.insert(key)

    # Visual tree
    print("\n--- Tree Structure ---")
    bst.print_tree()

    # Traversals
    print("\n--- Traversals ---")
    print(f"Inorder   (sorted): {bst.inorder()}")
    print(f"Preorder  (copy)  : {bst.preorder()}")
    print(f"Postorder (delete): {bst.postorder()}")
    print(f"Level Order (BFS) : {bst.level_order()}")

    # Tree properties
    print("\n--- Properties ---")
    print(f"Height    : {bst.height()}")
    print(f"Node count: {bst.count_nodes()}")
    print(f"Min key   : {bst.find_min()}")
    print(f"Max key   : {bst.find_max()}")
    print(f"Is valid BST: {bst.is_bst()}")

    # Search
    print("\n--- Search ---")
    for key in [40, 99]:
        found = bst.contains(key)
        print(f"  Search {key}: {'Found' if found else 'Not Found'}")

    # Deletion (all three cases)
    print("\n--- Deletion ---")
    print("Delete 10 (leaf node):")
    bst.delete(10)
    print(f"  Inorder: {bst.inorder()}")

    print("Delete 30 (node with two children):")
    bst.delete(30)
    print(f"  Inorder: {bst.inorder()}")

    print("Delete 20 (node with one child):")
    bst.delete(20)
    print(f"  Inorder: {bst.inorder()}")

    print("\n--- Final Tree ---")
    bst.print_tree()

    print("\n" + "=" * 60)
    print("All BST operations completed successfully.")
    print("=" * 60)