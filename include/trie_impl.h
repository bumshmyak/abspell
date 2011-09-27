#ifdef TRIEE_IMPL_H
/*---------------------TTrieNode<TData> implementation-----------------------*/
    
template <typename TData>
void TTrieNode<TData>::add_child(char symbol, size_t child_node_id) {
  typedef typename map<char, size_t>::iterator TChildIterator;
  TChildIterator child_iterator = children_.find(symbol);
  if (child_iterator == children_.end()) {
    children_[symbol] = child_node_id;
    if (depth_ == 0) {
      depth_ = 1;
    }
  } else {
    return;
  }
}

template <typename TData>
size_t TTrieNode<TData>::find(char symbol) const {
  typedef typename map<char, size_t>::const_iterator TChildIterator;
  TChildIterator child_iterator = children_.find(symbol);
  if (child_iterator == children_.end()) {
    return 0;
  } else {
    return child_iterator->second;
  }
}

template <typename TData>
bool TTrieNode<TData>::is_acceptable() const {
    return (data_ptr_.get() != NULL);
}

template <typename TData>
void TTrieNode<TData>::make_acceptable(const TData& data) {
    data_ptr_ = auto_ptr<TData>(new TData(data));
}

template <typename TData>
const typename TTrieNode<TData>::TChildrenMap& TTrieNode<TData>::get_children() const {
    return children_;
}

template <typename TData>
TData& TTrieNode<TData>::get_data() {
    return *data_ptr_;
}

template <typename TData>
const TData& TTrieNode<TData>::get_data() const {
    return *data_ptr_;
}

template <typename TData>
void TTrieNode<TData>::set_depth(size_t depth) {
  depth_ = depth;
}

template <typename TData>
size_t TTrieNode<TData>::get_depth() const {
  return depth_;
}

/*---------------------Comparison operators for TFrontState<TData>-----------*/

template <typename TData>
bool operator == (const TFrontState<TData>& lhs, const TFrontState<TData>& rhs) {
  return lhs.index_ == rhs.index_ && lhs.cost_ == rhs.cost_ && lhs.node_id_ == rhs.node_id_;
}

template <typename TData>
bool operator < (const TFrontState<TData>& lhs, const TFrontState<TData>& rhs) {
  return 
      (lhs.index_ < rhs.index_) ||
      (lhs.index_ == rhs.index_ && lhs.cost_ < rhs.cost_) ||
      (lhs.index_ == rhs.index_ && lhs.cost_ == rhs.cost_ && lhs.node_id_ < rhs.node_id_);
}

/*------------------------------TTrie<TData> implementation------------------*/
template <typename TData>
const TData& TTrie<TData>::find(const string& text) const {
  static const TData bad_data = TData();
  size_t current_node_id = 0;
  for (size_t index = 0; index < text.size(); ++index) {
    current_node_id = node_storage_[current_node_id].find(text[index]);
    if (current_node_id == 0) {
      assert(false);
      return bad_data;
    }
  }
  if (!node_storage_[current_node_id].is_acceptable()) {
    assert(false);
    return bad_data;
  }
  return node_storage_[current_node_id].get_data();
}

template <typename TData>
template <typename TOutIterator>
void TTrie<TData>::find_nearby(
    const string& text,
    TOutIterator out,
    int max_distance) const {
  //set<TState> front;
  vector<TState> front;
  //front.insert(TState(0, 0, 0));
  front.push_back(TState(0, 0, 0));
  
  vector<TData> found_nodes;

  while(!front.empty()) {
    //set<TState> new_front;
    vector<TState> new_front;
    while(!front.empty()) {
      const TState& state = front.back();
      //const TState& state = *front.begin();
      if (node_storage_[state.node_id_].is_acceptable() && state.index_ == text.size()) {
        found_nodes.push_back(node_storage_[state.node_id_].get_data());
      }
    
      // stop traversing is there is no chance to reach accepted nodes withing given distance
      if (node_storage_[state.node_id_].get_depth() + max_distance < text.size() - state.index_) {
        front.pop_back();
        continue;
      }

      if (state.cost_ + 1 <= max_distance) {
        new_front.push_back(TState(state.index_ + 1, state.cost_ + 1, state.node_id_));
        //new_front.insert(TState(state.index_ + 1, state.cost_ + 1, state.node_id_));
      }
      
      if (state.index_ < text.size()) {
        const typename TTrieNode<TData>::TChildrenMap& children =
            node_storage_[state.node_id_].get_children();
        typedef typename TTrieNode<TData>::TChildrenMap::const_iterator TIt;
        for (TIt it = children.begin(); it != children.end(); ++it) {
          int delta_cost = it->first == text[state.index_] ? 0 : 1;
          if (state.cost_ + delta_cost <= max_distance) { 
            new_front.push_back(TState(state.index_ + 1, state.cost_ + delta_cost, it->second));
            //new_front.insert(TState(state.index_ + 1, state.cost_ + delta_cost, it->second));
          }
        }
      }
     
      //set<TState> burning_front;
      vector<TState> burning_front;
      if (state.cost_ + 1 <= max_distance) {
        const typename TTrieNode<TData>::TChildrenMap& children =
            node_storage_[state.node_id_].get_children();
        typedef typename TTrieNode<TData>::TChildrenMap::const_iterator TIt;
        for (TIt it = children.begin(); it != children.end(); ++it) {
          burning_front.push_back(TState(state.index_, state.cost_ + 1, it->second));
          //burning_front.insert(TState(state.index_, state.cost_ + 1, it->second));
        }
      }

      front.pop_back();
      //front.erase(front.begin());
      front.insert(front.end(), burning_front.begin(), burning_front.end());
      //front.insert(burning_front.begin(), burning_front.end());
    }

    //remove duclicates
    sort(new_front.begin(), new_front.end());
    new_front.erase(
        unique(new_front.begin(), new_front.end()),
        new_front.end());
    
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

// returns true if object added
// false if it already exists
template <typename TData>
bool TTrie<TData>::add(const string& text, const TData& data) {
    size_t current_node_id = 0;
    for (size_t index = 0; index < text.size(); ++index) {
      node_storage_[current_node_id].set_depth(
          max(text.size() - index, node_storage_[current_node_id].get_depth()));

      size_t next_node_id = node_storage_[current_node_id].find(text[index]);
      if (next_node_id == 0) {
        node_storage_.push_back(TTrieNode<TData>());
        next_node_id = node_storage_.size() - 1;
        node_storage_[current_node_id].add_child(text[index], next_node_id);
      }
      current_node_id = next_node_id;
    }
    if (node_storage_[current_node_id].is_acceptable()) {
      return false;
    }
    node_storage_[current_node_id].make_acceptable(data);
    return true;
}

#endif //TRIEE_IMPL_H
