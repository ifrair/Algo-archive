// Segment tree template.
// Range queries add, set, get_sum, get_max, get_min
// uses 0 indexation

#pragma once

#include <iostream>
#include <numeric>
#include <vector>

template<class T>
class SegmentTree {
    struct MaxInfo {
        T max = std::numeric_limits<T>().min();
        size_t max_idx = 0;
        size_t num_maxes = 0;
    };
    struct MinInfo {
        T min = std::numeric_limits<T>().max();
        size_t min_idx = 0;
        size_t num_mins = 0;
    };
    struct Node {
        T sum = 0;
        T add = 0;
        T set = std::numeric_limits<T>().min();
        MaxInfo max_info;
        MinInfo min_info;
    };
public:
    SegmentTree() = default;
    SegmentTree(const std::vector<T>& arr): sz(arr.size()), tree(arr.size() * 4) {
        build(0, 0, sz - 1, arr);
    }
    void build(const std::vector<T>& arr) {
        sz = arr.size();
        tree.resize(sz * 4);
        build(0, 0, sz - 1, arr);
    }
      
    void add(size_t l_idx, size_t r_idx, T delta) {
        add(0, 0, sz - 1, l_idx, r_idx, delta);
    }
    void set(size_t l_idx, size_t r_idx, T value) {
        set(0, 0, sz - 1, l_idx, r_idx, value);
    }
      
    T get_sum(size_t l_idx, size_t r_idx) {
        return get_sum(0, 0, sz - 1, l_idx, r_idx);
    }
    MaxInfo get_max(size_t l_idx, size_t r_idx) {
        return get_max(0, 0, sz - 1, l_idx, r_idx);
    }
    MinInfo get_min(size_t l_idx, size_t r_idx) {
        return get_min(0, 0, sz - 1, l_idx, r_idx);
    }
      
private:
    void build(size_t node, size_t l, size_t r, const std::vector<T>& arr) {
        if (l == r) {
            tree[node] = {arr[l], 0, arr[l], {arr[l], l, 1}, {arr[l], l, 1}};
            return;
        }
        size_t t = (l + r) / 2;
        build(node * 2 + 1, l, t, arr);
        build(node * 2 + 2, t + 1, r, arr);
        update_node(node);
    }
    void add(size_t node, size_t l, size_t r, size_t l_idx, size_t r_idx, T delta) {
        if (l > r_idx || r < l_idx) return;
        push(node, l, r);
        if (l >= l_idx && r <= r_idx) {
            add_node(node, delta, l, r);
            return;
        }
        size_t t = (l + r) / 2;
        add(node * 2 + 1, l, t, l_idx, r_idx, delta);
        add(node * 2 + 2, t + 1, r, l_idx, r_idx, delta);
        update_node(node);
    }
    void set(size_t node, size_t l, size_t r, size_t l_idx, size_t r_idx, T value) {
        if (l > r_idx || r < l_idx) return;
        push(node, l, r);
        if (l >= l_idx && r <= r_idx) {
            set_node(node, value, l, r);
            return;
        }
        size_t t = (l + r) / 2;
        set(node * 2 + 1, l, t, l_idx, r_idx, value);
        set(node * 2 + 2, t + 1, r, l_idx, r_idx, value);
        update_node(node);
    }
    T get_sum(size_t node, size_t l, size_t r, size_t l_idx, size_t r_idx) {
        if (l > r_idx || r < l_idx) return 0;
        push(node, l, r);
        if (l >= l_idx && r <= r_idx) {
            return tree[node].sum;
        }
        size_t t = (l + r) / 2;
        return get_sum(node * 2 + 1, l, t, l_idx, r_idx) +
            get_sum(node * 2 + 2, t + 1, r, l_idx, r_idx);
    }
    MaxInfo get_max(size_t node, size_t l, size_t r, size_t l_idx, size_t r_idx) {
        if (l > r_idx || r < l_idx) return MaxInfo();
        push(node, l, r);
        if (l >= l_idx && r <= r_idx) {
            return tree[node].max_info;
        }
        size_t t = (l + r) / 2;
        auto maxl = get_max(node * 2 + 1, l, t, l_idx, r_idx);
        auto maxr = get_max(node * 2 + 2, t + 1, r, l_idx, r_idx);
        return get_max_info(maxl, maxr);
    }
      
