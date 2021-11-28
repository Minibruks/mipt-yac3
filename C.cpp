#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

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

    [[nodiscard]] virtual std::vector<std::pair<int64_t, int64_t>> GetNeighbors(const int64_t& v) const = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<std::pair<int64_t, int64_t>>> adjacency_list_;

public:
    explicit GraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<std::pair<int64_t, int64_t>>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<std::pair<int64_t, int64_t>> GetNeighbors(const int64_t& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        adjacency_list_[source].emplace_back(std::make_pair(dest, weight));
        if (!oriented) {
            adjacency_list_[dest].emplace_back(std::make_pair(source, weight));
        }
    }
};

int64_t MST(const IGraph& graph) {
    int64_t answer = 0;
    std::vector<int64_t> min_edge_weight(graph.GetVertexDegree(), INT32_MAX);
    std::vector<int64_t> min_edge_end(graph.GetVertexDegree(), -1);
    std::vector<bool> used(graph.GetVertexDegree(), false);
    min_edge_weight[0] = 0;
    std::set<std::pair<int64_t, int64_t>> queue;
    queue.insert(std::make_pair(0, 0));

    for (int64_t i = 0; i < graph.GetVertexDegree(); i++) {
        int64_t current_vertex = queue.begin()->second;
        int64_t current_weight = queue.begin()->first;
        queue.erase(queue.begin());
        used[current_vertex] = true;

        if (min_edge_end[current_vertex] != -1) {
            // answer += min_edge_weight[current_vertex];
            answer += current_weight;
            // std::cout << "vertex: " << current_vertex << " vertex_end: " << min_edge_end[current_vertex] << "\n";
        }

        std::vector<std::pair<int64_t, int64_t>> neighbors = graph.GetNeighbors(current_vertex);
        for (auto& neighbor : neighbors) {
            int64_t possible_edge = neighbor.first;
            int64_t possible_weight = neighbor.second;

            if (possible_weight < min_edge_weight[possible_edge] && !used[possible_edge]) {
                queue.erase(std::make_pair(min_edge_weight[possible_edge], possible_edge));
                min_edge_weight[possible_edge] = possible_weight;
                min_edge_end[possible_edge] = current_vertex;
                queue.insert(std::make_pair(min_edge_weight[possible_edge], possible_edge));
            }
        }
    }
    return answer;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex = 0;
    int edges = 0;
    std::cin >> vertex >> edges;

    GraphList graph(vertex, NOT_ORIENTED);
    for (int i = 0; i < edges; i++) {
        int x = 0;
        int y = 0;
        int weight = 0;
        std::cin >> x >> y >> weight;
        graph.AddEdge(x - 1, y - 1, weight);
    }

    std::cout << MST(graph) << "\n";
}