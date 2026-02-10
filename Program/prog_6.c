"""
COMP202 - Data Structures and Algorithms
Assignment #3 Theory - Program 6
Topic: Graph Representations and Traversal Algorithms
"""

from collections import defaultdict, deque
import heapq


# ============================================================
# PART A: Graph Representation - Adjacency List
# ============================================================

class Graph:
    """
    Graph using Adjacency List representation.
    Supports both directed and undirected, weighted and unweighted graphs.
    """

    def __init__(self, directed=False, weighted=False):
        self.directed = directed
        self.weighted = weighted
        self.adj = defaultdict(list)   # vertex -> list of (neighbor, weight)
        self.vertices = set()

    def add_vertex(self, v):
        self.vertices.add(v)
        if v not in self.adj:
            self.adj[v] = []

    def add_edge(self, u, v, weight=1):
        """Add edge u → v (and v → u if undirected)."""
        self.add_vertex(u)
        self.add_vertex(v)
        self.adj[u].append((v, weight))
        if not self.directed:
            self.adj[v].append((u, weight))

    def get_neighbors(self, v):
        return self.adj[v]

    def display(self):
        print("Graph (Adjacency List):")
        for vertex in sorted(self.vertices):
            neighbors = [(n, w) if self.weighted else n for n, w in self.adj[vertex]]
            print(f"  {vertex}: {neighbors}")

    def num_vertices(self):
        return len(self.vertices)

    def num_edges(self):
        count = sum(len(neighbors) for neighbors in self.adj.values())
        return count if self.directed else count // 2


# ============================================================
# PART B: Adjacency Matrix Representation
# ============================================================

class GraphMatrix:
    """Graph using Adjacency Matrix representation."""

    def __init__(self, num_vertices, directed=False):
        self.n = num_vertices
        self.directed = directed
        self.matrix = [[0] * num_vertices for _ in range(num_vertices)]

    def add_edge(self, u, v, weight=1):
        self.matrix[u][v] = weight
        if not self.directed:
            self.matrix[v][u] = weight

    def remove_edge(self, u, v):
        self.matrix[u][v] = 0
        if not self.directed:
            self.matrix[v][u] = 0

    def has_edge(self, u, v):
        return self.matrix[u][v] != 0

    def display(self):
        print("Graph (Adjacency Matrix):")
        print("  ", "  ".join(str(i) for i in range(self.n)))
        for i, row in enumerate(self.matrix):
            print(f"  {i} {row}")


# ============================================================
# PART C: DFS - Depth First Search
# ============================================================

def dfs_recursive(graph, start):
    """
    DFS using recursion.
    Time: O(V + E), Space: O(V) for visited set + recursion stack
    """
    visited = []
    seen = set()

    def _dfs(v):
        seen.add(v)
        visited.append(v)
        for neighbor, _ in sorted(graph.get_neighbors(v)):
            if neighbor not in seen:
                _dfs(neighbor)

    _dfs(start)
    return visited


def dfs_iterative(graph, start):
    """
    DFS using explicit stack (iterative version).
    Time: O(V + E), Space: O(V)
    """
    visited = []
    seen = set()
    stack = [start]

    while stack:
        v = stack.pop()
        if v not in seen:
            seen.add(v)
            visited.append(v)
            for neighbor, _ in sorted(graph.get_neighbors(v), reverse=True):
                if neighbor not in seen:
                    stack.append(neighbor)

    return visited


# ============================================================
# PART D: BFS - Breadth First Search
# ============================================================

def bfs(graph, start):
    """
    BFS using a queue.
    Visits all vertices at distance d before vertices at d+1.
    Time: O(V + E), Space: O(V)
    """
    visited = []
    seen = set([start])
    queue = deque([start])

    while queue:
        v = queue.popleft()
        visited.append(v)
        for neighbor, _ in sorted(graph.get_neighbors(v)):
            if neighbor not in seen:
                seen.add(neighbor)
                queue.append(neighbor)

    return visited


def bfs_shortest_path(graph, start, end):
    """
    Find shortest path (by hop count, unweighted) using BFS.
    Returns path as a list of vertices, or None if unreachable.
    """
    if start == end:
        return [start]
    parent = {start: None}
    queue = deque([start])

    while queue:
        v = queue.popleft()
        for neighbor, _ in graph.get_neighbors(v):
            if neighbor not in parent:
                parent[neighbor] = v
                if neighbor == end:
                    # Reconstruct path
                    path = []
                    node = end
                    while node is not None:
                        path.append(node)
                        node = parent[node]
                    return path[::-1]
                queue.append(neighbor)

    return None  # No path found


# ============================================================
# PART E: Dijkstra's Shortest Path (Weighted)
# ============================================================

