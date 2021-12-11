#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <stack>
#include <list>

using VertexT = size_t;
using WeightT = size_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    enum { UNDEFINED_DIST = 1000 * 1000 * 1000 * 1LL };

    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& weight_from_to) = 0;

    virtual void UpdateEdge(const VertexT& vertex_from, const VertexT& vertex_to, const WeightT& delta_weight) = 0;

    virtual size_t GetEdge(const VertexT& vertex_from, const VertexT& vertex_to) const = 0;
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<VertexT>> matrix_;

public:
    GraphMatrix() = default;

    explicit GraphMatrix(const VertexT& quantity, bool oriented_status) {
        for (VertexT i = 0; i < quantity; ++i) {
            std::vector<VertexT> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_num_ = quantity;
        oriented_ = oriented_status;
    }

    explicit GraphMatrix(const IGraph& graph) : IGraph(graph) {
        matrix_ = reinterpret_cast<const GraphMatrix*>(&graph)->matrix_;
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

    size_t GetEdge(const VertexT& vertex_from, const VertexT& vertex_to) const override {
        return matrix_[vertex_from][vertex_to];
    }
};

void DFS(IGraph& graph, const VertexT& start, std::vector<VertexT>& path, std::vector<bool>& used) {
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

size_t CurrentMinFlow(IGraph& temp_graph, std::vector<VertexT>& path, const VertexT& start, const VertexT& finish) {
    size_t flow = IGraph::UNDEFINED_DIST;

    for (VertexT current = finish; current != start; current = path[current]) {
        VertexT prev = path[current];
        flow = std::min(flow, temp_graph.GetEdge(prev, current));
    }

    return flow;
}

void UpdateFlowInEdge(IGraph& temp_graph, std::vector<VertexT>& path, size_t flow, const VertexT& start, const VertexT& finish) {
    for (VertexT current = finish; current != start; current = path[current]) {
        VertexT prev = path[current];

        temp_graph.UpdateEdge(prev, current, -flow);
        temp_graph.UpdateEdge(current, prev, flow);
    }
}

size_t FordFulkerson(IGraph& graph, const VertexT& start, const VertexT& finish) {
    GraphMatrix temp_graph(graph);

    size_t max_flow = 0;

    std::vector<bool> used(temp_graph.GetVertexNum(), false);
    std::vector<VertexT> path(graph.GetVertexNum(), IGraph::UNDEFINED_DIST);

    while (true) {
        DFS(temp_graph, start, path, used);
        if (!used[finish]) {
            break;
        }

        size_t flow = CurrentMinFlow(temp_graph, path, start, finish);
        UpdateFlowInEdge(temp_graph, path, flow, start, finish);

        max_flow += flow;

        path.assign(temp_graph.GetVertexNum(), IGraph::UNDEFINED_DIST);
        used.assign(temp_graph.GetVertexNum(), false);
    }

    return max_flow;
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
