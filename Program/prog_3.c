"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 3
Topic: Linked List Implementations (Singly, Doubly, Circular)
"""


# ============================================================
# PART A: Singly Linked List
# ============================================================

class SLLNode:
    def __init__(self, data):
        self.data = data
        self.next = None


class SinglyLinkedList:
    """Singly Linked List with full operations."""

    def __init__(self):
        self.head = None
        self._size = 0

    # --- Insertion ---
    def insert_at_beginning(self, data):
        new_node = SLLNode(data)
        new_node.next = self.head
        self.head = new_node
        self._size += 1

    def insert_at_end(self, data):
        new_node = SLLNode(data)
        if self.head is None:
            self.head = new_node
        else:
            current = self.head
            while current.next:
                current = current.next
            current.next = new_node
        self._size += 1

    def insert_at_position(self, data, position):
        """Insert at given 0-indexed position."""
        if position == 0:
            self.insert_at_beginning(data)
            return
        if position > self._size:
            raise IndexError("Position out of range.")
        new_node = SLLNode(data)
        current = self.head
        for _ in range(position - 1):
            current = current.next
        new_node.next = current.next
        current.next = new_node
        self._size += 1

    # --- Deletion ---
    def delete_at_beginning(self):
        if self.head is None:
            raise Exception("List is empty.")
        data = self.head.data
        self.head = self.head.next
        self._size -= 1
        return data

    def delete_at_end(self):
        if self.head is None:
            raise Exception("List is empty.")
        if self.head.next is None:
            data = self.head.data
            self.head = None
            self._size -= 1
            return data
        current = self.head
        while current.next.next:
            current = current.next
        data = current.next.data
        current.next = None
        self._size -= 1
        return data

    def delete_by_value(self, data):
        if self.head is None:
            raise Exception("List is empty.")
        if self.head.data == data:
            self.head = self.head.next
            self._size -= 1
            return
        current = self.head
        while current.next and current.next.data != data:
            current = current.next
        if current.next is None:
            raise ValueError(f"Value {data} not found in list.")
        current.next = current.next.next
        self._size -= 1

    # --- Search ---
    def search(self, data):
        current = self.head
        index = 0
        while current:
            if current.data == data:
                return index
            current = current.next
            index += 1
        return -1

    # --- Utility ---
    def reverse(self):
        """Reverse the linked list in-place. O(n) time, O(1) space."""
        prev = None
        current = self.head
        while current:
            next_node = current.next
            current.next = prev
            prev = current
            current = next_node
        self.head = prev

    def get_middle(self):
        """Find middle node using slow/fast pointer technique."""
        slow = fast = self.head
        while fast and fast.next:
            slow = slow.next
            fast = fast.next.next
        return slow.data if slow else None

    def has_cycle(self):
        """Detect cycle using Floyd's cycle detection algorithm."""
        slow = fast = self.head
        while fast and fast.next:
            slow = slow.next
            fast = fast.next.next
            if slow == fast:
                return True
        return False

    def to_list(self):
        result = []
        current = self.head
        while current:
            result.append(current.data)
            current = current.next
        return result

    def display(self):
        items = self.to_list()
        print("SLL:", " -> ".join(map(str, items)) + " -> NULL")

    def size(self):
        return self._size


# ============================================================
# PART B: Doubly Linked List
# ============================================================

class DLLNode:
    def __init__(self, data):
        self.data = data
        self.prev = None
        self.next = None


