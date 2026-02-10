"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 1
Topic: Stack Implementation and Applications
"""

# ============================================================
# PART A: Stack using Array (List)
# ============================================================

class StackArray:
    """Stack implemented using Python list (dynamic array)."""

    def __init__(self, capacity=10):
        self.capacity = capacity
        self.stack = []
        self.top = -1

    def push(self, item):
        """Push item onto the stack. Raises exception if overflow."""
        if self.is_full():
            raise OverflowError("Stack Overflow: Cannot push to a full stack.")
        self.stack.append(item)
        self.top += 1
        print(f"Pushed: {item}")

    def pop(self):
        """Remove and return the top item. Raises exception if empty."""
        if self.is_empty():
            raise UnderflowError("Stack Underflow: Cannot pop from an empty stack.")
        self.top -= 1
        return self.stack.pop()

    def peek(self):
        """Return top item without removing it."""
        if self.is_empty():
            raise Exception("Stack is empty.")
        return self.stack[-1]

    def is_empty(self):
        return self.top == -1

    def is_full(self):
        return self.top == self.capacity - 1

    def size(self):
        return self.top + 1

    def display(self):
        if self.is_empty():
            print("Stack is empty.")
        else:
            print("Stack (top -> bottom):", self.stack[::-1])


# ============================================================
# PART B: Stack using Linked List
# ============================================================

class Node:
    def __init__(self, data):
        self.data = data
        self.next = None


class StackLinkedList:
    """Stack implemented using a singly linked list."""

    def __init__(self):
        self.top = None
        self._size = 0

    def push(self, item):
        new_node = Node(item)
        new_node.next = self.top
        self.top = new_node
        self._size += 1
        print(f"Pushed: {item}")

    def pop(self):
        if self.is_empty():
            raise Exception("Stack Underflow")
        popped = self.top.data
        self.top = self.top.next
        self._size -= 1
        return popped

    def peek(self):
        if self.is_empty():
            raise Exception("Stack is empty")
        return self.top.data

    def is_empty(self):
        return self.top is None

    def size(self):
        return self._size

    def display(self):
        if self.is_empty():
            print("Stack is empty.")
            return
        current = self.top
        items = []
        while current:
            items.append(str(current.data))
            current = current.next
        print("Stack (top -> bottom):", " -> ".join(items))


# ============================================================
# PART C: Application - Balanced Parentheses Checker
# ============================================================

def is_balanced(expression):
    """
    Check if parentheses/brackets/braces in expression are balanced.
    Uses a stack-based algorithm.
    Time Complexity: O(n), Space Complexity: O(n)
    """
    stack = StackLinkedList()
    matching = {')': '(', ']': '[', '}': '{'}
    opening = set('([{')
    closing = set(')]}')

    for char in expression:
        if char in opening:
            stack.push(char)
        elif char in closing:
            if stack.is_empty() or stack.peek() != matching[char]:
                return False
            stack.pop()

    return stack.is_empty()


# ============================================================
# PART D: Application - Infix to Postfix Conversion
# ============================================================

def precedence(op):
    """Return operator precedence."""
    if op in ('+', '-'):
        return 1
    if op in ('*', '/'):
        return 2
    if op == '^':
        return 3
    return 0


def infix_to_postfix(expression):
    """
    Convert infix expression to postfix (Reverse Polish Notation).
    Algorithm: Shunting-yard by Edsger Dijkstra
    Time Complexity: O(n), Space Complexity: O(n)
    """
    stack = StackArray()
    postfix = []
    tokens = expression.split()

    for token in tokens:
        if token.isalnum():               # Operand
            postfix.append(token)
        elif token == '(':
            stack.push(token)
        elif token == ')':
            while not stack.is_empty() and stack.peek() != '(':
                postfix.append(stack.pop())
            stack.pop()  # Remove '('
        else:                             # Operator
            while (not stack.is_empty() and
                   precedence(stack.peek()) >= precedence(token)):
                postfix.append(stack.pop())
            stack.push(token)

    while not stack.is_empty():
        postfix.append(stack.pop())

    return ' '.join(postfix)


# ============================================================
# PART E: Application - Evaluate Postfix Expression
# ============================================================

def evaluate_postfix(expression):
    """
    Evaluate a postfix expression and return the result.
    Time Complexity: O(n), Space Complexity: O(n)
    """
    stack = StackArray()
    tokens = expression.split()

    for token in tokens:
        if token.lstrip('-').isdigit():   # Operand (including negatives)
            stack.push(int(token))
        else:
            b = stack.pop()
            a = stack.pop()
            if token == '+':
                stack.push(a + b)
            elif token == '-':
                stack.push(a - b)
            elif token == '*':
                stack.push(a * b)
            elif token == '/':
                if b == 0:
                    raise ZeroDivisionError("Division by zero")
                stack.push(a // b)
            elif token == '^':
                stack.push(a ** b)

    return stack.pop()


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Stack Implementation and Applications")
    print("=" * 60)

    # --- Array Stack Demo ---
    print("\n--- Array-based Stack ---")
    s = StackArray()
    s.push(10)
    s.push(20)
    s.push(30)
    s.display()
    print(f"Peek: {s.peek()}")
    print(f"Popped: {s.pop()}")
    s.display()
    print(f"Size: {s.size()}")

    # --- Linked List Stack Demo ---
    print("\n--- Linked List Stack ---")
    ls = StackLinkedList()
    ls.push("A")
    ls.push("B")
    ls.push("C")
    ls.display()
    print(f"Popped: {ls.pop()}")
    ls.display()

    # --- Balanced Parentheses ---
    print("\n--- Balanced Parentheses Checker ---")
    test_cases = [
        ("{ ( a + b ) * [ c - d ] }", True),
        ("( ( a + b )", False),
        ("{ [ ( ) ] }", True),
        ("( ] )", False),
    ]
    for expr, expected in test_cases:
        result = is_balanced(expr.replace(" ", ""))
        status = "✓" if result == expected else "✗"
        print(f"  {status} '{expr}' -> {'Balanced' if result else 'Not Balanced'}")

    # --- Infix to Postfix ---
    print("\n--- Infix to Postfix Conversion ---")
    infix_expressions = [
        "a + b * c",
        "( a + b ) * c",
        "a + b * c - d / e",
        "( a + b ) * ( c - d )",
    ]
    for expr in infix_expressions:
        postfix = infix_to_postfix(expr)
        print(f"  Infix:   {expr}")
        print(f"  Postfix: {postfix}")
        print()

    # --- Evaluate Postfix ---
    print("--- Evaluate Postfix Expression ---")
    postfix_eval = [
        ("3 4 +", 7),
        ("5 3 - 2 *", 4),
        ("2 3 4 * +", 14),
    ]
    for expr, expected in postfix_eval:
        result = evaluate_postfix(expr)
        status = "✓" if result == expected else "✗"
        print(f"  {status} '{expr}' = {result} (expected {expected})")

    print("\n" + "=" * 60)
    print("All Stack operations completed successfully.")
    print("=" * 60)