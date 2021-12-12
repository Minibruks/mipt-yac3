#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>

using VertexT = size_t;
using WeightT = size_t;

enum { NO_WEIGHT = 0 };
enum { FIRST_QUERY = 1, SECOND_QUERY = 2, NO_QUERY = 0 };

template <typename T>
class FunctorSum {
public:
    T operator()(const T& first, const T& second) const {
        return first + second;
    }
};

template <typename ElemT, typename CompValueT, typename Functor>
class UnionSet {
    std::vector<ElemT> parent_;
    std::vector<CompValueT> comp_values_;
    Functor functor_;

public:
    const size_t MAX_SIZE = 1000000;

    UnionSet() {
        parent_.resize(MAX_SIZE);
        comp_values_.resize(MAX_SIZE);
    }

    explicit UnionSet(size_t set_size) {
        parent_.resize(MAX_SIZE);
        comp_values_.resize(MAX_SIZE);

        for (size_t i = 0; i < set_size; i++) {
            MakeSet(i, NO_WEIGHT);
        }
    }

    void MakeSet(const ElemT elem, const CompValueT comp_value = 0) {
        parent_[elem] = elem;
        comp_values_[elem] = comp_value;
    }

    ElemT FindSet(const ElemT elem) {
        if (parent_[elem] == elem) {
            return elem;
        }
        return parent_[elem] = FindSet(parent_[elem]);
    }

    void UnionSets(ElemT first_leader, ElemT sec_leader, const CompValueT comp_value = 0) {
        first_leader = FindSet(first_leader);
        sec_leader = FindSet(sec_leader);
        if (first_leader != sec_leader) {
            if (comp_values_[first_leader] < comp_values_[sec_leader]) {
                std::swap(first_leader, sec_leader);
            }
            parent_[sec_leader] = first_leader;
            comp_values_[sec_leader] = functor_(comp_values_[sec_leader], comp_value);
            comp_values_[first_leader] = functor_(comp_values_[first_leader], comp_values_[sec_leader]);
        } else {
            comp_values_[first_leader] = functor_(comp_values_[first_leader], comp_value);
        }
    }

    CompValueT GetCompValue(const ElemT elem) {
        return comp_values_[FindSet(elem)];
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t vertex_num = 0;
    size_t query_num = 0;
    std::cin >> vertex_num >> query_num;

    UnionSet<VertexT, WeightT, FunctorSum<WeightT>> set(vertex_num);

    for (size_t i = 0; i < query_num; i++) {
        size_t current_query = NO_QUERY;
        std::cin >> current_query;
        if (current_query == FIRST_QUERY) {
            VertexT edge_begin = 0;
            VertexT edge_end = 0;
            WeightT weight = 0;
            std::cin >> edge_begin >> edge_end >> weight;
            set.UnionSets(edge_begin - 1, edge_end - 1, weight);
        } else if (current_query == SECOND_QUERY) {
            VertexT vertex_weight_wanted = 0;
            std::cin >> vertex_weight_wanted;
            std::cout << set.GetCompValue(vertex_weight_wanted - 1) << "\n";
        }
    }

    return 0;
}