    MinInfo get_min(size_t node, size_t l, size_t r, size_t l_idx, size_t r_idx) {
        if (l > r_idx || r < l_idx) return MinInfo();
        push(node, l, r);
        if (l >= l_idx && r <= r_idx) {
            return tree[node].min_info;
        }
        size_t t = (l + r) / 2;
        auto minl = get_min(node * 2 + 1, l, t, l_idx, r_idx);
        auto minr = get_min(node * 2 + 2, t + 1, r, l_idx, r_idx);
        return get_min_info(minl, minr);
    }
      
    MaxInfo get_max_info(MaxInfo& f, MaxInfo& s) {
        auto res = (f.max < s.max ? s: f);
        res.num_maxes = (
            f.max == s.max ?
            f.num_maxes + s.num_maxes :
            res.num_maxes
        );
        return res;
    }
    MinInfo get_min_info(MinInfo& f, MinInfo& s) {
        auto res = (f.min > s.min ? s: f);
        res.num_mins = (
            f.min == s.min ?
            f.num_mins + s.num_mins :
            res.num_mins
        );
        return res;
    }
    void update_node(size_t node) {
        tree[node].sum = tree[node * 2 + 1].sum + tree[node * 2 + 2].sum;
          
        tree[node].max_info = get_max_info(
           tree[node * 2 + 1].max_info,
           tree[node * 2 + 2].max_info
        );
          
        tree[node].min_info = get_min_info(
           tree[node * 2 + 1].min_info,
           tree[node * 2 + 2].min_info
        );
    }
    void add_node(size_t node, T delta, size_t l, size_t r) {
        if (tree[node].set != Node().set) {
            tree[node].set += delta;
        } else {
            tree[node].add += delta;
        }
        tree[node].sum += delta * (r - l + 1);
        tree[node].max_info.max += delta;
        tree[node].min_info.min += delta;
    }
    void set_node(size_t node, T value, size_t l, size_t r) {
        tree[node] = {
            value * (T)(r - l + 1),
            0,
            value,
            {value, l, r - l + 1},
            {value, l, r - l + 1}
        };
    }
    void push(size_t node, size_t l, size_t r) {
        if (l == r) {
            tree[node].set = Node().set;
            tree[node].add = 0;
            return;
        }
        size_t t = (l + r) / 2;
        if (tree[node].set != Node().set) {
            set_node(node * 2 + 1, tree[node].set, l, t);
            set_node(node * 2 + 2, tree[node].set, t + 1, r);
            tree[node].set = Node().set;
        } else if (tree[node].add != 0){
            add_node(node * 2 + 1, tree[node].add, l, t);
            add_node(node * 2 + 2, tree[node].add, t + 1, r);
            tree[node].add = 0;
        }
    }
      
private:
    size_t sz;
    std::vector<Node> tree;
};

//int main() {
//    SegmentTree<int> st({0, 1, 2, 3, 4, 5});
//    st.add(2, 2, 4); // {0, 1, 6, 3, 4, 5}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    st.add(1, 1, 5); // {0, 6, 6, 3, 4, 5}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    st.set(0, 1, 2); // {2, 2, 6, 3, 4, 5}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    st.add(1, 5, 2); // {2, 4, 8, 5, 6, 7}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    st.set(1, 1, 8); // {2, 8, 8, 5, 6, 7}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    st.add(4, 5, 3); // {2, 8, 8, 5, 9, 10}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    st.add(3, 3, 3); // {2, 8, 8, 5, 9, 10}
//    std::cout << st.get_sum(2, 4) << std::endl;
//    
//    
//    std::cout << st.get_max(0, 3).max << ' ' << st.get_max(0, 3).max_idx << ' ' << st.get_max(0, 3).num_maxes << std::endl;
//    std::cout << st.get_min(2, 5).min << ' ' << st.get_min(2, 5).min_idx << ' ' << st.get_min(2, 5).num_mins << std::endl;
//}

