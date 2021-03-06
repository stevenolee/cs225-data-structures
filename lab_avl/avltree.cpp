/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

// changes the height by 'offset'for all nodes in the subtree with given root node
template <class K, class V>
int AVLTree<K, V>::getHeight(Node* current){
	if (current == NULL){return -1;}
	if ((current->left == NULL) && (current->right == NULL)){
		return 0;
	}
	if (current->right == NULL){
		return getHeight(current->left) + 1;
	}
	if (current->left == NULL){
		return getHeight(current->right) + 1;
	}

	return max(getHeight(current->left)+1, getHeight(current->right)+1);

}


template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
	Node* stickLeft = root;
	Node* stickMid = stickLeft->right;
	stickLeft->right = stickMid->left;
	stickMid->left = stickLeft;
	root = stickMid;
}


template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t); 	
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
	Node* stickRight = root;
	Node* stickMid = stickRight->left;
//cout << "______________________________________for rotateRight (r, m, l): " << stickRight << stickMid << stickRight << endl;
if (stickMid != NULL){
	cout << "the fucking RIGHT of " << stickMid->value << ": " << stickMid->right << endl;
}
	stickRight->left = stickMid->right;
	stickMid->right = stickRight;
	root = stickMid;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
	rotateRight(t->right);
	rotateLeft(t);	
}



template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
	int heightLeft = getHeight(subtree->left);
	int heightRight = getHeight(subtree->right);
	int balance = heightRight - heightLeft;
	if (balance <= -2){
		if (getHeight(subtree->left->right) - getHeight(subtree->left->left) < 0){
			rotateRight(subtree);
		} else {
			rotateLeftRight(subtree);
		}
	} else if (balance >= 2){
		if (getHeight(subtree->right->right) - getHeight(subtree->right->left) > 0){
			rotateLeft(subtree);
		} else {
			rotateRightLeft(subtree);
		}
	}
	int height = getHeight(subtree);
	subtree->height = height;
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
// base case
	if (subtree == NULL){
		Node* child = new Node(key, value);
		subtree = child;
		subtree->height = 0;
		return;
	}

// recursion
	if (key < subtree->key){
		insert(subtree->left, key, value);
	} else {
		insert(subtree->right, key, value);
	}

// rebalance
	rebalance(subtree);


}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        // your code here
    } else if (key > subtree->key) {
        // your code here
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
        } else {
            /* one-child remove */
            // your code here
        }
        // your code here
    }
}
