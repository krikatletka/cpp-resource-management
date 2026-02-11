#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cstddef>

// This assignment sparked my interest in implementing core data structures from scratch. 
// I initially tried a vector-based set, but insert/erase become O(n).
// This motivated me to implement a BST-based set from scratch as a learning exercise.


// This implementation uses an unbalanced BST.
// Average complexity is O(log n), but the worst case can degrade to O(n).
// A natural next step would be implementing a self-balancing tree
// (AVL or Red-Black) to guarantee O(log n) worst-case performance.


class MySet {
private:
    struct Node {
        std::string key;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(std::string k) : key(std::move(k)) {}  
    };

    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;

   
    static bool contains_impl(const Node* node, const std::string& key) 
        while (node) {
            if (key < node->key) node = node->left.get();
            else if (key > node->key) node = node->right.get();
            else return true; 
        }
        return false; 
    }

    static bool insert_impl(std::unique_ptr<Node>& node, std::string key) {
        if (!node) {
            node = std::make_unique<Node>(std::move(key));  
            return true;
        }

        if (key < node->key) {
            return insert_impl(node->left, std::move(key));
        }
        if (key > node->key) {
            return insert_impl(node->right, std::move(key));
        }

        return false; 
    }


    static std::string extract_min_key(std::unique_ptr<Node>& node) {
        if (!node->left){
            std::string k = std::move(node->key);
            node = std::move(node->right);
            return k;
        }
        return extract_min_key(node->left);
    }


    static bool erase_impl(std::unique_ptr<Node>& node, const std::string& key) {
        if (!node) return false;

        if (key < node->key) {
            return erase_impl(node->left, key);
        }
        if (key > node->key) {
            return erase_impl(node->right, key);
        }


        if (!node->left) {
            node = std::move(node->right);
            return true;
        }

       
        if (!node->right) {
            node = std::move(node->left);
            return true;
        }

        node->key = extract_min_key(node->right);
        return true;

    }

    static void inorder_impl(const Node* node, std::vector<std::string>& out) {
        if (!node) return;
        inorder_impl(node->left.get(), out);
        out.push_back(node->key); 
        inorder_impl(node->right.get(), out);
    }

public:
    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void clear() noexcept {
        root_.reset();
        size_ = 0;
    }

    bool contains(const std::string& key) const {
        return contains_impl(root_.get(), key);
    }

    bool insert(std::string key) {
        bool ok = insert_impl(root_, std::move(key));
        if (ok) ++size_;
        return ok;
    }

    bool erase(const std::string& key) {
        bool ok = erase_impl(root_, key);
        if (ok) --size_;
        return ok;
    }

    std::vector<std::string> values_sorted() const {
        std::vector<std::string> out;
        out.reserve(size_); 
        inorder_impl(root_.get(), out);
        return out;
    }
};
