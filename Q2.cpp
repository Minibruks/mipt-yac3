#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

const int64_t kUndefinedNum = 1000 * 1000 * 1000 * 1LL;
using VertexT = int32_t;
using WeightT = int32_t;
using DistT = int32_t;

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

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight) override {
        Edge edge(vertex_from, vertex_to, weight);
        adjacency_list_[vertex_from].push_back(edge);
        if (!oriented_) {
            Edge edge_back(vertex_to, vertex_from, weight);
            adjacency_list_[vertex_to].push_back(edge_back);
        }
    }
};

struct Node {
    DistT distant;
    VertexT vertex;

    Node(DistT new_dist, VertexT new_vert) {
        distant = new_dist;
        vertex = new_vert;
    }

    void Update(DistT update_dist, VertexT update_vert) {
        distant = update_dist;
        vertex = update_vert;
    }

    bool operator<(const Node& other) const {
        return distant <= other.distant;
    }
};

std::vector<VertexT> Dijkstra(IGraphList& graph, VertexT start) {
    std::vector<VertexT> dists(graph.GetVertexNum(), kUndefinedNum);
    dists[start] = 0;

    std::set<Node> set_for_algo;
    Node node_to_start(dists[start], start);
    set_for_algo.insert(node_to_start);
    while (!set_for_algo.empty()) {
        VertexT current_vert = set_for_algo.begin()->vertex;
        set_for_algo.erase(set_for_algo.begin());
        auto neighbors = graph.GetNeighbors(current_vert);
        for (auto& neighbor : neighbors) {
            VertexT to = neighbor.to;
            VertexT len = neighbor.weight;
            if (dists[current_vert] + len < dists[to]) {
                Node node_to_change(dists[to], to);
                set_for_algo.erase(node_to_change);
                dists[to] = dists[current_vert] + len;
                node_to_change.Update(dists[to], to);
                set_for_algo.insert(node_to_change);
            }
        }
    }

    return dists;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int floor_needed = 0;
    int stairs_up = 0;
    int stairs_down = 0;
    int lift_in = 0;
    int lift_out = 0;
    int lifts_num = 0;
    std::cin >> floor_needed >> stairs_up >> stairs_down >> lift_in >> lift_out >> lifts_num;
    std::vector<std::vector<int>> floors_for_lifts;
    VertexT vertex_num = 0;
    int floors_num = 0;
    for (int i = 0; i < lifts_num; i++) {
        std::vector<int> tmp;
        std::cin >> floors_num;
        for (int j = 0; j < floors_num; j++) {
            int floor = 0;
            std::cin >> floor;
            vertex_num = std::max(vertex_num, floor);
            tmp.push_back(floor);
        }
        floors_for_lifts.push_back(tmp);
    }
    vertex_num = std::max(vertex_num, floor_needed);

    IGraphList graph(vertex_num + lifts_num, ORIENTED);
    for (int i = 1; i < vertex_num; i++) {
        graph.AddEdge(i, i - 1, stairs_down);
        graph.AddEdge(i - 1, i, stairs_up);
    }

    for (VertexT curr_lift = 0; curr_lift < static_cast<VertexT>(floors_for_lifts.size()); curr_lift++) {
        for (VertexT curr_floor = 0; curr_floor < static_cast<VertexT>(floors_for_lifts[curr_lift].size()); curr_floor++) {
            VertexT lift_vertex = floors_for_lifts[curr_lift][curr_floor];
            graph.AddEdge(vertex_num + curr_lift, lift_vertex - 1, lift_in);
            graph.AddEdge(lift_vertex - 1, vertex_num + curr_lift, lift_out);
        }
    }

    std::vector<VertexT> answer = Dijkstra(graph, 0);
    std::cout << answer[floor_needed - 1];
    return 0;
}