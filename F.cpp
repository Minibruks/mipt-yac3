#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>

const int64_t kBigNum = 1000 * 1000 * 1000 * 1LL;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct Edge {
    int64_t value;
    int64_t weight;
};

class IGraph {
protected:
    int64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] int64_t GetVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<int64_t>& GetNeighbors(const int64_t& v) = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;

    virtual int64_t GetEdge(int64_t& source, int64_t& dest) const = 0;

    virtual void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
};

class IGraphList : public IGraph {
private:
    std::vector<std::vector<int64_t>> adjacency_list_;

public:
    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<int64_t>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<int64_t>& GetNeighbors(const int64_t& v) override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        adjacency_list_[source].emplace_back(dest + weight);
    }
};

class IGraphMatrix : public IGraph {
private:
    std::vector<std::vector<int64_t>> matrix_;

public:
    explicit IGraphMatrix(const int64_t& quantity) {
        for (int64_t i = 0; i < quantity; ++i) {
            std::vector<int64_t> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_degree = quantity;
    }

    [[nodiscard]] std::vector<int64_t>& GetNeighbors(const int64_t& v) override {
        return matrix_[v];
    }

    void AddEdge(const int64_t& a, const int64_t& b, const int64_t& weight) override {
        if (a == b) {
            return;
        }
        matrix_[a][b] = weight;
    }

    int64_t GetEdge(int64_t& source, int64_t& dest) const override {
        return matrix_[source][dest];
    }

    void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        matrix_[source][dest] = weight;
    }
};

bool BFS(IGraph& g, const int64_t& start, const int64_t& finish, std::vector<bool>& used,
         std::vector<int64_t>& parent) {
    used.assign(g.GetVertexDegree(), false);
    parent.assign(g.GetVertexDegree(), kBigNum);
    std::queue<int64_t> queue;
    queue.push(start);

    used[start] = true;
    parent[start] = kBigNum;

    while (!queue.empty()) {
        int64_t v = queue.front();
        queue.pop();

        auto neighbors = g.GetNeighbors(v);
        for (int64_t u = 0; u < g.GetVertexDegree(); u++) {
            if (!used[u] && neighbors[u] > 0) {
                parent[u] = v;
                if (u == finish) {
                    return true;
                }
                queue.push(u);
                parent[u] = v;
                used[u] = true;
            }
        }
    }

    return false;
}

int64_t EdmondsKarp(IGraph& graph, int start, int finish) {
    std::vector<bool> used(graph.GetVertexDegree());
    std::vector<int64_t> path(graph.GetVertexDegree());
    int64_t max_flow = 0;
    while (true) {
        int64_t flow = kBigNum;
        if (!BFS(graph, start, finish, used, path)) {
            break;
        }
        for (int64_t v = finish; v != start; v = path[v]) {
            int64_t u = path[v];
            flow = std::min(flow, graph.GetEdge(u, v));
        }

        for (int64_t v = finish; v != start; v = path[v]) {
            int64_t u = path[v];

            graph.SetEdge(u, v, graph.GetEdge(u, v) - flow);
            graph.SetEdge(v, u, graph.GetEdge(v, u) + flow);
        }

        max_flow += flow;
    }
    return max_flow;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex = 0;
    int edges = 0;
    std::cin >> vertex >> edges;

    IGraphMatrix graph(vertex);
    for (int i = 0; i < edges; i++) {
        int x = 0;
        int y = 0;
        int weight = 0;
        std::cin >> x >> y >> weight;
        graph.AddEdge(x - 1, y - 1, weight);
    }
    std::cout << EdmondsKarp(graph, 0, vertex - 1) << "\n";
}
