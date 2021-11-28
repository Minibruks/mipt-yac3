#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <stack>
#include <list>

// const int64_t kUndefinedDist = 1000 * 1000 * 1000 * 1LL;
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

    virtual std::vector<std::vector<VertexT>> GetAllAdjEdges() = 0;
};

class IGraphMatrix final : public IGraph {
private:
    std::vector<std::vector<VertexT>> matrix_;

public:
    explicit IGraphMatrix(const VertexT& quantity, bool oriented_status) {
        for (VertexT i = 0; i < quantity; ++i) {
            std::vector<VertexT> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_num_ = quantity;
        oriented_ = oriented_status;
    }

    IGraphMatrix(const IGraphMatrix& graph) : IGraph(graph) {
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

    std::vector<std::vector<VertexT>> GetAllAdjEdges() override {
        return matrix_;
    }
};

std::vector<std::vector<VertexT>> FloydWarshall(IGraph& graph) {
    std::vector<std::vector<VertexT>> matrix_for_algo = graph.GetAllAdjEdges();
    for (VertexT k = 0; k < graph.GetVertexNum(); k++) {
        for (VertexT i = 0; i < graph.GetVertexNum(); i++) {
            for (VertexT j = 0; j < graph.GetVertexNum(); j++) {
                VertexT res = std::min(matrix_for_algo[i][j], matrix_for_algo[i][k] + matrix_for_algo[k][j]);
                matrix_for_algo[i][j] = res;
            }
        }
    }

    return matrix_for_algo;
}

void PrintMatrix(std::vector<std::vector<VertexT>>& matrix) {
//    for (VertexT i = 0; i < matrix.size(); i++) {
//        for (VertexT j = 0; j < matrix[i].size(); j++) {
//            std::cout << answer[i][j] << " ";
//        }
//        std::cout << "\n";
//    }

    for (auto& matrix_string : matrix) {
        for (auto& elem : matrix_string) {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int width = 0;
    std::cin >> width;

    IGraphMatrix graph(width, ORIENTED);
    for (int vertex_i = 0; vertex_i < width; vertex_i++) {
        for (int vertex_j = 0; vertex_j < width; vertex_j++) {
            WeightT weight = 0;
            std::cin >> weight;
            graph.AddEdge(vertex_i, vertex_j, weight);
        }
    }
    std::vector<std::vector<VertexT>> answer = FloydWarshall(graph);
    PrintMatrix(answer);
}