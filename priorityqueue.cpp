#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size)
    : nodes_(max_size + 1, KeyValuePair()), max_size_(max_size), size_(0) {
} // creates priority queue object

KeyValuePair PriorityQueue::min() {
  // Return the root of the heap, which is the minimum element in the priority
  // queue
  return nodes_[ROOT];
}

KeyValuePair PriorityQueue::removeMin() {
  // If the priority queue is empty, return an empty pair
  if (isEmpty()) {
    return std::make_pair(Key(), Value());
  }

  // Store the root of the heap, which is the minimum element in the priority
  // queue
  KeyValuePair min = nodes_[ROOT];

  // Replace the root of the heap with the last element in the priority queue
  nodes_[ROOT] = nodes_[size_];
  size_--;

  // Heapify down the root node to restore the min-heap property
  heapifyDown(ROOT);

  // Return the minimum element in the priority queue
  return min;
}

void PriorityQueue::insert(KeyValuePair kv) {
  // Add the node to the heap and increase the size of the heap
  size_++;
  if (size_ > max_size_) {
    return;
  }
  nodes_[size_] = kv;
  nlohmann::json result_one;
  heapifyUp(size_);
}

bool PriorityQueue::isEmpty() const {
  // checks if the heap is empty and returns as a boolean
  return size_ == 0;
}

size_t PriorityQueue::size() const { return size_; }



void PriorityQueue::heapifyUp(size_t i) {
    while (i > ROOT) {
        size_t parent = i / 2;
        if (nodes_[i].first < nodes_[parent].first) {
            std::swap(nodes_[i], nodes_[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

void PriorityQueue::heapifyDown(size_t i) {
    while (2 * i <= size_) {
        size_t left_child = 2 * i;
        size_t right_child = left_child + 1;
        size_t min_child = left_child;
        if (right_child <= size_ && nodes_[right_child].first < nodes_[left_child].first) {
            min_child = right_child;
        }
        if (nodes_[min_child].first < nodes_[i].first) {
            std::swap(nodes_[i], nodes_[min_child]);
            i = min_child;
        } else {
            break;
        }
    }
}


void PriorityQueue::removeNode(size_t i) {
  // TODO: complete this function
  // Removes the ith node from the heap
  if (i < ROOT || i >= size_) { // invalid index
    return;
  }
  nodes_[i] = nodes_[size_ - 1]; // replace node with previous
  size_--;                       // decrease heap size

  int parent = (i - ROOT) / 2 + ROOT;
  if (i != ROOT && getKey(i) < getKey(parent)) {
    heapifyUp(i);
  } else {
    heapifyDown(i);
  }
}

Key PriorityQueue::getKey(size_t i) {
  // Return the key of the node at the given index
  return nodes_[i].first;
}

nlohmann::json PriorityQueue::JSON() const {
  nlohmann::json result;

  for (size_t i = 1; i <= size_; i++) {
    nlohmann::json node;
    KeyValuePair kv = nodes_[i];
    node["key"] = kv.first;
    node["value"] = kv.second;

    if (i != ROOT) {
      node["parent"] = std::to_string(i / 2);
    }
    if (2 * i <= size_) {
      node["leftChild"] = std::to_string(2 * i);
    }
    if (2 * i + 1 <= size_) {
      node["rightChild"] = std::to_string(2 * i + 1);
    }
    result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size_;
  result["metadata"]["maxHeapSize"] = max_size_;
  return result;
}