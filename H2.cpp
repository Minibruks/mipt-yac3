#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <list>

// const int64_t kBigNum = 1000 * 1000 * 1000 * 1LL;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct SetOfLists {
    std::vector<bool> used;
    std::vector<int64_t> parent;
};

struct Edge {
    int64_t from;
    int64_t to;
    int64_t flow;
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
    std::vector<std::vector<Edge>> adjacency_list_;

    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<Edge>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Edge>& GetNeighbors(const int64_t& v) override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        Edge a{source, dest, weight};

        // Edge b{dest, source, 0, 0, static_cast<int64_t>(adjacency_list_[source].size())};

        adjacency_list_[source].push_back(a);
        // adjacency_list_[dest].push_back(b);
    }
};

int Valence(char atom) {
    switch (atom) {
        case ('H'):
            return 1;
        case ('O'):
            return 2;
        case ('N'):
            return 3;
        case ('C'):
            return 4;
    }
    return 0;
}

int64_t ToVertNum(int i, int j, int width) {
    return i * width + j + 1;
}

void Init(IGraphList& graph, int height, int width, int64_t& start_sum, int64_t& target_sum) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char atom = ' ';
            std::cin >> atom;
            if ((i % 2 + j % 2) % 2 == 0) {
                graph.AddEdge(0, ToVertNum(i, j, width), Valence(atom));
                start_sum += Valence(atom);
                if (i + 1 < height) {
                    graph.AddEdge(ToVertNum(i, j, width), ToVertNum(i + 1, j, width), 1);
                }
                if (j + 1 < width) {
                    graph.AddEdge(ToVertNum(i, j, width), ToVertNum(i, j + 1, width), 1);
                }
                if (i - 1 >= 0) {
                    graph.AddEdge(ToVertNum(i, j, width), ToVertNum(i - 1, j, width), 1);
                }
                if (j - 1 >= 0) {
                    graph.AddEdge(ToVertNum(i, j, width), ToVertNum(i, j - 1, width), 1);
                }
            } else {
                graph.AddEdge(ToVertNum(i, j, width), graph.GetVertexDegree() - 1, Valence(atom));
                target_sum += Valence(atom);
            }
        }
    }
}

int64_t FindBackEdge(IGraphList& graph, Edge edge) {
    // auto neighb
    for (int64_t child = 0; child < static_cast<int64_t>(graph.adjacency_list_[edge.to].size()); child++) {
        if (graph.adjacency_list_[edge.to][child].to == edge.from) {
            return child;
        }
    }
    return -1;
}

int64_t TryToBuild(IGraphList& graph, std::vector<bool>& used, int64_t& max_flow, int64_t current = 0,
                   int64_t capacity = 5) {
    used[current] = true;
    if (current == graph.GetVertexDegree() - 1) {
        max_flow += capacity;
        return capacity;
    }
    auto& neighbors = graph.GetNeighbors(current);
    for (auto& neighbor : neighbors) {
        if (used[neighbor.to] || neighbor.flow == 0) {
            continue;
        }
        int64_t min_flow = TryToBuild(graph, used, max_flow, neighbor.to, std::min(neighbor.flow, capacity));
        if (min_flow > 0) {
            neighbor.flow -= min_flow;
            int64_t back_edge_num = FindBackEdge(graph, neighbor);
            if (back_edge_num != -1) {
                graph.adjacency_list_[neighbor.to][back_edge_num].flow += min_flow;
            } else {
                graph.adjacency_list_[neighbor.to].push_back({neighbor.to, current, min_flow});
            }
            return min_flow;
        }
    }
    return 0;
}

int main() {
    int height = 0;
    int width = 0;
    std::cin >> height >> width;
    IGraphList graph(height * width + 2, ORIENTED);
    int64_t max_flow = 0;
    int64_t start_sum = 0;
    int64_t target_sum = 0;
    std::vector<bool> used(height * width + 2, false);
    Init(graph, height, width, start_sum, target_sum);

    while (TryToBuild(graph, used, max_flow)) {
        used.assign(height * width + 2, false);
    }
    (max_flow == start_sum) && (max_flow == target_sum) && (max_flow != 0) ? std::cout << "Valid\n"
                                                                           : std::cout << "Invalid\n";
}