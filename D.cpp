#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

const int64_t kUndefinedDist = 1000 * 1000 * 1000 * 1LL;
using VertexT = int32_t;
using WeightT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct Edge {
    VertexT from;
    VertexT to;
    WeightT weight;

    explicit Edge() {
        from = 0;
        to = 0;
        weight = 0;
    }

    explicit Edge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight_from_to) {
        from = vertex_from;
        to = vertex_to;
        weight = weight_from_to;
    }

    void Update(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight_from_to) {
        from = vertex_from;
        to = vertex_to;
        weight = weight_from_to;
    }

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<Edge> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight) = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<Edge>> adjacency_list_;

public:
    explicit GraphList(const VertexT& vertex_number, bool oriented_status) {
        vertex_num_ = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (VertexT i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<Edge>();
        }
        oriented_ = oriented_status;
    }

    [[nodiscard]] std::vector<Edge> GetNeighbors(const VertexT& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight) override {
        Edge edge(vertex_from, vertex_to, weight);
        adjacency_list_[vertex_from].push_back(edge);
        if (!oriented_) {
            Edge edge_back(vertex_to, vertex_from, weight);
            adjacency_list_[vertex_to].push_back(edge_back);
        }
    }
};

VertexT MST(const IGraph& graph) {
    VertexT min_span_tree_sum = 0;
    std::vector<VertexT> min_edge_weight(graph.GetVertexNum(), kUndefinedDist);
    std::vector<VertexT> min_edge_end(graph.GetVertexNum(), -1);
    std::vector<bool> used(graph.GetVertexNum(), false);

    min_edge_weight[0] = 0;

    std::set<Edge> set_for_find_mst;
    Edge edge_to_start;
    set_for_find_mst.insert(edge_to_start);

    for (VertexT i = 0; i < graph.GetVertexNum(); i++) {
        VertexT current_vertex = set_for_find_mst.begin()->to;
        VertexT current_weight = set_for_find_mst.begin()->weight;
        set_for_find_mst.erase(set_for_find_mst.begin());
        
        used[current_vertex] = true;

        if (min_edge_end[current_vertex] != -1) {
            min_span_tree_sum += current_weight;
        }

        std::vector<Edge> neighbors = graph.GetNeighbors(current_vertex);
        for (auto& neighbor : neighbors) {
            VertexT possible_edge = neighbor.to;
            VertexT possible_weight = neighbor.weight;

            if (possible_weight < min_edge_weight[possible_edge] && !used[possible_edge]) {
                Edge edge_to_change(neighbor.from, possible_edge, min_edge_weight[possible_edge]);
                set_for_find_mst.erase(edge_to_change);
                min_edge_weight[possible_edge] = possible_weight;
                min_edge_end[possible_edge] = current_vertex;
                edge_to_change.Update(neighbor.from, possible_edge, min_edge_weight[possible_edge]);
                set_for_find_mst.insert(edge_to_change);
            }
        }
    }
    return min_span_tree_sum;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int spies_num = 0;
    std::cin >> spies_num;

    GraphList graph(spies_num + 1, NOT_ORIENTED);
    for (VertexT vertex_from = 0; vertex_from < spies_num; vertex_from++) {
        for (VertexT vertex_to = 0; vertex_to < spies_num; vertex_to++) {
            VertexT weight = 0;
            std::cin >> weight;
            if (vertex_to > vertex_from) {
                graph.AddEdge(vertex_from, vertex_to, weight);
            }
        }
    }

    for (VertexT spy = 0; spy < spies_num; spy++) {
        VertexT phantom_vertex_weight = 0;
        std::cin >> phantom_vertex_weight;
        graph.AddEdge(spy, spies_num, phantom_vertex_weight);
    }

    std::cout << MST(graph) << "\n";
}