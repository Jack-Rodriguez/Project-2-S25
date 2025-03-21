#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>
#include <stack>
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

//this function will return all of the files and directories in the current directory as a string/strings
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

void FileSystem::cd(const std::string& path) 
{
    //sets the current directory to the parent dirstory of the current directory
    if(path == "..")
    {
        currentDirectory = currentDirectory->parent;
    }
    //sets the current directory to the root directory if the path is "/"
    else if(path == "/")
    {
        currentDirectory = root;
    }
    else
    {
        //finds the node with the given name
        FileSystemNode* node = find(path);
        //if the node is not found, it will throw an error
        if(node == nullptr)
        {
            throw runtime_error("Directory not found");
        }
        else if(node->isDirectory == false)
        {
            throw runtime_error("Not a directory");
        }
        //if the node is found, it will set the current directory to the node
        currentDirectory = node;
    }
   
}

//this function deletes a file or directory from the current directory
//at first I assumed that this would be recursive
//but then i looked at the ~FileSystemNode destructor and realized I justy need to iterate
//I would have done a const auto& child : currentDirectory->children like i do above
//but I was having trouble with getting rid of the pointer
//after some research, I learned to do the for loop this way and to use erase
//so this function is of my own work, after outside research.
void FileSystem::rm(const std::string& name) 
{
    //we need to check if the current file even hasd children
    if(currentDirectory->children.empty())
    {
        throw runtime_error("Directory is empty");
    }

    //for every child in the current directory, we will check if the name is the same as the name of the child
    //this line is the one I got from research, I had to change it slightly for this project.
    for(auto i = currentDirectory->children.begin(); i != currentDirectory->children.end(); i++)
        {
            //if so, we delete it using ~FileSystemNode, and then we erase the child from the vector
            if((*i)->name == name)
            {
                delete *i;
                currentDirectory->children.erase(i);
                return;
            }
        }

    //if we cant find the name we throw an error message
    throw runtime_error("File or directory not found");
}

//this will return a string of the path to the current directory
std::string FileSystem::pwd() 
{
    //checks if the current directory is the root directory
    if(currentDirectory->parent == nullptr)
    {
        return "/";
    }

    //creates a stack to store the names of each directory on the path, and a new node that will be iterated to the root
    stack<string> pathStack;
    FileSystemNode* temp = currentDirectory;

    //this will store each name into the stack, and move to the next parent
    while(temp->parent != nullptr)
    {
        pathStack.push(temp->name);
        temp = temp->parent;
    }

    //creates a stringstream to store the path
    stringstream ss;

    //uses ss to create the final string of the entire path
    while(!pathStack.empty())
    {
         ss << "/" << pathStack.top();
         pathStack.pop();
    }
    
    //adds a final slash because every directory ends with one when printed
    ss << "/";
    
    //returns final string
    return ss.str();
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