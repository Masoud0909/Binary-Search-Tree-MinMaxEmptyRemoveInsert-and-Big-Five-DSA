/*As we can see the MergeSort performed dramatically better as far as time is concerned. */
/*Also for Time Complexity of treeSort: inserting each elem into BST takes O(log n) time on average(n= # of elems)but when it's not balanced it's gonna be O(n) per elem. so since there are n elems in input vector , total time complexity would be O(n log n )on average, but O(n^2) in worst case */


//*Masoud Rafiee*/
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

class TreeNode {//class for tree nodes and its defult constructor
public:
	int val;//integer value to store as node
	TreeNode* left;//pointer to the left child node 
	TreeNode* right;//pointer to the right child node
	TreeNode(int value) : val(value), left(nullptr), right(nullptr) {}
	//constructor to initialize the node with a given value and null left and right pointers
};

class BST {//starting the binary search tre
private:
	TreeNode* root;//pointer to the root of binary search tree

	TreeNode* insertHelper(TreeNode* node, int value) {//helper function to insert a value into our BST
		if (node == nullptr) {// if the current node is null, 
			return new TreeNode(value);//create a new node with the value
		}
		if (value < node->val) {//if the value in the inserted node is less than the val of current node 
			node->left = insertHelper(node->left, value);//recursively insert to the left subtree 
		}
		else if (value > node->val) {//if it's greater 
			node->right = insertHelper(node->right, value);//recursively insert to right subtree
		}
		return node;//return the current node
	}

	TreeNode* removeHelper(TreeNode* node, int value) {//to remove a node/val from BST
		if (node == nullptr) return nullptr;//if it's already null just return null
		if (value < node->val) {//if the valu is lesser than current val
			node->left = removeHelper(node->left, value);//recursively remove from left subtree
		}
		else if (value > node->val) {//if the val is bigger thn current val
			node->right = removeHelper(node->right, value);//recursiv. remove from right subteee
		}
		else {
			if (node->left == nullptr) {//if they're the same (current node val and what we want to remove
				TreeNode* temp = node->right;//if the left child is null, replace the node with its right child 
				delete node;//since we dont need it anymore, delete it
				return temp;
			}
			else if (node->right == nullptr) {
				TreeNode* temp = node->left;//ssame for right child, if its null, replace it with its left child
				delete node;
				return temp;
			}
			TreeNode* temp = findMin(node->right);
			node->val = temp->val;
			node->right = removeHelper(node->right, temp->val);
		}
		return node;
	}

	TreeNode* findMin(TreeNode* node) {
		if (node == nullptr) return nullptr;
		while (node->left != nullptr) {//traversing all the way to the leAF
			node = node->left;//reaching the leftmost node=smallest node
		}
		return node;//retunr the smallest
	}

	TreeNode* findMax(TreeNode* node) {
		if (node == nullptr) return nullptr;
		while (node->right != nullptr) {//same like find min but when we traverse to right , we'll find the largest node
			node = node->right;
		}
		return node;//retunr the max
	}

	bool isEmpty() const {//check if the BST is empty
		return root == nullptr;// of the root is null, it means it;s an empty tree
	}


	void makeEmpty(TreeNode*& node) {//to delte all nodes in our BST
		if (node != nullptr) {//if the node is not empty
			makeEmpty(node->left);//recursively delete nodes in the left and rright subtrees
			makeEmpty(node->right);
			delete node;//delete the current node
			node = nullptr;//so our current node will be null
		}
	}

	void inorderTraversal(TreeNode* node, std::vector<int>& sorted) {//performing inorder traversal and store values in a vector
		if (node != nullptr) {//check if current node is not empty 
			inorderTraversal(node->left, sorted);//recursively trverse the left;subtrree
			sorted.push_back(node->val);//pushing back the current node into the sorted vector
			inorderTraversal(node->right, sorted);//same for right subtreee
		}
	}

public:
	BST() : root(nullptr) {}//constructor  of BST:  with null root
	//BIG FIVE
	~BST() {//destructor:deallocating the memory occupied by bst
		makeEmpty(root);//deleting all ndoes
	}
	BST(const BST& other) = delete; // Copy constructor: disable to prevent unintended copiyng 
	BST& operator=(const BST& other) = delete; //copy assignment:same reason 
	BST(BST&& other) noexcept : root(other.root) {//move constructor:efficint transfering ownership of resource
		other.root = nullptr;//set the root of other tree to null to prevent double deletion
	} 
	BST& operator=(BST&& other) noexcept {//move assignment operator:
		if (this != &other) {//check for self-assignment 
			makeEmpty(root);////delete all nodes in current tree
			root = other.root;//transsfer owner ship of root ptr
			other.root = nullptr;//prevent double  deletion
		}
		return *this;//return reference to current object
	}

	void insert(int value) {//to insert a value (this one is public)
		root = insertHelper(root, value);//caling the helper one to do the job
	}

	void remove(int value) {//same , but public
		root = removeHelper(root, value);//calling helper one to do the task
	}

	std::vector<int> treeSort(const std::vector<int>& elems) {//sort a vector using BST
		BST tempTree;
		for (int elem : elems) {//iterate through the elements of the input vector
			tempTree.insert(elem);//insert each element into the temporary tree
		}
		std::vector<int> sorted;//declare a vec to store the sorted elems
		inorderTraversal(tempTree.root, sorted);//call this funct. to perform inorder traversal and sotre sorted elems
		return sorted;//retunring the sorted VECTOR
	}
};

int main() {
	BST bst;//creating an instance of BST we just built { interesting ! :) }
	std::vector<int> unsorted = {13,6,74,1,9,8 };
	std::vector<int> sorted = bst.treeSort(unsorted);//sort our vector using our tree sort fiunct

	std::cout << "Sorted array : ";
	for (int elem : sorted) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	//TIME EXPERIMENT FOR: treeSort
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<int> sorted1 = bst.treeSort(unsorted);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed= std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "treeSort took " << elapsed << " nanoseconds." << std::endl;

	//TIME EXPERIMENT FOR: mergeSort 
	start = std::chrono::high_resolution_clock::now();
	std::sort(unsorted.begin(), unsorted.end()); //implementing mergeSort as std::sort
	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "mergeSort took " << elapsed << " nanoseconds." << std::endl;

	return 0;
}


