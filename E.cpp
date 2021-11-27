#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <stack>
#include <list>

const int64_t kUndefinedDist = 1000 * 1000 * 1000 * 1LL;
using VertexT = int32_t;
using WeightT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight_from_to) = 0;

    virtual void UpdateEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& delta_weight) = 0;

    virtual VertexT GetVertex(const VertexT& vertex_from, const VertexT& vertex_to) const = 0;
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<VertexT>> matrix_;

public:
    explicit GraphMatrix(const VertexT& quantity, bool oriented_status) {
        for (VertexT i = 0; i < quantity; ++i) {
            std::vector<VertexT> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_num_ = quantity;
        oriented_ = oriented_status;
    }

    GraphMatrix(const GraphMatrix& graph) : IGraph(graph) {
        matrix_ = graph.matrix_;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const override {
        return matrix_[current_vertex];
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight_from_to) override {
        matrix_[vertex_from][vertex_to] = weight_from_to;
        if (!oriented_) {
            matrix_[vertex_to][vertex_from] = weight_from_to;
        }
    }

    void UpdateEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& delta_weight) override {
        matrix_[vertex_from][vertex_to] += delta_weight;
    }

    VertexT GetVertex(const VertexT& vertex_from, const VertexT& vertex_to) const override {
        return matrix_[vertex_from][vertex_to];
    }
};

void DFS(GraphMatrix& graph, const VertexT& start, std::vector<VertexT>& path, std::vector<bool>& used) {
    std::stack<VertexT> list_for_dfs;
    list_for_dfs.push(start);
    used[start] = true;

    while (!list_for_dfs.empty()) {
        VertexT current_vertex = list_for_dfs.top();
        list_for_dfs.pop();
        auto neighbors = graph.GetNeighbors(current_vertex);
        for (VertexT neighbor = 0; neighbor < graph.GetVertexNum(); ++neighbor) {
            if (neighbors[neighbor] > 0 && neighbor != path[current_vertex]) {
                if (!used[neighbor]) {
                    list_for_dfs.push(neighbor);
                    path[neighbor] = current_vertex;
                    used[neighbor] = true;
                }
            }
        }
    }
}

int FordFulkerson(GraphMatrix& graph, const VertexT& start, const VertexT& finish) {
    GraphMatrix graph_for_algo = graph;

    int answer = 0;

    std::vector<bool> used(graph_for_algo.GetVertexNum(), false);
    std::vector<VertexT> path(graph.GetVertexNum(), kUndefinedDist);

    while (true) {
        DFS(graph_for_algo, start, path, used);
        if (!used[finish]) {
            break;
        }

        WeightT flow = kUndefinedDist;

        for (VertexT current = finish; current != start; current = path[current]) {
            VertexT prev = path[current];
            flow = std::min(flow, graph_for_algo.GetVertex(prev, current));
        }

        for (VertexT current = finish; current != start; current = path[current]) {
            VertexT prev = path[current];

            graph_for_algo.UpdateEdge(prev, current, -flow);
            graph_for_algo.UpdateEdge(current, prev, flow);
        }

        answer += flow;

        path.assign(graph_for_algo.GetVertexNum(), kUndefinedDist);
        used.assign(graph_for_algo.GetVertexNum(), false);
    }

    return answer;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    VertexT vertex_num = 0;
    VertexT edge_num = 0;
    std::cin >> vertex_num >> edge_num;

    GraphMatrix graph(vertex_num, ORIENTED);

    for (VertexT i = 0; i < edge_num; ++i) {
        VertexT vertex_from = 0;
        VertexT vertex_to = 0;
        WeightT weight = 0;
        std::cin >> vertex_from >> vertex_to >> weight;
        graph.AddEdge(vertex_from - 1, vertex_to - 1, weight);
    }

    std::cout << FordFulkerson(graph, 0, vertex_num - 1);
}