def dijkstra(graph, source):
    """
    Dijkstra's Algorithm: Finds shortest paths from source to all vertices.
    Uses a min-heap (priority queue) for efficiency.
    Time: O((V + E) log V) with binary heap
    Space: O(V)
    Limitation: Does NOT work with negative edge weights.
    """
    # Initialize distances to infinity
    dist = {v: float('inf') for v in graph.vertices}
    dist[source] = 0
    parent = {v: None for v in graph.vertices}

    # Min-heap: (distance, vertex)
    heap = [(0, source)]

    visited = set()

    while heap:
        d, u = heapq.heappop(heap)

        if u in visited:
            continue
        visited.add(u)

        for v, weight in graph.get_neighbors(u):
            if dist[u] + weight < dist[v]:
                dist[v] = dist[u] + weight
                parent[v] = u
                heapq.heappush(heap, (dist[v], v))

    return dist, parent


def get_shortest_path(parent, source, target):
    """Reconstruct shortest path from Dijkstra's parent map."""
    path = []
    node = target
    while node is not None:
        path.append(node)
        node = parent[node]
    path.reverse()
    if path[0] == source:
        return path
    return None  # No path


# ============================================================
# PART F: Cycle Detection
# ============================================================

def has_cycle_undirected(graph):
    """
    Detect cycle in undirected graph using DFS.
    Time: O(V + E)
    """
    visited = set()

    def _dfs(v, parent):
        visited.add(v)
        for neighbor, _ in graph.get_neighbors(v):
            if neighbor not in visited:
                if _dfs(neighbor, v):
                    return True
            elif neighbor != parent:
                return True
        return False

    for v in graph.vertices:
        if v not in visited:
            if _dfs(v, None):
                return True
    return False


def topological_sort(graph):
    """
    Topological sort of a DAG (Directed Acyclic Graph).
    Uses DFS with a finish-time stack.
    Time: O(V + E)
    """
    visited = set()
    stack = []

    def _dfs(v):
        visited.add(v)
        for neighbor, _ in graph.get_neighbors(v):
            if neighbor not in visited:
                _dfs(neighbor)
        stack.append(v)

    for v in graph.vertices:
        if v not in visited:
            _dfs(v)

    return stack[::-1]


# ============================================================
# MAIN: Demonstration
# ============================================================

if __name__ == "__main__":
    print("=" * 60)
    print("COMP202 - Graph Algorithms")
    print("=" * 60)

    # --- Build undirected graph ---
    print("\n--- Undirected Graph ---")
    g = Graph(directed=False, weighted=False)
    edges = [(0, 1), (0, 2), (1, 3), (1, 4), (2, 4), (3, 5), (4, 5)]
    for u, v in edges:
        g.add_edge(u, v)
    g.display()
    print(f"Vertices: {g.num_vertices()}, Edges: {g.num_edges()}")

    # Traversals
    print(f"\nDFS (recursive) from 0: {dfs_recursive(g, 0)}")
    print(f"DFS (iterative) from 0: {dfs_iterative(g, 0)}")
    print(f"BFS              from 0: {bfs(g, 0)}")

    # Shortest path (unweighted)
    path = bfs_shortest_path(g, 0, 5)
    print(f"Shortest path 0 -> 5  : {path} (hops: {len(path)-1})")

    # Cycle detection
    print(f"\nHas cycle: {has_cycle_undirected(g)}")

    # Adjacency Matrix
    print("\n--- Adjacency Matrix ---")
    gm = GraphMatrix(4)
    for u, v in [(0, 1), (0, 2), (1, 3), (2, 3)]:
        gm.add_edge(u, v)
    gm.display()

    # --- Dijkstra on weighted graph ---
    print("\n--- Dijkstra's Shortest Path (Weighted) ---")
    wg = Graph(directed=False, weighted=True)
    weighted_edges = [
        ('A', 'B', 4), ('A', 'C', 2), ('B', 'C', 5),
        ('B', 'D', 10), ('C', 'E', 3), ('D', 'F', 11),
        ('E', 'D', 4), ('E', 'F', 6)
    ]
    for u, v, w in weighted_edges:
        wg.add_edge(u, v, w)
    wg.display()

    dist, parent = dijkstra(wg, 'A')
    print(f"\nShortest distances from A:")
    for v in sorted(dist):
        path = get_shortest_path(parent, 'A', v)
        print(f"  A -> {v}: {dist[v]:3}  path: {' -> '.join(map(str, path))}")

    # --- Topological Sort ---
    print("\n--- Topological Sort (DAG) ---")
    dag = Graph(directed=True)
    dag_edges = [
        ('A', 'C'), ('B', 'C'), ('B', 'D'),
        ('C', 'E'), ('D', 'F'), ('E', 'F')
    ]
    for u, v in dag_edges:
        dag.add_edge(u, v)
    dag.display()
    topo = topological_sort(dag)
    print(f"Topological Order: {topo}")

    print("\n" + "=" * 60)
    print("All Graph algorithms completed successfully.")
    print("=" * 60)