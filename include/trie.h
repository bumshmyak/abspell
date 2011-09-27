#pragma once

#include <using_std.h>

#include <tr1/memory>
#include <cassert>

using std::auto_ptr;

template <typename TData>
class TTrieNode {
public:
    typedef map<char, size_t> TChildrenMap;

    TTrieNode()
        : data_ptr_(NULL)
        , depth_(0) {
    }
    
    TTrieNode(const TTrieNode<TData>& rhs)
        : children_(rhs.children_)
        , data_ptr_(auto_ptr<TData>(rhs.data_ptr_.get() ? new TData(*rhs.data_ptr_) : NULL))
        , depth_(rhs.depth_) { 
    }

    void add_child(char symbol, size_t child_node_id);

    size_t find(char symbol) const;

    bool is_acceptable() const;

    void make_acceptable(const TData& data);

    const TChildrenMap& get_children() const;

    TData& get_data();

    const TData& get_data() const;

    void set_depth(size_t depth);

    size_t get_depth() const;

private:
    TChildrenMap children_;
    auto_ptr<TData> data_ptr_;
    size_t depth_;
};


template <typename TData>
struct TFrontState {
    TFrontState()
        : index_(0)
        , cost_(0)
        , node_id_(0) {
    }

    TFrontState(int index, int cost, size_t node_id) 
        : index_(index)
        , cost_(cost)
        , node_id_(node_id) {
    }


    int index_;
    int cost_;
    size_t node_id_;
};


template <typename TData>
bool operator == (const TFrontState<TData>& lhs, const TFrontState<TData>& rhs);

template <typename TData>
bool operator < (const TFrontState<TData>& lhs, const TFrontState<TData>& rhs);


template <typename TData>
class TTrie {
    typedef TFrontState<TData> TState;
public:
    TTrie()
        : node_storage_(1) {
    }

    const TData& find(const string& text) const;

    template <typename TOutIterator>
    void find_nearby(const string& text, TOutIterator out,int max_distance = 0) const;

    // returns true if object added
    // false if it already exists
    bool add(const string& text, const TData& data);

    // void load(isstream& is);

    // void save(ostream &os) const;

private:
    vector< TTrieNode<TData> > node_storage_;
};

#define TRIEE_IMPL_H
#include <trie_impl.h>
#undef TRIEE_IMPL_H
