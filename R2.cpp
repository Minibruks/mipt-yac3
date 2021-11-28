#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

const size_t kBigNum = SIZE_MAX;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct SetOfLists {
    std::vector<bool> used;
    std::vector<int64_t> parent;
};

struct Edge {
    size_t from;
    size_t to;
    size_t cost;
};

class IGraph {
protected:
    size_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] size_t GetVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<Edge>& GetNeighbors(const size_t& v) = 0;

    virtual void AddEdge(const size_t& source, const size_t& dest, const size_t& weight) = 0;

    // virtual int64_t GetEdge(int64_t& source, int64_t& dest) const = 0;

    // virtual void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
};

class IGraphList : public IGraph {
public:
    std::vector<std::vector<Edge>> list_of_edges_;

    explicit IGraphList(const size_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        list_of_edges_.resize(vertex_number);
        for (size_t i = 0; i < vertex_number; i++) {
            list_of_edges_[i] = std::vector<Edge>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Edge>& GetNeighbors(const size_t& v) override {
        return list_of_edges_.at(v);
    }

    void AddEdge(const size_t& source, const size_t& dest, const size_t& weight) override {
        Edge a{source, dest, weight};
        Edge b{dest, source, weight};

        list_of_edges_[source].push_back(a);
        list_of_edges_[dest].push_back(b);
    }
};

std::vector<size_t> Solve(IGraphList& graph, std::vector<size_t>& ill) {
    std::vector<size_t> dists(graph.GetVertexDegree(), kBigNum);
    // dists[start] = 0;

    std::set<std::pair<size_t, size_t>> s;
    // s.insert(std::make_pair(dists[start], start));
    for (auto& vertex : ill) {
        dists[vertex] = 0;
        s.insert(std::make_pair(dists[vertex], vertex));
    }

    while (!s.empty()) {
        size_t v = s.begin()->second;
        s.erase(s.begin());
        auto neighbors = graph.GetNeighbors(v);
        for (auto& neighbor : neighbors) {
            size_t to = neighbor.to;
            size_t len = neighbor.cost;
            if (dists[v] + len < dists[to]) {
                s.erase(std::make_pair(dists[to], to));
                dists[to] = dists[v] + len;
                s.insert(std::make_pair(dists[to], to));
            }
        }
    }

    return dists;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t n = 0;
    size_t m = 0;
    size_t k = 0;
    std::cin >> n >> m >> k;
    std::vector<size_t> ill;

    for (size_t i = 0; i < k; i++) {
        size_t x = 0;
        std::cin >> x;
        ill.push_back(x - 1);
    }

    IGraphList graph(n, NOT_ORIENTED);
    for (size_t i = 0; i < m; i++) {
        size_t x = 0;
        size_t y = 0;
        size_t weight = 0;
        std::cin >> x >> y >> weight;
        graph.AddEdge(x - 1, y - 1, weight);
    }
    size_t start = 0;
    size_t finish = 0;
    std::cin >> start >> finish;

    std::vector<size_t> ans1 = Solve(graph, ill);
    std::vector<size_t> f;
    f.push_back(start - 1);
    std::vector<size_t> ans2 = Solve(graph, f);
    // std::cout << ans1[finish - 1] << " " << ans2[finish - 1];
    if (ans2[finish - 1] < ans1[finish - 1]) {
        std::cout << ans2[finish - 1] << "\n";
    } else {
        std::cout << -1 << "\n";
    }

    return 0;
}