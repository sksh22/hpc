#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

vector<vector<int>> graph;
vector<bool> visited_bfs;
vector<bool> visited_dfs;

void sequential_bfs(int start) {
    queue<int> q;
    vector<bool> visited(graph.size(), false);
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int node = q.front(); q.pop();
        cout << node << " ";
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

void parallel_bfs(int start) {
    queue<int> q;
    q.push(start);
    visited_bfs[start] = true;

    cout << "[Parallel BFS Output] ";

    while (!q.empty()) {
        int node = q.front(); q.pop();
        cout << node << " ";

        #pragma omp parallel
        {
            #pragma omp single
            {
                cout << "\nThreads used in this BFS level: " << omp_get_num_threads() << endl;
            }

            #pragma omp for
            for (int i = 0; i < graph[node].size(); i++) {
                int next = graph[node][i];
                if (!visited_bfs[next]) {
                    #pragma omp critical
                    {
                        if (!visited_bfs[next]) {
                            visited_bfs[next] = true;
                            q.push(next);
                        }
                    }
                }
            }
        }
    }
}

void sequential_dfs(int start) {
    stack<int> s;
    vector<bool> visited(graph.size(), false);
    s.push(start);

    while (!s.empty()) {
        int node = s.top(); s.pop();
        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";
            for (int i = graph[node].size() - 1; i >= 0; i--) {
                int neighbor = graph[node][i];
                if (!visited[neighbor]) {
                    s.push(neighbor);
                }
            }
        }
    }
}

void parallel_dfs(int start) {
    stack<int> s;
    s.push(start);

    cout << "[Parallel DFS Output] ";

    while (!s.empty()) {
        int node = s.top(); s.pop();
        if (!visited_dfs[node]) {
            visited_dfs[node] = true;
            cout << node << " ";

            #pragma omp parallel
            {
                #pragma omp single
                {
                    cout << "\nThreads used in this DFS level: " << omp_get_num_threads() << endl;
                }

                #pragma omp for
                for (int i = graph[node].size() - 1; i >= 0; i--) {
                    int neighbor = graph[node][i];
                    if (!visited_dfs[neighbor]) {
                        #pragma omp critical
                        s.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int nodes, edges;
    cout << "Enter number of nodes: ";
    cin >> nodes;
    cout << "Enter number of edges: ";
    cin >> edges;

    graph.resize(nodes);
    visited_bfs.resize(nodes, false);
    visited_dfs.resize(nodes, false);

    cout << "Enter " << edges << " edges (format: u v):\n";
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // undirected
    }

    double start_time, end_time;

    cout << "\nSequential BFS: ";
    start_time = omp_get_wtime();
    sequential_bfs(0);
    end_time = omp_get_wtime();
    cout << "\nTime: " << (end_time - start_time) << " seconds\n";

    cout << "\nParallel BFS: ";
    start_time = omp_get_wtime();
    parallel_bfs(0);
    end_time = omp_get_wtime();
    cout << "\nTime: " << (end_time - start_time) << " seconds\n";

    cout << "\nSequential DFS: ";
    start_time = omp_get_wtime();
    sequential_dfs(0);
    end_time = omp_get_wtime();
    cout << "\nTime: " << (end_time - start_time) << " seconds\n";

    cout << "\nParallel DFS: ";
    start_time = omp_get_wtime();
    parallel_dfs(0);
    end_time = omp_get_wtime();
    cout << "\nTime: " << (end_time - start_time) << " seconds\n";

    return 0;
}

/* Enter number of nodes: 7
Enter number of edges: 11
Enter 11 edges (format: u v):
0 1
0 3
3 1
3 2
3 4
1 2 
1 6
1 5
2 4
4 6
2 5 */