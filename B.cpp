#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>

const int64_t kMaxSize = 100000;
using SetT = int;

enum {NO_WEIGHT = 0};
enum {FIRST_QUERY = 1, SECOND_QUERY = 2, NO_QUERY = 0};

class UnionSet {
    std::vector<SetT> parent_;
    std::vector<SetT> weight_;

public:
    UnionSet() {
        parent_.resize(kMaxSize);
        weight_.resize(kMaxSize);
    }

    UnionSet(SetT set_size) {
        parent_.resize(kMaxSize);
        weight_.resize(kMaxSize);

        for (SetT i = 0; i < set_size; i++) {
            MakeSet(i, NO_WEIGHT);
        }
    }

    void MakeSet(SetT elem, SetT weight) {
        parent_[elem] = elem;
        weight_[elem] = weight;
    }

    SetT FindSet(SetT elem) {
        if (parent_[elem] == elem) {
            return elem;
        }
        return parent_[elem] = FindSet(parent_[elem]);
    }

    void UnionSets(SetT first_leader, SetT sec_leader, SetT weight) {
        first_leader = FindSet(first_leader);
        sec_leader = FindSet(sec_leader);
        if (first_leader != sec_leader) {
            if (weight_[first_leader] < weight_[sec_leader]) {
                std::swap(first_leader, sec_leader);
            }
            parent_[sec_leader] = first_leader;
            weight_[first_leader] += weight_[sec_leader] + weight;
        } else {
            weight_[first_leader] += weight;
        }
    }

    SetT GetWeight(SetT elem) {
        return weight_[FindSet(elem)];
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    SetT vertex_num = 0;
    SetT query_num = 0;
    std::cin >> vertex_num >> query_num;

    UnionSet set(vertex_num);

    for (int i = 0; i < query_num; i++) {
        int current_query = NO_QUERY;
        std::cin >> current_query;
        if (current_query == FIRST_QUERY) {
            SetT edge_begin = 0;
            SetT edge_end = 0;
            SetT weight = 0;
            std::cin >> edge_begin >> edge_end >> weight;
            set.UnionSets(edge_begin - 1, edge_end - 1, weight);
        } else if (current_query == SECOND_QUERY) {
            SetT edge_wanted = 0;
            std::cin >> edge_wanted;
            std::cout << set.GetWeight(edge_wanted - 1) << "\n";
        }
    }

    return 0;
}