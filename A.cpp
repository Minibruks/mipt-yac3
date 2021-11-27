#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>

class UnionSet {
    std::vector<int> parent_;
    std::vector<int> size_;

public:
    UnionSet() {
        parent_.resize(100000);
        size_.resize(100000);
    }

    void MakeSet(int elem) {
        parent_[elem] = elem;
        size_[elem] = 1;
    }

    int FindSet(int elem) {
        if (parent_[elem] == elem) {
            return elem;
        }
        return parent_[elem] = FindSet(parent_[elem]);
    }

    void UnionSets(int first_leader, int sec_leader) {
        first_leader = FindSet(first_leader);
        sec_leader = FindSet(sec_leader);
        if (first_leader != sec_leader) {
            if (size_[first_leader] < size_[sec_leader]) {
                std::swap(first_leader, sec_leader);
            }
            parent_[sec_leader] = first_leader;
            size_[first_leader] += size_[sec_leader];
        }
    }

    int GetSize(int elem) {
        return size_[elem];
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int islands = 0;
    int bridges = 0;
    std::cin >> islands >> bridges;

    if (islands == 1) {
        std::cout << 0 << "\n";
        return 0;
    }

    UnionSet set;

    for (int i = 0; i < islands; i++) {
        set.MakeSet(i);
    }

    int count = 0;

    for (int i = 0; i < bridges; i++) {
        int island1 = 0;
        int island2 = 0;
        std::cin >> island1 >> island2;
        count++;
        set.UnionSets(island1, island2);
        if (set.GetSize(set.FindSet(island1)) == islands) {
            std::cout << count << "\n";
            return 0;
        }
    }
    std::cout << count << "\n";
    return 0;
}