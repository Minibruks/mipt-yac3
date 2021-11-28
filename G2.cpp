#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>

// const int64_t kBigNum = 1000 * 1000 * 1000 * 1LL;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct SetOfLists {
    std::vector<bool> used;
    std::vector<int64_t> parent;
};

struct Edge {
    int64_t from;
    int64_t to;
    int64_t flow;
    int64_t capacity;
    int64_t reverse;
};

class IGraph {
protected:
    int64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] int64_t GetVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<Edge>& GetNeighbors(const int64_t& v) = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;

    // virtual int64_t GetEdge(int64_t& source, int64_t& dest) const = 0;

    // virtual void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
};

class IGraphList : public IGraph {
public:
    std::vector<std::vector<Edge>> adjacency_list_;

    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<Edge>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Edge>& GetNeighbors(const int64_t& v) override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        Edge a{source, dest, 0, weight, static_cast<int64_t>(adjacency_list_[dest].size())};

        Edge b{dest, source, 0, 0, static_cast<int64_t>(adjacency_list_[source].size())};

        adjacency_list_[source].push_back(a);
        adjacency_list_[dest].push_back(b);
    }
};

bool BFS(IGraphList& g, const int64_t& start, const int64_t& finish, std::vector<int64_t>& level) {
    // used.assign(g.GetVertexDegree(), false);
    level.assign(g.GetVertexDegree(), -1);
    std::list<int64_t> l;
    l.push_back(start);

    // used[start] = true;
    level[start] = 0;

    while (!l.empty()) {
        int64_t v = l.front();
        l.pop_front();

        auto neighbors = g.GetNeighbors(v);
        for (auto& neighbor : neighbors) {
            if (level[neighbor.to] == -1 && neighbor.flow < neighbor.capacity) {
                level[neighbor.to] = level[v] + 1;
                l.push_back(neighbor.to);
            }
        }
    }

    return level[finish] >= 0;
}

int64_t DFS(IGraphList& graph, int64_t& start, int64_t& finish, int64_t flow, std::vector<int64_t>& level,
            std::vector<int64_t>& path) {
    if (start == finish) {
        return flow;
    }
    // auto neighbors = graph.GetNeighbors(start);
    for (; path[start] < static_cast<int64_t>(graph.adjacency_list_[start].size()); path[start]++) {
        Edge& neighbor = graph.adjacency_list_[start][path[start]];
        if (level[neighbor.to] == level[start] + 1 && neighbor.flow < neighbor.capacity) {
            int64_t curr_flow = std::min(flow, neighbor.capacity - neighbor.flow);
            int64_t tmp_flow = DFS(graph, neighbor.to, finish, curr_flow, level, path);

            if (tmp_flow > 0) {
                neighbor.flow += tmp_flow;
                graph.adjacency_list_[neighbor.to][neighbor.reverse].flow -= tmp_flow;
                return tmp_flow;
            }
        }
    }
    return 0;
}

int64_t Dinic(IGraphList& graph, int64_t start, int64_t finish) {
    int64_t total = 0;
    std::vector<bool> used(graph.GetVertexDegree());
    std::vector<int64_t> level(graph.GetVertexDegree());

    while (BFS(graph, start, finish, level)) {
        std::vector<int64_t> path(graph.GetVertexDegree() + 1, 0);

        while (int64_t flow = DFS(graph, start, finish, INT32_MAX, level, path)) {
            total += flow;
        }
    }

    return total;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex = 0;
    int edges = 0;
    std::cin >> vertex >> edges;

    IGraphList graph(vertex, ORIENTED);
    for (int i = 0; i < edges; i++) {
        int x = 0;
        int y = 0;
        int weight = 0;
        std::cin >> x >> y >> weight;
        graph.AddEdge(x - 1, y - 1, weight);
    }
    std::cout << Dinic(graph, 0, vertex - 1) << "\n";
}