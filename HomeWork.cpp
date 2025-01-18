#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stack>
#include <functional>

using namespace std;

// Graph representations
const int MAX_NODES = 100;
bool adjMatrix[MAX_NODES][MAX_NODES];
vector<int> adjList[MAX_NODES];
vector<pair<int, int>> adjArray;

// Variables for DFS
int dfs_counter = 0, finish_counter = 0;
vector<int> state(MAX_NODES, 0); // 0: new, 1: active, 2: finished
vector<int> dfs_num(MAX_NODES, -1);
vector<int> fin_num(MAX_NODES, -1);
vector<int> comp(MAX_NODES, -1);
stack<int> Ostack, Rstack;
// Add edge to all representations
void addEdge(int u, int v) {
    adjMatrix[u][v] = true;
    adjList[u].push_back(v);
    adjArray.push_back({u, v});
}



// ====================== 1 ======================
// Initialize
void Init(int nodes) {
    dfs_counter = 1;
    for (int v = 0; v < nodes; ++v) {
        state[v] = 0; // new
        dfs_num[v] = -1;
        comp[v] = -1;
    }
    while (!Ostack.empty()) Ostack.pop();
    while (!Rstack.empty()) Rstack.pop();
}
// DFS
void dfs(int w) {
    state[w] = 1; // active
    dfs_num[w] = dfs_counter++;
    
    // Explore neighbors
    for (int u : adjList[w]) {
        if (state[u] == 0) {
            dfs(u);
        }
    }

    fin_num[w] = finish_counter++;
    state[w] = 2; // finished
}
// Depth First Search
void DepthFirstSearch(int nodes) {
    Init(nodes);
    for (int v = 0; v < nodes; ++v) {
        if (state[v] == 0) {
            dfs(v);
        }
    }
}


// ====================== 2 ======================
// Reverse the edges of the graph
void reverseEdges(int nodes) {
    vector<int> reversedAdjList[MAX_NODES];
    for (int u = 0; u < nodes; ++u) {
        for (int v : adjList[u]) {
            reversedAdjList[v].push_back(u);
        }
    }
    for (int i = 0; i < nodes; ++i) {
        adjList[i] = reversedAdjList[i];
    }
}
// Check if the graph is strongly connected
bool isStronglyConnected(int nodes) {
    // Perform DFS on the original graph
    DepthFirstSearch(nodes);
    for (int i = 0; i < nodes; ++i) {
        if (state[i] == 0) return false;
    }

    // Reverse the graph
    reverseEdges(nodes);

    // Perform DFS on the reversed graph
    Init(nodes);
    DepthFirstSearch(nodes);
    for (int i = 0; i < nodes; ++i) {
        if (state[i] == 0) return false;
    }

    return true;
}


// ====================== 3 ======================
// Cheriyan-Melhorn-Gabow Algorithm
void CheriyanMelhornGabow(int v) {
    state[v] = 1; // active
    dfs_num[v] = dfs_counter++;
    Ostack.push(v);
    Rstack.push(v);

    for (int u : adjList[v]) {
        if (state[u] == 0) {
            CheriyanMelhornGabow(u);
        } else if (state[u] == 1) {
            while (!Rstack.empty() && dfs_num[Rstack.top()] > dfs_num[u]) {
                Rstack.pop();
            }
        }
    }

    if (!Rstack.empty() && Rstack.top() == v) {
        Rstack.pop();
        int w;
        do {
            w = Ostack.top();
            Ostack.pop();
            comp[w] = v;
            state[w] = 2; // finished
        } while (w != v);
    }
}


// ====================== 4 ======================
// Utility function for detecting bridges
bool hasBridgeUtil(int u, bool visited[], int disc[], int low[], int parent[], int nodes) {
    static int time = 0;

    visited[u] = true;
    disc[u] = low[u] = ++time;

    for (int v : adjList[u]) {
        if (!visited[v]) {
            parent[v] = u;
            if (hasBridgeUtil(v, visited, disc, low, parent, nodes))
                return true;

            low[u] = min(low[u], low[v]);

            if (low[v] > disc[u])
                return true;
        } else if (v != parent[u]) {
            low[u] = min(low[u], disc[v]);
        }
    }
    return false;
}
// Function to detect if there are any bridges in the graph
bool hasBridge(int nodes) {
    bool *visited = new bool[nodes];
    int *disc = new int[nodes];
    int *low = new int[nodes];
    int *parent = new int[nodes];

    for (int i = 0; i < nodes; i++) {
        visited[i] = false;
        parent[i] = -1;
    }

    for (int i = 0; i < nodes; i++) {
        if (!visited[i]) {
            if (hasBridgeUtil(i, visited, disc, low, parent, nodes)) {
                delete[] visited;
                delete[] disc;
                delete[] low;
                delete[] parent;
                return true;
            }
        }
    }

    delete[] visited;
    delete[] disc;
    delete[] low;
    delete[] parent;

    return false;
}


// ===============================================
// Initialize the graph and reset variables
void initGraph(int nodes) {
    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            adjMatrix[i][j] = false;
        }
        adjList[i].clear();
    }
    adjArray.clear();
    dfs_counter = 0;
    finish_counter = 0;
    fill(state.begin(), state.end(), 0);
    fill(dfs_num.begin(), dfs_num.end(), -1);
    fill(fin_num.begin(), fin_num.end(), -1);
    fill(comp.begin(), comp.end(), -1);
    while (!Ostack.empty()) Ostack.pop();
    while (!Rstack.empty()) Rstack.pop();
}
// Create a random graph for testing
void createRandomGraph(int nodes, int edges) {
    srand(time(0));
    initGraph(nodes);

    for (int i = 0; i < edges; ++i) {
        int u = rand() % nodes;
        int v = rand() % nodes;
        addEdge(u, v);
    }
}
// Print graph representations
void printGraph(int nodes) {
    cout << "Adjacency Matrix:\n";
    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nAdjacency List:\n";
    for (int i = 0; i < nodes; ++i) {
        cout << i << ": ";
        for (int v : adjList[i]) {
            cout << v << " ";
        }
        cout << endl;
    }

    cout << "\nAdjacency Array:\n";
    for (auto edge : adjArray) {
        cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    cout << endl;
}

int main() {

        int nodes = 5, edges = 10;
    createRandomGraph(nodes, edges);

    printGraph(nodes);

    DepthFirstSearch(nodes);

    if (isStronglyConnected(nodes)) {
        cout << "\n\nThe graph is strongly connected.\n";
    } else {
        cout << "\n\nThe graph is not strongly connected.\n";
    }
    return 0;
}
