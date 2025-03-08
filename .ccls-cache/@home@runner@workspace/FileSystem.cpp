#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>

FileSystemNode::FileSystemNode(std::string name, bool isDir) 
    : name(name), isDirectory(isDir), parent(nullptr) {}

FileSystemNode::~FileSystemNode() {
    for (auto child : children) {
        delete child;
    }
}

FileSystem::FileSystem() {
    root = new FileSystemNode("/", true);
    currentDirectory = root;
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::mkdir(const std::string& name) {
    //to do..
}

void FileSystem::touch(const std::string& name) {
   //to do..
}

std::string FileSystem::ls() {
    // to do..
    // You can sstream here (like displayTree())
    return 0;
}

void FileSystem::cd(const std::string& path) {
    //to do..
   
}

void FileSystem::rm(const std::string& name) {
    //to do..
   
}

std::string FileSystem::pwd() {
    //to do..
    return 0;
}

void FileSystem::cp(const std::string& source, const std::string& destination) {
    // Hints:
    // - Parse paths to find source node and destination parent
    // - Check if destination exists (throw error if yes)
    // - Create copy using copyNode helper
    // - Add to destination's children
}

// Helper method to recursively copy a node
FileSystemNode* FileSystem::copyNode(FileSystemNode* source, FileSystemNode* destParent, const std::string& newName) {
    // Hints:
    // - Create new node with same properties
    // - For directories, recursively copy children
    return nullptr; // Placeholder
}

FileSystemNode* FileSystem::findNode(FileSystemNode* startNode, const std::string& name) {
    if (startNode->name == name) {
        return startNode;
    }
    for (auto child : startNode->children) {
        FileSystemNode* found = findNode(child, name);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

FileSystemNode* FileSystem::find(const std::string& name) {
    return findNode(root, name);
}

std::string FileSystem::displayTree(FileSystemNode* node, std::string indent) {
    std::stringstream ss;
    ss << indent << node->name << (node->isDirectory ? "/" : "") << "\n";
    if (node->isDirectory) {
        for (auto child : node->children) {
            ss << displayTree(child, indent + "  ");
        }
    }
    return ss.str();
}

std::string FileSystem::tree() {
    return displayTree(root, "");
}