class DoublyLinkedList:
    """Doubly Linked List — each node has prev and next pointers."""

    def __init__(self):
        self.head = None
        self.tail = None
        self._size = 0

    def insert_at_end(self, data):
        new_node = DLLNode(data)
        if self.head is None:
            self.head = self.tail = new_node
        else:
            new_node.prev = self.tail
            self.tail.next = new_node
            self.tail = new_node
        self._size += 1

    def insert_at_beginning(self, data):
        new_node = DLLNode(data)
        if self.head is None:
            self.head = self.tail = new_node
        else:
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node
        self._size += 1

    def delete_at_end(self):
        if self.tail is None:
            raise Exception("List is empty.")
        data = self.tail.data
        if self.head == self.tail:
            self.head = self.tail = None
        else:
            self.tail = self.tail.prev
            self.tail.next = None
        self._size -= 1
        return data

    def delete_by_value(self, data):
        current = self.head
        while current:
            if current.data == data:
                if current.prev:
                    current.prev.next = current.next
                else:
                    self.head = current.next
                if current.next:
                    current.next.prev = current.prev
                else:
                    self.tail = current.prev
                self._size -= 1
                return
            current = current.next
        raise ValueError(f"{data} not found.")

    def display_forward(self):
        items = []
        current = self.head
        while current:
            items.append(str(current.data))
            current = current.next
        print("DLL (forward):", " <-> ".join(items))

    def display_backward(self):
        items = []
        current = self.tail
        while current:
            items.append(str(current.data))
            current = current.prev
        print("DLL (backward):", " <-> ".join(items))

    def size(self):
        return self._size


# ============================================================
# PART C: Circular Linked List
# ============================================================

class CircularLinkedList:
    """Circular Singly Linked List — last node points back to head."""

    def __init__(self):
        self.head = None
        self._size = 0

    def insert_at_end(self, data):
        new_node = SLLNode(data)
        if self.head is None:
            self.head = new_node
            new_node.next = self.head
        else:
            current = self.head
            while current.next != self.head:
                current = current.next
            current.next = new_node
            new_node.next = self.head
        self._size += 1

    def delete_by_value(self, data):
        if self.head is None:
            raise Exception("List is empty.")
        if self.head.data == data:
            if self.head.next == self.head:
                self.head = None
            else:
                current = self.head
                while current.next != self.head:
                    current = current.next
                current.next = self.head.next
                self.head = self.head.next
            self._size -= 1
            return
        current = self.head
        while current.next != self.head and current.next.data != data:
            current = current.next
        if current.next == self.head:
            raise ValueError(f"{data} not found.")
        current.next = current.next.next
        self._size -= 1

    def display(self):
        if self.head is None:
            print("Circular List is empty.")
            return
        items = [str(self.head.data)]
        current = self.head.next
        while current != self.head:
            items.append(str(current.data))
            current = current.next
        print("CLL:", " -> ".join(items), "-> (back to head)")

    def size(self):
        return self._size


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Linked List Implementations")
    print("=" * 60)

    # --- Singly Linked List ---
    print("\n--- Singly Linked List ---")
    sll = SinglyLinkedList()
    for val in [10, 20, 30, 40, 50]:
        sll.insert_at_end(val)
    sll.display()

    sll.insert_at_beginning(5)
    sll.insert_at_position(25, 3)
    sll.display()

    print(f"Search 30: index {sll.search(30)}")
    print(f"Middle element: {sll.get_middle()}")
    print(f"Has cycle: {sll.has_cycle()}")

    sll.delete_at_beginning()
    sll.delete_at_end()
    sll.delete_by_value(25)
    sll.display()

    sll.reverse()
    print("After reversal:")
    sll.display()

    # --- Doubly Linked List ---
    print("\n--- Doubly Linked List ---")
    dll = DoublyLinkedList()
    for val in [100, 200, 300, 400]:
        dll.insert_at_end(val)
    dll.display_forward()
    dll.display_backward()

    dll.delete_at_end()
    dll.delete_by_value(200)
    dll.display_forward()

    # --- Circular Linked List ---
    print("\n--- Circular Linked List ---")
    cll = CircularLinkedList()
    for val in ["A", "B", "C", "D"]:
        cll.insert_at_end(val)
    cll.display()
    cll.delete_by_value("B")
    cll.display()

    print("\n" + "=" * 60)
    print("All Linked List operations completed successfully.")
    print("=" * 60)