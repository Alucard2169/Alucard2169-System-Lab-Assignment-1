#include <bits/stdc++.h>
using namespace std;

/**
 * Konigsberg Bridge Problem with Euler’s condition
 * ------------------------------------------------
 * Vertices (landmasses):
 *   0 = North Bank
 *   1 = South Bank
 *   2 = Kneiphof Island
 *   3 = East Island
 *
 * Bridges are undirected edges. We check degrees and determine:
 *   - Eulerian circuit (0 odd degrees)
 *   - Eulerian path (2 odd degrees)
 *   - Impossible (>2 odd degrees)
 */

struct Edge {
    int to;
    int id;
    bool used;
};

class Graph {
    int V;
    vector<vector<Edge>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addBridge(int u, int v, int id) {
        adj[u].push_back({v, id, false});
        adj[v].push_back({u, id, false});
    }

    vector<int> getDegrees() {
        vector<int> deg(V);
        for (int i = 0; i < V; i++) deg[i] = adj[i].size();
        return deg;
    }

    // Count odd degree vertices
    vector<int> getOddVertices() {
        vector<int> odd;
        for (int i = 0; i < V; i++) {
            if (adj[i].size() % 2 != 0) odd.push_back(i);
        }
        return odd;
    }

    // Hierholzer’s algorithm for Eulerian path/circuit
    vector<int> eulerianTrail(int start) {
        vector<int> path;
        stack<int> st;
        st.push(start);

        while (!st.empty()) {
            int u = st.top();
            bool found = false;
            for (auto &e : adj[u]) {
                if (!e.used) {
                    e.used = true;
                    // mark reverse edge used
                    for (auto &rev : adj[e.to]) {
                        if (rev.id == e.id && !rev.used) {
                            rev.used = true;
                            break;
                        }
                    }
                    st.push(e.to);
                    found = true;
                    break;
                }
            }
            if (!found) {
                path.push_back(u);
                st.pop();
            }
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    // Landmasses: 0=N, 1=S, 2=K, 3=E
    Graph g(4);
    int id = 0;

    // Original 7 bridges of Konigsberg
    g.addBridge(0,2,id++); // N-K
    g.addBridge(0,2,id++); // N-K
    g.addBridge(0,3,id++); // N-E
    g.addBridge(0,1,id++); // N-S
    g.addBridge(2,3,id++); // K-E
    g.addBridge(2,1,id++); // K-S
    g.addBridge(2,1,id++); // K-S

    // g.addBridge(0,1,id++); // extra bridge N-S
    // g.addBridge(2,3,id++); // extra bridge K-E
    // g.addBridge(0,3,id++); // extra bridge N-E


    vector<string> names = {"North Bank", "South Bank", "Kneiphof Island", "East Island"};

    // Step 1: Print degrees
    cout << "Degrees of each landmass:\n";
    vector<int> deg = g.getDegrees();
    for (int i = 0; i < (int)deg.size(); i++) {
        cout << "  " << names[i] << ": " << deg[i]
             << (deg[i] % 2 ? " (odd)" : " (even)") << "\n";
    }

    // Step 2: Count odd vertices
    vector<int> odd = g.getOddVertices();
    cout << "\nNumber of odd degree vertices: " << odd.size() << "\n";

    if (odd.size() > 2) {
        cout << "Result: No Eulerian PATH or CIRCUIT exists.\n";
        return 0;
    }

    // Step 3: Determine start point
    int start = 0;
    if (odd.size() == 2) {
        cout << "Result: Eulerian PATH exists (different start & end).\n";
        start = odd[0]; // must start at one odd-degree vertex
    } else {
        cout << "Result: Eulerian CIRCUIT exists (start and end same).\n";
    }

    // Step 4: Find and print Eulerian trail/circuit
    vector<int> path = g.eulerianTrail(start);

    cout << "\nEulerian traversal path:\n";
    for (size_t i = 0; i < path.size(); i++) {
        cout << names[path[i]];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << endl;

    return 0;
}

