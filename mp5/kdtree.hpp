/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <math.h>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
// get first and second points that you are comparing
	int valFirst = first[curDim];
	int valSecond = second[curDim];
// compare the values
	if (valFirst < valSecond){
		return true;
	}
	else if (valFirst > valSecond){
		return false;
	}
// if they're equal, use < operator
	else if (valFirst == valSecond){
		if (first < second){
			return true;
		}
		else {
			return false;
		}
	}
	
    return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
	double currentDistance = 0;
	double potentialDistance = 0;
// calculate currentBest's distance	
	for (int i = 0; i < Dim; i++){
		currentDistance = currentDistance + pow(target[i] - currentBest[i], 2);
		potentialDistance = potentialDistance + pow(target[i] - potential[i], 2);
	}
	currentDistance = sqrt(currentDistance);
	potentialDistance = sqrt(potentialDistance);
// check if distances are equal
// if so, compare the points

// if distances are not equal
	if (potentialDistance < currentDistance){
		return true;
	} 
	else if (currentDistance < potentialDistance){
		return false;
	}
	else if (currentDistance == potentialDistance){
		return potential < currentBest;
	}

	
     return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
	vector<Point<Dim>> sorted = newPoints;
	if (sorted.size() == 0){
		root = NULL;
		size = 0;
		return;
	}
	else {
//		vector<Point<Dim>> sorted = newPoints;
		root = buildTree(sorted, 0, sorted.size()-1, 0);
	}

}
  
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(vector<Point<Dim>>& sorted, int start, int end, int counter){
// set the current dimension and increment our counter
	if (start > end){
		return NULL;
	}

	int curDim = counter % Dim;
	counter++;
//cout << "BUILD TREE START AND END: " << start << " " << end << endl;
	int m = (start+end)/2;
	int median = quickSelect(sorted, start, end, m, curDim);
	KDTreeNode* newNode = new KDTreeNode(sorted[median]);
// build left subtree
	newNode->left = buildTree(sorted, start, median-1, counter);
// build right subtree
	newNode->right = buildTree(sorted, median+1, end, counter);
	return newNode;
}

template <int Dim>
int KDTree<Dim>::quickSelect(vector<Point<Dim>>& v, int start, int end, int k, int dimension){

// reached the end
if (start == end){
	return start;
}
	int pivotIndex = end;
	if ((end - start + 1) != 0){
		pivotIndex = start + (int)(rand() % (end - start + 1));
	} 
	pivotIndex = partition(v, start, end, pivotIndex, dimension);
//	int newLength = index - start + 1;
	if (pivotIndex == k){
		return pivotIndex;
	}
	else if (k < pivotIndex){
		return quickSelect(v, start, pivotIndex - 1, k, dimension);
	}
	else {
		return quickSelect(v, pivotIndex + 1, end, k, dimension);
	}
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& v, int start, int end, int pivotIndex, int dimension){
//cout << pivotIndex << " " << dimension << " " << v.size() << " " << Dim << endl;
	int pivotValue = v[pivotIndex][dimension];
// move pivot to end
	swap(v[pivotIndex], v[end]);
// save the start index as a temp variable
	int storeIndex = start;
	for (int i = start; i < end; i++){
		if (v[i][dimension] < pivotValue){
			swap(v[storeIndex], v[i]);
			storeIndex++;
		}
	}
// finalize pivot
	swap(v[end], v[storeIndex]);
	return storeIndex;
}

template <int Dim>
void KDTree<Dim>::destroy(KDTreeNode* node) {
	if (node == NULL){
		return;
	}
// delete left and right subtrees
	destroy(node->left);
	destroy(node->right);
// do the deleting
	delete node;
}

template <int Dim>
void KDTree<Dim>::copy(KDTreeNode *& thisRoot, KDTreeNode *& otherRoot) {
// base case
	if (otherRoot == NULL){
		thisRoot = NULL;
	}
	else {
		thisRoot = newKDTreeNode(otherRoot->point);
// recurse for left and right subtrees
		copy(thisRoot->left, otherRoot->left);
		copy(thisRoot->right, otherRoot->right);
	}
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
	copy(this->root, other->root);
	
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
// if not already equal, destroy this and set this to rhs
	if (this != &rhs){
		destroy(this->root);
		copy(this->root, rhs->root);
	}
	return *this;

}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
	destroy(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::neighborHelper(KDTreeNode* currentNode, const Point<Dim>& query, int count) const{
// track which way we traverse
	bool wentLeft = false;
// create currentBest point;
	Point<Dim> currentBest;

// if we are still recursing deeper into the tree, increment count
	int currentDim = 0;
		currentDim = count % Dim;
		count++;
// found the leaf
	if (currentNode->left == NULL && currentNode->right == NULL){
		if (shouldReplace(query, currentNode->point, root->point)){
			currentBest = root->point;
		}
		else {
			currentBest = currentNode->point;
		}
		return currentBest;
	}
	else {
// choose to recurse left or right subtree
// left
		if (smallerDimVal(query, currentNode->point, currentDim)){
// check if NULL
			if (currentNode->left == NULL){
				wentLeft = false;
				currentNode = currentNode->right;
				currentBest = neighborHelper(currentNode, query, count);
			}
			else {
				wentLeft = true;
				currentNode = currentNode->left;
				currentBest = neighborHelper(currentNode, query, count);
			}
		}
// right
		else {
			if (currentNode->right == NULL){
				wentLeft = true;
				currentNode = currentNode->left;
				currentBest = neighborHelper(currentNode, query, count);
			} 
			else {
				wentLeft = false;
				currentNode = currentNode->right;
				currentBest = neighborHelper(currentNode, query, count);
			}
		}
	}
//count--;
// once it's at the end leaf node
// calculate radius
currentDim = count % Dim;

	if (currentBest == currentNode->point){
		return currentBest;
	}
	double radius = getRadius(currentBest, query);
//	double potential = getRadius(query, currentNode->point);
	double potential = query[currentDim] - currentNode->point[currentDim];
	if (potential < 0 ){
		potential = currentNode->point[currentDim] - query[currentDim];
	}

	

	bool replace = shouldReplace(query, currentBest, currentNode->point);
	if (potential == radius){
		if (smallerDimVal(currentNode->point, currentBest, currentDim)){
			radius = true;
		}
	}
	if (potential < radius || replace){
		if(replace)
			currentBest = currentNode->point;
// now we have to check the other side of the node
		if (wentLeft && (currentNode->right != NULL)){
			currentNode = currentNode->right;
			Point<Dim> consider = neighborHelper(currentNode, query, count);
			if (shouldReplace(query, currentBest, consider)){
				currentBest = consider;
			}
		}
		else if (!wentLeft && (currentNode->left != NULL)){
			currentNode = currentNode->left;
			Point<Dim> consider = neighborHelper(currentNode, query, count);
			if (shouldReplace(query, currentBest, consider)){
				currentBest = consider;
			}
		}
	}



	return currentBest;
}

template <int Dim>
double KDTree<Dim>::getRadius(const Point<Dim>& first, const Point<Dim>& second) const{
	double radius = 0;
	for (int i = 0; i < Dim; i++){
		radius = radius + pow(first[i] - second[i], 2);
	}

	return sqrt(radius);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
//printTree(cout);
	Point<Dim> best = neighborHelper(root, query, 0);
	return best;

//    return Point<Dim>();
}

