#pragma once

#include <tr1/memory>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>


using std::vector;
using std::map;
using std::string;
using std::auto_ptr;
using std::sort;
using std::unique;

#include <iostream>
using std::cerr;
using std::endl;
using std::hex;
using std::dec;

template <typename TData>
class TTrieNode {
public:
    typedef map<char, TTrieNode<TData> > TChildrenMap;

    TTrieNode()
        : data_ptr_(NULL) {
    }
    
    TTrieNode(const TTrieNode<TData>& rhs)
        : children_(rhs.children_) 
        , data_ptr_(auto_ptr<TData>(rhs.data_ptr_.get() ? new TData(*rhs.data_ptr_) : NULL)) {
    }

    TTrieNode<TData>* add_child(char symbol) {
        return &(children_[symbol]);
    }

    const TTrieNode* find(char symbol) const {
        typedef typename map<char, TTrieNode<TData> >::const_iterator TChildIterator;
        TChildIterator child_iterator = children_.find(symbol);
        if (child_iterator == children_.end()) {
            return NULL;
        } else {
            return &(child_iterator->second);
        }
    }
    TTrieNode* find(char symbol) {
        typedef typename map<char, TTrieNode<TData> >::iterator TChildIterator;
        TChildIterator child_iterator = children_.find(symbol);
        if (child_iterator == children_.end()) {
            return NULL;
        } else {
            return &(child_iterator->second);
        }
    }

    bool is_acceptable() const {
        return (data_ptr_.get() != NULL);
    }

    void make_acceptable(const TData& data) {
        data_ptr_ = auto_ptr<TData>(new TData(data));
    }

    const TChildrenMap& get_children() const {
        return children_;
    }

    TData& get_data() {
        return *data_ptr_;
    }
    const TData& get_data() const {
        return *data_ptr_;
    }

private:
    TChildrenMap children_;
    auto_ptr<TData> data_ptr_;
};

template <typename TData>
struct TFrontState {
    TFrontState()
        : index_(0)
        , cost_(0)
        , node_ptr_(NULL) {
    }

    TFrontState(int index, int cost, const TTrieNode<TData>* node_ptr) 
        : index_(index)
        , cost_(cost)
        , node_ptr_(node_ptr) {
    }


    int index_;
    int cost_;
    const TTrieNode<TData>* node_ptr_;
};

template <typename TData>
bool operator == (const TFrontState<TData>& lhs, const TFrontState<TData>& rhs) {
    return lhs.index_ == rhs.index_ && lhs.cost_ == rhs.cost_ && lhs.node_ptr_ == rhs.node_ptr_;
}


template <typename TData>
class TTrie {
    typedef TFrontState<TData> TState;
public:
  
    const TData& find(const string& text) const {
        static const TData bad_data = TData();
        const TTrieNode<TData>* current_node_ptr = &root_;
        for (size_t index = 0; index < text.size(); ++index) {
            current_node_ptr = current_node_ptr->find(text[index]);
            if (current_node_ptr == NULL) {
                assert(false);
                return bad_data;
                //return NULL;
            }
        }
        if (!current_node_ptr->is_acceptable()) {
            assert(false);
            return bad_data;
        }
        return current_node_ptr->get_data();
        //return current_node_ptr; 
    }

    template <typename TOutIterator>
    void find_nearby(const string& text, TOutIterator out,int max_distance = 0) const {
        vector<TState> front;
        front.push_back(TState(0, 0, &root_));
        
        vector<TData> found_nodes;

        while(!front.empty()) {
            vector<TState> new_front;
            while(!front.empty()) {
                const TState& state = front.back();
                if (state.node_ptr_->is_acceptable() && state.index_ == text.size()) {
                    found_nodes.push_back(state.node_ptr_->get_data());
                }
                
                if (state.cost_ + 1 <= max_distance) {
                    new_front.push_back(TState(state.index_ + 1, state.cost_ + 1, state.node_ptr_));
                }
                
                if (state.index_ < text.size()) {
                    const typename TTrieNode<TData>::TChildrenMap& children = state.node_ptr_->get_children();
                    typedef typename TTrieNode<TData>::TChildrenMap::const_iterator TIt;
                    for (TIt it = children.begin(); it != children.end(); ++it) {
                        int delta_cost = it->first == text[state.index_] ? 0 : 1;
                        if (state.cost_ + delta_cost <= max_distance) { 
                            new_front.push_back(TState(state.index_ + 1, state.cost_ + delta_cost, &(it->second)));
                        }
                    }
                }
               
                vector<TState> burning_front;
                if (state.cost_ + 1 <= max_distance) {
                    const typename TTrieNode<TData>::TChildrenMap& children = state.node_ptr_->get_children();
                    typedef typename TTrieNode<TData>::TChildrenMap::const_iterator TIt;
                    for (TIt it = children.begin(); it != children.end(); ++it) {
                        burning_front.push_back(TState(state.index_, state.cost_ + 1, &(it->second)));
                    }
                }

                front.pop_back();
                front.insert(front.end(), burning_front.begin(), burning_front.end());
            }

            //remove duclicates
            /*
            sort(new_front.begin(), new_front.end());
            new_front.erase(
                unique(new_front.begin(), new_front.end()),
                new_front.end());
            */
            front.swap(new_front);
        }

        sort(found_nodes.begin(), found_nodes.end());
        typename vector<TData>::const_iterator
            unique_end = unique(found_nodes.begin(), found_nodes.end()),
            unique_it = found_nodes.begin();
        for (; unique_it != unique_end; ++unique_it) {
            *out = *unique_it;
            ++out;            
        }
    }

    void add(const string& text, const TData& data) {
        TTrieNode<TData>* current_node_ptr = &root_;
        for (size_t index = 0; index < text.size(); ++index) {
            TTrieNode<TData>* next_node_ptr = current_node_ptr->find(text[index]);
            if (next_node_ptr == NULL) {
                next_node_ptr = current_node_ptr->add_child(text[index]);
            }
            current_node_ptr = next_node_ptr;
        }
        current_node_ptr->make_acceptable(data);
    }

    void load() {
    }

    void save()const {
    }

private:
    TTrieNode<TData> root_;
};
