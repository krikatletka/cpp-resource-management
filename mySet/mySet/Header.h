#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cstddef>

class MySet {
private:
    struct Node {
        std::string key;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(std::string k) : key(std::move(k)) {}   //random implicit transformations
    };

    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;

   
    static bool contains_impl(const Node* node, const std::string& key) {
        while (node) {
            if (key < node->key) node = node->left.get();
            else if (key > node->key) node = node->right.get();
            else return true; 
        }
        return false; //= равно
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

        return false; // дубль
    }


    static Node* min_node(Node* node) {
        while (node->left) node = node->left.get();
        return node;
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

        // нет пр
        if (!node->right) {
            node = std::move(node->left);
            return true;
        }

        Node* succ = min_node(node->right.get());
        node->key = succ->key;
        return erase_impl(node->right, succ->key);
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