#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>

const int64_t kBigNum = 1000 * 1000 * 1000 * 1LL;

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

    [[nodiscard]] virtual std::pair<int64_t, std::pair<int64_t, int64_t>> GetNeighbors(const int64_t& v) const = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
    // virtual int64_t GetEdge(int64_t& source, int64_t& dest) const = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> adjacency_list_;

public:
    int64_t edge_num = 0;
    explicit GraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        oriented = oriented_status;
    }

    [[nodiscard]] std::pair<int64_t, std::pair<int64_t, int64_t>> GetNeighbors(const int64_t& v) const override {
        return adjacency_list_[v];
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        adjacency_list_.emplace_back(std::make_pair(weight, std::make_pair(source, dest)));
        edge_num++;
    }

//    int64_t GetEdge(int64_t& source, int64_t& dest) const override {
//
//    }
};

class UnionSet {
    std::vector<int64_t> parent_;
    std::vector<int64_t> size_;

public:
    UnionSet() {
        parent_.resize(100000);
        size_.resize(100000);
    }

    void MakeSet(int64_t elem) {
        parent_[elem] = elem;
        size_[elem] = 1;
    }

    int64_t FindSet(int64_t elem) {
        if (parent_[elem] == elem) {
            return elem;
        }
        return parent_[elem] = FindSet(parent_[elem]);
    }

    void UnionSets(int64_t first_leader, int64_t sec_leader) {
        first_leader = FindSet(first_leader);
        sec_leader = FindSet(sec_leader);
        if (rand() & 1) {
            std::swap(first_leader, sec_leader);
        }
        if (first_leader != sec_leader) {
            parent_[first_leader] = sec_leader;
        }
    }

    int64_t GetSize(int64_t elem) {
        return size_[elem];
    }
};

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

    int64_t sum_weight = 0;

    UnionSet set;
    for (int i = 0; i < vertex; i++) {
        set.MakeSet(i);
    }

    std::vector<int64_t> less_sum(graph.GetVertexDegree(), kBigNum);

    int64_t vertex_num = graph.GetVertexDegree();
    while (vertex_num > 1) {
        for (int64_t i = 0; i < graph.edge_num; i++) {
            int64_t begin = set.FindSet(graph.GetNeighbors(i).second.first);
            int64_t end = set.FindSet(graph.GetNeighbors(i).second.second);
            if (end == begin) {
                continue;
            }

            if (less_sum[begin] == kBigNum) {
                less_sum[begin] = i;
            }
            if (less_sum[end] == kBigNum) {
                less_sum[end] = i;
            }

            if (graph.GetNeighbors(less_sum[begin]).first > graph.GetNeighbors(i).first) {
                less_sum[begin] = i;
            }
            if (graph.GetNeighbors(less_sum[end]).first > graph.GetNeighbors(i).first) {
                less_sum[begin] = i;
            }
        }

        for (int64_t i = 0; i < graph.GetVertexDegree(); i++) {
            if (less_sum[i] != kBigNum) {
                int64_t begin = set.FindSet(graph.GetNeighbors(less_sum[i]).second.first);
                int64_t end = set.FindSet(graph.GetNeighbors(less_sum[i]).second.second);

                if (end == begin) {
                    continue;
                }

                sum_weight += graph.GetNeighbors(less_sum[i]).first;
                set.UnionSets(begin, end);
                vertex_num--;
            }
        }
        less_sum.assign(graph.GetVertexDegree(), kBigNum);
    }

    std::cout << sum_weight << "\n";
    return 0;
}