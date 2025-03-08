#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>
using namespace std;

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

//this function will make a new directory in the current directory
void FileSystem::mkdir(const std::string& name) 
{
    //this loop will check if the directory name already exists in the current directory.
    for(const auto& child : currentDirectory->children) // i learned this sytax from ChatGPT. I asked it for an unrelated example so that i didnt just copy it without understanding it.
    {
        //if the new name is the same as the name of the child, then it will throw an error.
        if(child->name == name)
        {
            throw runtime_error("Directory already exists");
        }
    }

    //makes a new directory with the given name, and sets it to being true (a directory)
    FileSystemNode* newDir = new FileSystemNode(name, true);

    //sets the new directory to point to the current directory as a parent
    newDir->parent = currentDirectory;

    //adds the new directory to the vector of children in the current directory
    currentDirectory->children.push_back(newDir);

}

//this function will make a new file in the current directory
void FileSystem::touch(const std::string& name) 
{
    //this loop will check if the file name already exists in the current directory.
   for(const auto& child : currentDirectory->children)
    {
        //if the new name is the same as the name of the child, then it will throw an error.
        if(child->name == name)
        {
            throw runtime_error("File already exists");
        }  
    }
    
    //creates new file with the current name. Its a file because isDir is false
    FileSystemNode* newFile = new FileSystemNode(name, false);

    //sets the file to point to the current directory as a parent
    newFile->parent = currentDirectory;

    //adds the new file to the back of the children vector of the current directory
    currentDirectory->children.push_back(newFile);
}

std::string FileSystem::ls() 
{
    //initializes streamed string
    stringstream ss;

    //for every child in the current directory, it will add the name to the stringstream
    for(const auto& child : currentDirectory->children)
    {
        //if is is a directory, a slash is added to the end of the name
        if(child->isDirectory)
        {
            ss << child->name << "/\n";
        }
        else{
            ss << child->name << "\n";
        }
    }

    //returns the stringstream as a string
    return ss.str();
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