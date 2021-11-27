#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>

const int64_t kBigNum = INT32_MAX;

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

    [[nodiscard]] virtual std::vector<int64_t>& GetNeighbors(const int64_t& v) = 0;

    virtual void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;

    virtual int64_t GetEdge(int64_t source, int64_t dest) const = 0;

    virtual void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) = 0;
};

class IGraphList : public IGraph {
private:
    std::vector<std::vector<int64_t>> adjacency_list_;

public:
    explicit IGraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<int64_t>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<int64_t>& GetNeighbors(const int64_t& v) override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        adjacency_list_[source].emplace_back(dest + weight);
    }
};

class IGraphMatrix : public IGraph {
private:
    std::vector<std::vector<int64_t>> matrix_;

public:
    explicit IGraphMatrix(const int64_t& quantity) {
        for (int64_t i = 0; i < quantity; ++i) {
            std::vector<int64_t> tmp(quantity, kBigNum);
            tmp[i] = 0;
            matrix_.push_back(tmp);
        }
        vertex_degree = quantity;
    }

    [[nodiscard]] std::vector<int64_t>& GetNeighbors(const int64_t& v) override {
        return matrix_[v];
    }

    void AddEdge(const int64_t& a, const int64_t& b, const int64_t& weight) override {
        if (a == b) {
            return;
        }
        matrix_[a][b] = std::min(matrix_[a][b], -weight);
    }

    int64_t GetEdge(int64_t source, int64_t dest) const override {
        return matrix_[source][dest];
    }

    void SetEdge(const int64_t& source, const int64_t& dest, const int64_t& weight) override {
        matrix_[source][dest] = weight;
    }
};

void Solve(IGraph& graph, std::vector<std::vector<int64_t>>& from_to) {
    for (int64_t k = 0; k < graph.GetVertexDegree(); k++) {
        for (int64_t i = 0; i < graph.GetVertexDegree(); i++) {
            for (int64_t j = 0; j < graph.GetVertexDegree(); j++) {
                if (graph.GetEdge(i, k) + graph.GetEdge(k, j) < graph.GetEdge(i, j)) {
                    from_to[i][j] = from_to[i][k];
                }
                int64_t res = std::min(graph.GetEdge(i, j), graph.GetEdge(i, k) + graph.GetEdge(k, j));
                graph.SetEdge(i, j, res);
            }
        }
    }
}

void BuildPath(IGraphMatrix& graph, int64_t& start, int64_t& end, std::vector<std::vector<int64_t>>& from_to,
               std::vector<int64_t>& answer, bool& is_cycle) {
    if (from_to[start][end] == -1) {
        return;
    }
    while (start != end) {
        if (graph.GetEdge(start, start) < 0) {
            is_cycle = true;
            return;
        }
        start = from_to[start][end];
        answer.emplace_back(start);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int64_t n = 0;
    int64_t m = 0;
    int64_t k = 0;
    std::cin >> n >> m >> k;

    IGraphMatrix graph(n);
    std::map<std::pair<int64_t, int64_t>, std::pair<int64_t, int64_t>> edges;
    std::vector<int64_t> path;
    std::vector<std::vector<int64_t>> from_to(n,std::vector<int64_t>(n, -1));
    for (int i = 0; i < m; i++) {
        int64_t x = 0;
        int64_t y = 0;
        int64_t weight = 0;
        std::cin >> x >> y >> weight;
        // if (-weight < graph.GetEdge(x - 1, y - 1)) {
        graph.AddEdge(x - 1, y - 1, weight);
        // }

        if (edges.count({x - 1, y - 1})) {
            if (weight > edges[{x - 1, y - 1}].first) {
                edges[{x - 1, y - 1}] = {weight, i};
                from_to[x - 1][y - 1] = y - 1;
            }
        } else {
            edges[{x - 1, y - 1}] = {weight, i};
            from_to[x - 1][y - 1] = y - 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << graph.GetEdge(i, j) << " ";
        }
        std::cout << "\n";
    }

    std::vector<int64_t> cities_with_concerts(k, 0);
    for (int32_t i = 0; i < k; ++i) {
        int64_t city = 0;
        std::cin >> city;
        cities_with_concerts[i] = city - 1;
    }

    path.emplace_back(cities_with_concerts[0]);
    std::vector<int64_t> answer;
    bool is_cycle = false;
    Solve(graph, from_to);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << graph.GetEdge(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << from_to[i][j] << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < k - 1; i++) {
        BuildPath(graph, cities_with_concerts[i], cities_with_concerts[i + 1], from_to, path, is_cycle);
        if (is_cycle) {
            std::cout << "infinitely kind\n";
            return 0;
        }
    }

    for (size_t i = 0; i < path.size() - 1; ++i) {
        answer.push_back(edges[{path[i], path[i + 1]}].second + 1);
    }
    std::cout << answer.size() << "\n";
    for (const auto& item : answer) {
        std::cout << item << " ";
    }

    return 0;
}