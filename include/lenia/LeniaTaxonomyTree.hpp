#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <stack>

#include <nlohmann/json.hpp>

class TreeNode {
public:
    std::string name;
    std::vector<TreeNode*> children;

    std::string data;

    bool isLeaf() const {
        return children.empty();
    }

    TreeNode(const std::string& name) : name(name) {}

    void addChild(TreeNode* child) {
        children.push_back(child);
    }

    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

class LeniaTaxonomyTree {
public:
    TreeNode* root;

    LeniaTaxonomyTree() {
        root = new TreeNode("Lenia");
    }

    ~LeniaTaxonomyTree() {
        delete root;
    }

    void addPhylum(const std::string& phylum) {
        root->addChild(new TreeNode(phylum));
    }

    void addClass(const std::string& phylum, const std::string& class_) {
        TreeNode* phylumNode = findNode(root, phylum);
        if (phylumNode == nullptr) {
            std::cerr << "Phylum not found: " << phylum << std::endl;
            return;
        }
        phylumNode->addChild(new TreeNode(class_));
    }

    void addOrder(const std::string& phylum, const std::string& class_, const std::string& order) {
        TreeNode* phylumNode = findNode(root, phylum);
        if (phylumNode == nullptr) {
            std::cerr << "Phylum not found: " << phylum << std::endl;
            return;
        }
        TreeNode* classNode = findNode(phylumNode, class_);
        if (classNode == nullptr) {
            std::cerr << "Class not found: " << class_ << std::endl;
            return;
        }
        classNode->addChild(new TreeNode(order));
    }

    void addFamily(const std::string& phylum, const std::string& class_, const std::string& order, const std::string& family) {
        TreeNode* phylumNode = findNode(root, phylum);
        if (phylumNode == nullptr) {
            std::cerr << "Phylum not found: " << phylum << std::endl;
            return;
        }
        TreeNode* classNode = findNode(phylumNode, class_);
        if (classNode == nullptr) {
            std::cerr << "Class not found: " << class_ << std::endl;
            return;
        }
        TreeNode* orderNode = findNode(classNode, order);
        if (orderNode == nullptr) {
            std::cerr << "Order not found: " << order << std::endl;
            return;
        }
        orderNode->addChild(new TreeNode(family));
    }

    void addGenus(const std::string& phylum, const std::string& class_, const std::string& order, const std::string& family, const std::string& genus) {
        TreeNode* phylumNode = findNode(root, phylum);
        if (phylumNode == nullptr) {
            std::cerr << "Phylum not found: " << phylum << std::endl;
            return;
        }
        TreeNode* classNode = findNode(phylumNode, class_);
        if (classNode == nullptr) {
            std::cerr << "Class not found: " << class_ << std::endl;
            return;
        }
        TreeNode* orderNode = findNode(classNode, order);
        if (orderNode == nullptr) {
            std::cerr << "Order not found: " << order << std::endl;
            return;
        }
        TreeNode* familyNode = findNode(orderNode, family);
        if (familyNode == nullptr) {
            std::cerr << "Family not found: " << family << std::endl;
            return;
        }
        familyNode->addChild(new TreeNode(genus));
    }

    void addSpecies(const std::string& phylum, const std::string& class_, const std::string& order, const std::string& family, const std::string& genus, const std::string& species, const std::string& data) {
        TreeNode* phylumNode = findNode(root, phylum);
        if (phylumNode == nullptr) {
            std::cerr << "Phylum not found: " << phylum << std::endl;
            return;
        }
        TreeNode* classNode = findNode(phylumNode, class_);
        if (classNode == nullptr) {
            std::cerr << "Class not found: " << class_ << std::endl;
            return;
        }
        TreeNode* orderNode = findNode(classNode, order);
        if (orderNode == nullptr) {
            std::cerr << "Order not found: " << order << std::endl;
            return;
        }
        TreeNode* familyNode = findNode(orderNode, family);
        if (familyNode == nullptr) {
            std::cerr << "Family not found: " << family << std::endl;
            return;
        }
        TreeNode* genusNode = findNode(familyNode, genus);
        if (genusNode == nullptr) {
            std::cerr << "Genus not found: " << genus << std::endl;
            return;
        }
        genusNode->addChild(new TreeNode(species));
        TreeNode* speciesNode = findNode(genusNode, species);   
        speciesNode->data = data;
    }

    void addSpecies(const std::string& phylum, const std::string& class_, const std::string& order, const std::string& family, const std::string& species, const std::string& data) {
        TreeNode* phylumNode = findNode(root, phylum);
        if (phylumNode == nullptr) {
            std::cerr << "Phylum not found: " << phylum << std::endl;
            return;
        }
        TreeNode* classNode = findNode(phylumNode, class_);
        if (classNode == nullptr) {
            std::cerr << "Class not found: " << class_ << std::endl;
            return;
        }
        TreeNode* orderNode = findNode(classNode, order);
        if (orderNode == nullptr) {
            std::cerr << "Order not found: " << order << std::endl;
            return;
        }
        TreeNode* familyNode = findNode(orderNode, family);
        if (familyNode == nullptr) {
            std::cerr << "Family not found: " << family << std::endl;
            return;
        }
        familyNode->addChild(new TreeNode(species));
        TreeNode* speciesNode = findNode(familyNode, species);   
        speciesNode->data = data;
    }

    void saveToFile(const std::string& filename) {
                nlohmann::json json;
        std::function<void(TreeNode*, nlohmann::json&)> serialize = [&](TreeNode* node, nlohmann::json& jsonNode) {
            jsonNode["name"] = node->name;
            jsonNode["data"] = node->data;
            if (!node->isLeaf()) {
                for (TreeNode* child : node->children) {
                    nlohmann::json childNode;
                    serialize(child, childNode);
                    jsonNode["children"].push_back(childNode);
                }
            }
        };
        serialize(root, json);
        std::ofstream file(filename);
        file << json.dump(4);
        file.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "File not found: " << filename << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        nlohmann::json json = nlohmann::json::parse(buffer.str());
        std::function<TreeNode*(nlohmann::json&)> deserialize = [&](nlohmann::json& jsonNode) -> TreeNode* {
            TreeNode* node = new TreeNode(jsonNode["name"]);
            node->data = jsonNode["data"];
            for (nlohmann::json& child : jsonNode["children"]) {
                node->addChild(deserialize(child));
            }
            return node;
        };
        root = deserialize(json);
    }

    void print() const {
        std::function<void(TreeNode*, int)> printNode = [&](TreeNode* node, int depth) {
            for (int i = 0; i < depth; i++) {
                std::cout << "  ";
            }
            std::cout << "- " << node->name << std::endl;
            for (TreeNode* child : node->children) {
                printNode(child, depth + 1);
            }
        };
        printNode(root, 0);
    }

private:
    TreeNode* findNode(TreeNode* node, const std::string& name) {
        std::stack<TreeNode*> stack;
        stack.push(node);
        while (!stack.empty()) {
            TreeNode* current = stack.top();
            stack.pop();
            if (current->name == name) {
                return current;
            }
            for (TreeNode* child : current->children) {
                stack.push(child);
            }
        }
        return nullptr;
    }
};
