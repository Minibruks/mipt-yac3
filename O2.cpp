#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>
#include <set>

const int64_t kBigNum = 1000 * 1000 * 1000 * 1LL;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct SetOfLists {
    std::vector<bool> used;
    std::vector<int64_t> parent;
};

struct Edge {
    int64_t from;
    int64_t to;
    int64_t cost;
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
    std::vector<std::vector<Edge>> list_of_edges_;

    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        list_of_edges_.resize(vertex_number);
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Edge>& GetNeighbors(const int64_t& v) override {
        return list_of_edges_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        Edge a{source, dest, weight};

        // Edge b{dest, source, 0, 0, static_cast<int64_t>(list_of_edges_[source].size())};

        list_of_edges_[source].push_back(a);
        // list_of_edges_[dest].push_back(b);
    }
};

std::vector<int64_t> SolveDijkstra(IGraphList& graph, int64_t start) {
    std::vector<int64_t> dists(graph.GetVertexDegree(), kBigNum);
    dists[start] = 0;

    std::set<std::pair<int64_t, int64_t>> s;
    s.insert(std::make_pair(dists[start], start));
    while (!s.empty()) {
        int64_t v = s.begin()->second;
        s.erase(s.begin());
        auto neighbors = graph.GetNeighbors(v);
        for (auto& neighbor : neighbors) {
            int64_t to = neighbor.to;
            int64_t len = neighbor.cost;
            if (dists[v] + len < dists[to]) {
                s.erase(std::make_pair(dists[to], to));
                dists[to] = dists[v] + len;
                s.insert(std::make_pair(dists[to], to));
            }
        }
    }

    return dists;
}

std::vector<int64_t> Solve(IGraphList& graph, int64_t start) {
    std::vector<int64_t> dists(graph.GetVertexDegree(), kBigNum);
    dists[start] = 0;
    for (int64_t x = 0; x < graph.GetVertexDegree(); x++) {
        for (int64_t y = 0; y < graph.GetVertexDegree(); y++) {
            for (uint64_t i = 0; i < graph.list_of_edges_[y].size(); i++) {
                if (dists[graph.list_of_edges_[y][i].from] < kBigNum) {
                    if (dists[graph.list_of_edges_[y][i].to] >
                        dists[graph.list_of_edges_[y][i].from] + graph.list_of_edges_[y][i].cost) {
                        dists[graph.list_of_edges_[y][i].to] =
                                dists[graph.list_of_edges_[y][i].from] + graph.list_of_edges_[y][i].cost;
                    }
                }
            }
        }
    }
    return dists;
}

std::vector<std::vector<Edge>> Shortest(IGraphList& graph) {

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

    return 0;
}