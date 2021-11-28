#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>

const int64_t kBigNum = 1000 * 1000 * 1000 * 1LL;
const int64_t kPoison = 100000;

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

void RelaxGraph(IGraphList& graph, std::vector<int64_t>& dists, std::vector<int64_t>& parent, std::vector<bool>& visited) {
    for (auto iteration = 0; iteration < graph.GetVertexDegree(); iteration++) {
        for (auto vertex = 0; vertex < graph.GetVertexDegree(); vertex++) {
            for (auto neighbor : graph.list_of_edges_[vertex]) {
                auto next = neighbor.to;
                auto weight = neighbor.cost;
                if (dists[vertex] != kBigNum && dists[vertex] + weight < dists[next]) {
                    dists[next] = dists[vertex] + weight;
                    visited[vertex] = true;
                    parent[next] = vertex;
                }
            }
        }
    }
}

bool Relax(std::vector<int64_t>& dists, std::vector<int64_t>& parent, int64_t vertex, int64_t neigb, int64_t weight) {
    if (dists[vertex] != kBigNum && dists[neigb] > dists[vertex] + weight) {
        dists[neigb] = dists[vertex] + weight;
        parent[neigb] = vertex;
        return true;
    }
    return false;
}

std::vector<int64_t> Solve(IGraphList& graph, int64_t start, std::vector<bool>& used) {
    std::vector<int64_t> dists(graph.GetVertexDegree(), kBigNum);
    dists[start] = 0;
    used[start] = true;
    std::vector<int64_t> parent(graph.GetVertexDegree(), -1);

    RelaxGraph(graph, dists, parent, used);

    int64_t first = -1;
    for (auto vertex = 0; vertex < graph.GetVertexDegree(); vertex++) {
        for (auto neighbor : graph.list_of_edges_[vertex]) {
            auto next = neighbor.to;
            auto weight = neighbor.cost;
            if (Relax(dists, parent, vertex, next, weight)) {
                first = next;
            }
        }
    }
    std::vector<int64_t> cycle;
    if (first != -1) {
        for (int i = 0; i < graph.GetVertexDegree(); i++) {
            first = parent[first];
        }
        cycle.push_back(first);
        for (auto v = parent[first]; v != first; v = parent[v]) {
            cycle.push_back(v);
        }
        cycle.push_back(first);
        std::reverse(cycle.begin(), cycle.end());
    }
    return cycle;
}

std::vector<int64_t> Cycle(IGraphList& graph) {
    std::vector<int64_t> ans;
    std::vector<bool> used(graph.GetVertexDegree(), false);
    for (auto i = 0; i < graph.GetVertexDegree(); i++) {
        if (!used[i]) {
            ans = Solve(graph, (int64_t)i, used);
        }
        if (!ans.empty()) {
            return ans;
        }
    }
    return ans;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex = 0;
    std::cin >> vertex;

    IGraphList graph(vertex, ORIENTED);
    for (auto i = 0; i < vertex; i++) {
        for (auto j = 0; j < vertex; j++) {
            auto weight = 0;
            std::cin >> weight;
            if (weight != kPoison) {
                graph.AddEdge(i, j, weight);
            }
        }
    }
    std::vector<int64_t> ans = Cycle(graph);
    if (!ans.empty()) {
        std::cout << "YES" << "\n";
        std::cout << ans.size() << "\n";
        for (auto elem : ans) {
            std::cout << elem + 1 << ' ';
        }
    } else {
        std::cout << "NO";
    }
    return 0;
}