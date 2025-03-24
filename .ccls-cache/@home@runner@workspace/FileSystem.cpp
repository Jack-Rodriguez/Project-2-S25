#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>
#include <stack>
#include <iostream>
using namespace std;

/*-----------------------------------------------------------------------------------------------------------------------
Hello, my name is Jack Rodriguez U69523108. This is my own work. Any and all code taken from the my research is cited 
mentioned below. I did not steal any work and say it is my own. I used AI like ChatGPT and Replit assistant to help me
learn concepts and debug my code. What I did not do was use AI to write any code for me. Most functions were complete 
within the first week. I had to go back and redo many of these as they ended up not working in future tests. My biggest 
problem was cd. I coded cd to work as intended within the testcd in the tesstest.cpp file. Cd ends up being used in three 
different ways than I was prepared for, making me change it each time. I left comments on each function for what worked 
and what didn't. This was a challenging project but I learned a lot. It is quite messy after all of the debugging so I 
hope to come back some day and clean it up. If you read this have a great day! And thank you for your hard work!
-----------------------------------------------------------------------------------------------------------------------*/

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
            throw runtime_error("File already exists");
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

//this function changes directories. Though I thought I finished this early on, there were many problems 
//later on in other test cases. eventually I figured it out after hours of debugging and research.
void FileSystem::cd(const std::string& path) 
{
    //sets the current directory to the parent directory of the current directory
    if(path == "..")
    {
        currentDirectory = currentDirectory->parent;
    }
    //sets the current directory to the root directory if the path is "/"
    else if(path == "/")
    {
        currentDirectory = root;
    }
    //heres a fun bit. I needed to check if the path started with a slash. If so, I was parsing.
    else if (!path.empty() && path.front() == '/') 
    {

        //the tracker node starts at the root directory
        FileSystemNode* tracker = root;
        //we make a streamed string to parse the path
        stringstream ss(path);
        //this is a temp value to hold any string we get from the path
        string part;
        //we keep track if we find the given directory
        bool found = true;

        //this loop will parse through, and collect every string that is between the slashes
        while (getline(ss, part, '/')) 
        {
            if (part.empty()) continue;
            //we need a found token for this loop as well. It will affect the found variable
            //This is like so messy but it works and Im afraid to change it.
            bool stepFound = false;
            //for every child in the tracker's current directory, ccheck if the part (the string we are parsing)
            //is the same as the child's name
            for (auto child : tracker->children) 
            {
                if (child->name == part) 
                {
                    //changes tracker to that child, and sets found to true
                    tracker = child;
                    stepFound = true;
                    break;
                }
            }
            if (!stepFound) 
            {
                found = false;
                break;
            }
        }

        //if the path was never found, then we throw an error
        if (!found || !tracker->isDirectory) 
        {
            throw runtime_error("Directory not found");
        }
        
        //then we set the current directory to the tracker which is at the desired location
        currentDirectory = tracker;
    } 
        //this is if we are passed a path that is just a directory name with no slashes    
    else
    {
        bool found = false;
        //for each child in the current directory check the names and cd to the matching child
        for (auto child : currentDirectory->children) 
        {
            if (child->name == path && child->isDirectory) 
            {
                currentDirectory = child;
                found = true;
                break;
            }
        }
        //if the path was never found, then we throw an error
        if (!found) throw std::runtime_error("Directory not found");
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

void FileSystem::cp(const std::string& source, const std::string& destination) 
{

    //similar to above, we are going to parse through the string, and save each string between the slashes into a vector, then iterate through the vector, changing the directories to the next child with a matching name.
    stringstream ss(source);
    string part;
    vector<string> pathParts;

    //parses through and pushes the string between the slashes into the vector
    while(getline(ss, part, '/'))
        {
            if(!part.empty())
            {
                pathParts.push_back(part);
            }
        }

    //makes a tracking node that starts at the root directory
    FileSystemNode* tracker = root;

    //goes through the vector and changes the tracker to the next child with a matching name
    for(const auto& step : pathParts)
        {
            bool found = false;
            //checks each child of each string in the vector.
            for(auto child : tracker->children)
                {
                    if(child->name == step)
                    {
                        //tracker moves through the children
                        tracker = child;
                        found = true;
                        break;
                    }
                }
            if(!found)
            {
                throw runtime_error("Source not found");
            }
        }
    
    //we make a sourceNode that is the tracker at the end of the path
    FileSystemNode* sourceNode = tracker;

    //we make a destName to change as destination is a const
    string destName = destination;

    //we take the slash off the front of the destination name
    if (!destName.empty() && destName.front() == '/') 
    {
        destName = destName.substr(1);
    }

    // Check if destination exists in root. If so we throw an error.
    for(auto child : root->children) 
    {
        if(child->name == destName) 
        {
            throw runtime_error("Destination already exists");
        }
    }

    // Create copy with root as parent
    //this begins the recursion
    FileSystemNode* newNode = copyNode(sourceNode, root, destName);
    //adds this new node to the children of the root
    root->children.push_back(newNode);
}


// Helper method to recursively copy a node
//this function is given a source to copy from, a destination parent, and a new name for the copy
FileSystemNode* FileSystem::copyNode(FileSystemNode* source, FileSystemNode* destParent, const std::string& newName) 
{
    
    //creates a new node with the given name and parent
    FileSystemNode* newNode = new FileSystemNode(newName, source->isDirectory);
    //sets the new node to have the destination parent as the parent.
    newNode->parent = destParent;

    //if source is a file, we return the new node
    if(!newNode->isDirectory)
    {
        return newNode;
    }

    //this loops the each child of the new node, it calls the function on each child. 
    //if the child is a directory, it goes through each child of that
    //and so on
    for(auto child : source->children)
        {
            //recursively calls the function on each child
            FileSystemNode* newChild = copyNode(child, newNode, child->name);
            //i had added this way later. Sets the parent of each child to be the new node
             newChild->parent = newNode; 
            //adds each child to the children of the new nodes
            newNode->children.push_back(newChild);
        }

    //returns the new node
    return newNode;
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