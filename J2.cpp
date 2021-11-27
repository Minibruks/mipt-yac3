#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>

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

    [[nodiscard]] virtual Edge& GetNeighbors(const int64_t& v) = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;

    // virtual int64_t GetEdge(int64_t& source, int64_t& dest) const = 0;

    // virtual void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
};

class IGraphList : public IGraph {
public:
    std::vector<Edge> list_of_edges_;

    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        // list_of_edges_.resize(vertex_number);
        oriented = oriented_status;
    }

    [[nodiscard]] Edge& GetNeighbors(const int64_t& v) override {
        return list_of_edges_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        Edge a{source, dest, weight};

        // Edge b{dest, source, 0, 0, static_cast<int64_t>(list_of_edges_[source].size())};

        list_of_edges_.push_back(a);
        // list_of_edges_[dest].push_back(b);
    }
};

int64_t Solve(IGraphList& graph, int64_t up_border, int64_t start, int64_t dest) {
    std::vector<int64_t> dists(graph.GetVertexDegree(), kBigNum);
    std::vector<int64_t> save(graph.GetVertexDegree(), kBigNum);
    dists[start] = 0;
    for (int j = 0; j < up_border; j++) {
        save.assign(dists.begin(),  dists.end());
        for (uint64_t i = 0; i < graph.list_of_edges_.size(); i++) {
                int64_t x = graph.list_of_edges_[i].from;
                int64_t y = graph.list_of_edges_[i].to;
                int64_t z = graph.list_of_edges_[i].cost;
                dists[y] = std::min(dists[y], save[x] + z);
            }
    }
    if (dists[dest] >= kBigNum / 2) {
        return -1;
    }
    return dists[dest];
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex = 0;
    int edges = 0;
    std::cin >> vertex >> edges;
    int64_t start = 0;
    int64_t finish = 0;
    int64_t nights_left = 0;
    std::cin >> nights_left >> start >> finish;

    IGraphList graph(vertex, ORIENTED);
    for (int i = 0; i < edges; i++) {
        int x = 0;
        int y = 0;
        int weight = 0;
        std::cin >> x >> y >> weight;
        graph.AddEdge(x - 1, y - 1, weight);
    }
    std::cout << Solve(graph, nights_left, start - 1, finish - 1) << "\n";
    return 0;
}