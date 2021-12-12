#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

using VertexT = int32_t;
using WeightT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

template <typename DataT>
struct Edge {
    VertexT from;
    VertexT to;
    WeightT weight;
    DataT data;

    explicit Edge() {
        from = 0;
        to = 0;
        weight = 0;
        data = 0;
    }

    explicit Edge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& new_weight = 0,
                  DataT new_data = 0) {
        from = vertex_from;
        to = vertex_to;
        weight = new_weight;
        data = new_data;
    }
};

template <typename DataT>
class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    enum { UNDEFINED_DIST = 1000 * 1000 * 1000 * 1LL };

    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<Edge<DataT>> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight,
                         const DataT& data) = 0;
};

template <typename DataT>
class IGraphList final : public IGraph<DataT> {
private:
    std::vector<std::vector<Edge<DataT>>> adjacency_list_;

public:
    explicit IGraphList(const VertexT& vertex_number, bool oriented_status) {
        this->vertex_num_ = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (VertexT i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<Edge<DataT>>();
        }
        this->oriented_ = oriented_status;
    }

    [[nodiscard]] std::vector<Edge<DataT>> GetNeighbors(const VertexT& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight,
                 const DataT& data) override {
        Edge<DataT> edge(vertex_from, vertex_to, weight, data);
        adjacency_list_[vertex_from].push_back(edge);
        if (!this->oriented_) {
            Edge<DataT> edge_back(vertex_to, vertex_from, weight, data);
            adjacency_list_[vertex_to].push_back(edge_back);
        }
    }
};

template <typename DataT>
std::vector<int> FordBellman(IGraph<DataT>& graph, VertexT start) {
    std::vector<int> dists(graph.GetVertexNum(), IGraph<DataT>::UNDEFINED_DIST);
    dists[start] = 0;
    while (true) {
        bool all_visited = false;
        for (int i = 0; i < graph.GetVertexNum(); i++) {
            auto neighbors = graph.GetNeighbors(i);
            for (auto& elem : neighbors) {
                if (dists[elem.from] < IGraph<DataT>::UNDEFINED_DIST) {
                    if (dists[elem.from] <= elem.data && elem.data + elem.weight < dists[elem.to]) {
                        dists[elem.to] = elem.data + elem.weight;
                        all_visited = true;
                    }
                }
            }
        }
        if (!all_visited) {
            break;
        }
    }
    return dists;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex_num = 0;
    int edges_num = 0;
    VertexT start = 0;
    VertexT finish = 0;
    std::cin >> vertex_num >> start >> finish >> edges_num;

    IGraphList<int> graph(vertex_num, ORIENTED);
    for (int i = 0; i < edges_num; i++) {
        VertexT from = 0;
        VertexT to = 0;
        int time_from = 0;
        int time_to = 0;
        std::cin >> from >> time_from >> to >> time_to;
        graph.AddEdge(from - 1, to - 1, time_to - time_from, time_from);
    }
    std::vector<int> dists = FordBellman(graph, start - 1);
    std::cout << dists[finish - 1] << "\n";
    return 0;
}
