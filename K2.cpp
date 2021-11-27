#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

const int64_t kUndefinedDist = 1000 * 1000 * 1000 * 1LL;
using VertexT = int32_t;
using TimeT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct Edge {
    VertexT from;
    VertexT to;
    TimeT time_from;
    TimeT time_in;

    explicit Edge() {
        from = 0;
        to = 0;
        time_from = 0;
        time_in = 0;
    }

    explicit Edge(const VertexT& vertex_from, const VertexT& vertex_to, const TimeT& time_from_vertex,
                  const TimeT& time_in_vertex) {
        from = vertex_from;
        to = vertex_to;
        time_from = time_from_vertex;
        time_in = time_in_vertex;
    }

    void Update(const VertexT& vertex_from, const VertexT& vertex_to, const TimeT& time_from_vertex,
                const TimeT& time_in_vertex) {
        from = vertex_from;
        to = vertex_to;
        time_from = time_from_vertex;
        time_in = time_in_vertex;
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

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const TimeT& time_from_vertex,
                         const TimeT& time_in_vertex) = 0;
};

class IGraphList final : public IGraph {
private:
    std::vector<std::vector<Edge>> adjacency_list_;

public:
    explicit IGraphList(const VertexT& vertex_number, bool oriented_status) {
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

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const TimeT& time_from_vertex,
                 const TimeT& time_in_vertex) override {
        Edge edge(vertex_from, vertex_to, time_from_vertex, time_in_vertex);
        adjacency_list_[vertex_from].push_back(edge);
        if (!oriented_) {
            Edge edge_back(vertex_to, vertex_from, time_from_vertex, time_in_vertex);
            adjacency_list_[vertex_to].push_back(edge_back);
        }
    }
};

VertexT FordBellman(IGraphList& graph, VertexT start, VertexT dest) {
    std::vector<VertexT> dists(graph.GetVertexNum(), kUndefinedDist);
    dists[start] = 0;
    while (true) {
        bool all_visited = false;
        for (VertexT i = 0; i < graph.GetVertexNum(); i++) {
            auto neighbors = graph.GetNeighbors(i);
            for (auto& elem : neighbors) {
                if (dists[elem.from] < kUndefinedDist) {
                    if (dists[elem.from] <= elem.time_from && elem.time_in < dists[elem.to]) {
                        dists[elem.to] = elem.time_in;
                        all_visited = true;
                    }
                }
            }
        }
        if (!all_visited) {
            break;
        }
    }
    return dists[dest];
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    VertexT vertex_num = 0;
    VertexT edges_num = 0;
    VertexT start = 0;
    VertexT finish = 0;
    std::cin >> vertex_num >> start >> finish >> edges_num;

    IGraphList graph(vertex_num, ORIENTED);
    for (int i = 0; i < edges_num; i++) {
        VertexT from = 0;
        VertexT to = 0;
        TimeT time_from = 0;
        TimeT time_to = 0;
        std::cin >> from >> time_from >> to >> time_to;
        graph.AddEdge(from - 1, to - 1, time_from, time_to);
    }
    std::cout << FordBellman(graph, start - 1, finish - 1) << "\n";
    return 0;
}