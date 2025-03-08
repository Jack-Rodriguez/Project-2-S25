// FileSystem.hpp
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <vector>
#include <sstream>

class FileSystemNode {
public:
    std::string name;
    bool isDirectory;
    std::vector<FileSystemNode*> children;
    FileSystemNode* parent;

    FileSystemNode(std::string name, bool isDir);
    ~FileSystemNode();
};

class FileSystem {
private:
    FileSystemNode* root;
    FileSystemNode* currentDirectory;

    FileSystemNode* findNode(FileSystemNode* startNode, const std::string& name);
    std::string displayTree(FileSystemNode* node, std::string indent);
    FileSystemNode* copyNode(FileSystemNode* source, FileSystemNode* destParent, const std::string& newName);

public:
    FileSystem();
    ~FileSystem();

    void mkdir(const std::string& name);
    void touch(const std::string& name);
    std::string ls();
    void cd(const std::string& path);
    void rm(const std::string& name);
    std::string pwd();
    FileSystemNode* find(const std::string& name);
    std::string tree();
    void cp(const std::string& source, const std::string& destination);
};

#endif // FILESYSTEM_HPP