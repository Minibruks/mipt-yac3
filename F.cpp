#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>

enum { ORIENTED = true, NOT_ORIENTED = false };

struct Node {
    // int64_t from;
    int64_t to;
    int64_t flow;
    int64_t max_flow;
};

class IGraph {
protected:
    int64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] int64_t GetVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<Node> GetNeighbors(const int64_t& v) const = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
    virtual Node& GetNode(const int64_t& source, const int64_t& dest) = 0;
};

class IGraphList final : public IGraph {
private:
    std::vector<std::vector<Node>> adjacency_list_;

public:
    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<Node>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Node> GetNeighbors(const int64_t& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        adjacency_list_[source].push_back({dest, 0, weight});
        adjacency_list_[dest].push_back({source, 0, weight});
    }

    Node& GetNode(const int64_t& source, const int64_t& dest) override {
        return adjacency_list_[source][dest];
    }
};

int64_t BFS(IGraph& graph, int64_t start, int64_t finish, std::vector<int64_t>& par_list, std::vector<int64_t>& path) {
    par_list.assign(graph.GetVertexDegree(), -1);
    path.assign(graph.GetVertexDegree(), 0);

    std::queue<int64_t> q;
    q.push(start);

    par_list[start] = -1;
    path[start] = 1000000001;
    while (!q.empty()) {
        int64_t curr_node = q.front();
        q.pop();

        std::vector<Node> neighbors = graph.GetNeighbors(curr_node);

        for (auto& neighbor : neighbors) {
            int64_t to = neighbor.to;
            if (par_list[to] == -1) {
                if (neighbor.max_flow - neighbor.flow > 0) {
                    par_list[to] = curr_node;
                    path[to] = std::min(path[curr_node], neighbor.max_flow - neighbor.flow);
                    if (to == finish) {
                        return path[finish];
                    }
                    q.push(to);
                }
            }
        }
    }
    return 0;
}

int64_t EdmondsKarp(IGraph& graph, int start, int finish) {
    std::vector<int64_t> par_list(graph.GetVertexDegree());
    std::vector<int64_t> path(graph.GetVertexDegree());
    int64_t max_flow = 0;
    while (true) {
        int64_t flow = BFS(graph, start, finish, par_list, path);
        if (!flow) {
            break;
        }
        max_flow += flow;
        int64_t curr_node = finish;
        while (curr_node != start) {
            int64_t prev_node = par_list[curr_node];
            graph.GetNode(prev_node, curr_node).flow += flow;
            graph.GetNode(curr_node, prev_node).flow -= flow;
            curr_node = prev_node;
        }
    }
    return max_flow;
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
    std::cout << EdmondsKarp(graph, 0, vertex - 1) << "\n";
}