#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>

const int64_t kBigNum = 2009000999;

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
        for (int64_t i = 0; i < vertex_number; i++) {
            list_of_edges_[i] = std::vector<Edge>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Edge>& GetNeighbors(const int64_t& v) override {
        return list_of_edges_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        Edge a{source, dest, weight};
        // Edge b{dest, source, weight};

        list_of_edges_[source].push_back(a);
        // list_of_edges_[dest].push_back(b);
    }
};

std::vector<int64_t> Solve(IGraphList& graph, int64_t start) {
    std::vector<int64_t> dists(graph.GetVertexDegree(), kBigNum);
    std::vector<int64_t> p(graph.GetVertexDegree(), kBigNum);
    std::vector<bool> u(graph.GetVertexDegree(), false);
    dists[start] = 0;
    for (int i = 0; i < graph.GetVertexDegree(); i++) {
        int v = -1;
        for (int j = 0; j < graph.GetVertexDegree(); j++) {
            if (!u[j] && (v == -1 || dists[j] < dists[v])) {
                v = j;
            }
        }
        if (dists[v] == kBigNum) {
            break;
        }
        u[v] = true;
        auto neighbors = graph.GetNeighbors(v);
        for (auto& neighbor : neighbors) {
            int64_t to = neighbor.to;
            int64_t len = neighbor.cost;
            if (dists[v] + len < dists[to]) {
                dists[to] = dists[v] + len;
                p[to] = v;
            }
        }
    }
    return dists;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int width = 0;
    int64_t start = 0;
    int64_t finish = 0;
    std::cin >> width >> start >> finish;
    IGraphList graph(width, ORIENTED);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            int64_t weight = 0;
            std::cin >> weight;
            if (weight >= 0 && i != j) {
                graph.AddEdge(i, j, weight);
            }
        }
    }
    std::vector<int64_t> answer = Solve(graph, start - 1);
    if (answer[finish - 1] == kBigNum) {
        answer[finish - 1] = -1;
    }
    std::cout << answer[finish - 1];
    return 0;
}