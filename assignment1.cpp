#include <iostream>
#include <omp.h>
#include <vector>
#include <queue>
using namespace std;

class Graph
{
private:
    int vertices;
    vector<vector<int>> adjList;

public:
    Graph(int vertices)
    {
        this->vertices = vertices;
        adjList.resize(vertices);
    }

    void addEdge(int u, int v)
    {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void printGraph()
    {
        for (int i = 0; i < vertices; i++)
        {
            cout << i << " -> ";
            for (int j = 0; j < adjList[i].size(); j++)
            {
                cout << adjList[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Sequential BFS
    void BFS_sequential(int start)
    {
        vector<bool> visited(vertices, false);
        queue<int> queue;
        queue.push(start);
        visited[start] = true;

        while (!queue.empty())
        {
            int current = queue.front();
            queue.pop();
            cout << "Visited " << current << endl;

            for (int i = 0; i < adjList[current].size(); i++)
            {
                if (!visited[adjList[current][i]])
                {
                    queue.push(adjList[current][i]);
                    visited[adjList[current][i]] = true;
                }
            }
        }
    }

    // Parallel BFS
    void BFS_parallel(int start)
    {
        vector<bool> visited(vertices, false);
        queue<int> queue;
        queue.push(start);
        visited[start] = true;

        while (!queue.empty())
        {
            int current;

#pragma omp parallel shared(queue, visited)
            {

#pragma omp single
                {
                    current = queue.front();
                    queue.pop();
                    cout << "Thread " << omp_get_thread_num() << " visited " << current << endl;
                }

#pragma omp for
                for (int i = 0; i < adjList[current].size(); i++)
                {
                    if (!visited[adjList[current][i]])
                    {
#pragma omp critical
                        {
                            queue.push(adjList[current][i]);
                            visited[adjList[current][i]] = true;
                        }
                    }
                }
            }
        }
    }

    // Sequential DFS
    void DFS_sequential(int start)
    {
        vector<bool> visited(vertices, false);
        DFSUtil_sequential(start, visited);
    }

    void DFSUtil_sequential(int v, vector<bool> &visited)
    {
        visited[v] = true;
        cout << "Visited " << v << endl;

        for (int i = 0; i < adjList[v].size(); i++)
        {
            int u = adjList[v][i];
            if (!visited[u])
            {
                DFSUtil_sequential(u, visited);
            }
        }
    }

    // Parallel DFS
    void DFS_parallel(int start)
    {
        vector<bool> visited(vertices, false);
        DFSUtil_parallel(start, visited);
    }

    void DFSUtil_parallel(int v, vector<bool> &visited)
    {

#pragma omp critical
        {
            visited[v] = true;
            cout << "Thread " << omp_get_thread_num() << " visited " << v << endl;
        }

#pragma omp parallel for
        for (int i = 0; i < adjList[v].size(); i++)
        {
            int u = adjList[v][i];
            if (!visited[u])
            {
                DFSUtil_parallel(u, visited);
            }
        }
    }
};

int main()
{
    Graph g(10);

    // Adding edges for a larger graph (10 vertices)
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 5);
    g.addEdge(2, 3);
    g.addEdge(2, 6);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 8);
    g.addEdge(8, 9);
    g.addEdge(7, 9);

    g.printGraph();

    // Timing BFS (Sequential)
    double start_time = omp_get_wtime();
    cout << "BFS (Sequential) traversal starting from vertex 0: " << endl;
    g.BFS_sequential(0);
    double end_time = omp_get_wtime();
    cout << "Sequential BFS Time: " << (end_time - start_time) * 1000 << " ms" << endl;
    cout << endl;

    // Timing BFS (Parallel)
    start_time = omp_get_wtime();
    cout << "BFS (Parallel) traversal starting from vertex 0: " << endl;
    g.BFS_parallel(0);
    end_time = omp_get_wtime();
    cout << "Parallel BFS Time: " << (end_time - start_time) * 1000 << " ms" << endl;
    cout << endl;

    // Timing DFS (Sequential)
    start_time = omp_get_wtime();
    cout << "DFS (Sequential) traversal starting from vertex 0: " << endl;
    g.DFS_sequential(0);
    end_time = omp_get_wtime();
    cout << "Sequential DFS Time: " << (end_time - start_time) * 1000 << " ms" << endl;
    cout << endl;

    // Timing DFS (Parallel)
    start_time = omp_get_wtime();
    cout << "DFS (Parallel) traversal starting from vertex 0: " << endl;
    g.DFS_parallel(0);
    end_time = omp_get_wtime();
    cout << "Parallel DFS Time: " << (end_time - start_time) * 1000 << " ms" << endl;

    return 0;
}
