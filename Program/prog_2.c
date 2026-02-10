"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 2
Topic: Queue Implementation and Applications
"""

# ============================================================
# PART A: Linear Queue using Array
# ============================================================

class LinearQueue:
    """Linear Queue implemented using a fixed-size array."""

    def __init__(self, capacity=10):
        self.capacity = capacity
        self.queue = [None] * capacity
        self.front = -1
        self.rear = -1

    def enqueue(self, item):
        """Add item to rear of queue."""
        if self.is_full():
            raise OverflowError("Queue Overflow: Queue is full.")
        if self.front == -1:
            self.front = 0
        self.rear += 1
        self.queue[self.rear] = item
        print(f"Enqueued: {item}")

    def dequeue(self):
        """Remove and return item from front of queue."""
        if self.is_empty():
            raise Exception("Queue Underflow: Queue is empty.")
        item = self.queue[self.front]
        self.queue[self.front] = None
        if self.front == self.rear:
            self.front = self.rear = -1  # Reset when last element removed
        else:
            self.front += 1
        return item

    def peek(self):
        if self.is_empty():
            raise Exception("Queue is empty.")
        return self.queue[self.front]

    def is_empty(self):
        return self.front == -1

    def is_full(self):
        return self.rear == self.capacity - 1

    def size(self):
        if self.is_empty():
            return 0
        return self.rear - self.front + 1

    def display(self):
        if self.is_empty():
            print("Queue is empty.")
        else:
            items = [str(x) for x in self.queue[self.front:self.rear + 1]]
            print(f"Queue (front -> rear): [{', '.join(items)}]")


# ============================================================
# PART B: Circular Queue using Array
# ============================================================

class CircularQueue:
    """
    Circular Queue (Ring Buffer) — solves the wasted space
    problem of linear queue after multiple dequeue operations.
    """

    def __init__(self, capacity=10):
        self.capacity = capacity
        self.queue = [None] * capacity
        self.front = -1
        self.rear = -1
        self._size = 0

    def enqueue(self, item):
        if self.is_full():
            raise OverflowError("Circular Queue Overflow.")
        if self.front == -1:
            self.front = 0
        self.rear = (self.rear + 1) % self.capacity
        self.queue[self.rear] = item
        self._size += 1
        print(f"Enqueued: {item}")

    def dequeue(self):
        if self.is_empty():
            raise Exception("Circular Queue Underflow.")
        item = self.queue[self.front]
        self.queue[self.front] = None
        if self.front == self.rear:
            self.front = self.rear = -1
        else:
            self.front = (self.front + 1) % self.capacity
        self._size -= 1
        return item

    def peek(self):
        if self.is_empty():
            raise Exception("Queue is empty.")
        return self.queue[self.front]

    def is_empty(self):
        return self.front == -1

    def is_full(self):
        return self._size == self.capacity

    def size(self):
        return self._size

    def display(self):
        if self.is_empty():
            print("Circular Queue is empty.")
            return
        items = []
        i = self.front
        count = 0
        while count < self._size:
            items.append(str(self.queue[i]))
            i = (i + 1) % self.capacity
            count += 1
        print(f"Circular Queue (front -> rear): [{', '.join(items)}]")


# ============================================================
# PART C: Queue using Two Stacks
# ============================================================

class QueueUsingTwoStacks:
    """
    Implement a Queue using two stacks.
    - Enqueue: O(1)  — push to stack1
    - Dequeue: Amortized O(1) — transfer stack1 to stack2 when stack2 is empty
    """

    def __init__(self):
        self.stack1 = []   # For enqueue
        self.stack2 = []   # For dequeue

    def enqueue(self, item):
        self.stack1.append(item)
        print(f"Enqueued: {item}")

    def dequeue(self):
        if not self.stack2:
            if not self.stack1:
                raise Exception("Queue is empty.")
            while self.stack1:
                self.stack2.append(self.stack1.pop())
        return self.stack2.pop()

    def peek(self):
        if not self.stack2:
            if not self.stack1:
                raise Exception("Queue is empty.")
            while self.stack1:
                self.stack2.append(self.stack1.pop())
        return self.stack2[-1]

    def is_empty(self):
        return not self.stack1 and not self.stack2

    def size(self):
        return len(self.stack1) + len(self.stack2)


# ============================================================
# PART D: Priority Queue
# ============================================================

class PriorityQueue:
    """
    Min-Priority Queue (smallest priority value served first).
    Implemented using a sorted list for simplicity.
    For better performance, a min-heap (O(log n)) should be used.
    """

    def __init__(self):
        self.items = []  # List of (priority, item) tuples

    def enqueue(self, item, priority):
        self.items.append((priority, item))
        self.items.sort(key=lambda x: x[0])   # Keep sorted by priority
        print(f"Enqueued: {item} (priority={priority})")

    def dequeue(self):
        if self.is_empty():
            raise Exception("Priority Queue is empty.")
        priority, item = self.items.pop(0)
        print(f"Dequeued: {item} (priority={priority})")
        return item

    def peek(self):
        if self.is_empty():
            raise Exception("Priority Queue is empty.")
        return self.items[0][1]

    def is_empty(self):
        return len(self.items) == 0

    def size(self):
        return len(self.items)

    def display(self):
        if self.is_empty():
            print("Priority Queue is empty.")
        else:
            print("Priority Queue:", [(p, v) for p, v in self.items])


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Queue Implementation and Applications")
    print("=" * 60)

    # --- Linear Queue ---
    print("\n--- Linear Queue ---")
    lq = LinearQueue(5)
    lq.enqueue(10)
    lq.enqueue(20)
    lq.enqueue(30)
    lq.display()
    print(f"Dequeued: {lq.dequeue()}")
    lq.display()
    print(f"Front: {lq.peek()}, Size: {lq.size()}")

    # --- Circular Queue ---
    print("\n--- Circular Queue ---")
    cq = CircularQueue(5)
    for val in [1, 2, 3, 4, 5]:
        cq.enqueue(val)
    cq.display()
    cq.dequeue()
    cq.dequeue()
    cq.display()
    cq.enqueue(6)       # Reuse freed space
    cq.enqueue(7)
    cq.display()

    # --- Queue Using Two Stacks ---
    print("\n--- Queue Using Two Stacks ---")
    qs = QueueUsingTwoStacks()
    qs.enqueue("A")
    qs.enqueue("B")
    qs.enqueue("C")
    print(f"Dequeued: {qs.dequeue()}")
    print(f"Dequeued: {qs.dequeue()}")
    qs.enqueue("D")
    print(f"Peek: {qs.peek()}")
    print(f"Dequeued: {qs.dequeue()}")
    print(f"Dequeued: {qs.dequeue()}")

    # --- Priority Queue ---
    print("\n--- Priority Queue ---")
    pq = PriorityQueue()
    pq.enqueue("Emergency Surgery", 1)
    pq.enqueue("Routine Checkup", 5)
    pq.enqueue("Urgent Care", 2)
    pq.enqueue("Follow-up Visit", 4)
    pq.display()
    print("\nProcessing tasks in priority order:")
    while not pq.is_empty():
        pq.dequeue()

    print("\n" + "=" * 60)
    print("All Queue operations completed successfully.")
    print("=" * 60)