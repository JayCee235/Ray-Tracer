#ifndef __BVHTREE
#define __BVHTREE

#include "vector/GenVector.h"
#include "Primitive.h"
#include "HitPoint.h"
#include "Loader.h"
#include "BVHNode.h"

class BVHTree : public Primitive
{
private:
	BVHNode* data;
	BVHTree* left;
	BVHTree* right;
	BVHTree* parent;
	int red;

public:
	BVHTree() {
		this->left = NULL;
		this->right = NULL;
		this->data = NULL;
		this->parent = NULL;
		this->red = 1;
	}

	BVHTree(BVHNode* n) {
		this->data = n;
		this->left = NULL;
		this->right = NULL;
		this->red = 1;
		this->parent = NULL;
	}

	BVHTree(Primitive* p) {
		this->data = new BVHNode(p);
		this->left = NULL;
		this->right = NULL;
		this->parent = NULL;
		this->red = 0;
	}

	BVHTree(BVHTree* copy) {
		this->left = copy->left;
		this->right = copy->right;
		this->data = new BVHNode(copy->data);
		this->red = copy->red;
		this->parent = NULL;
	}

	BVHTree* getParent() {
		return this->parent;
	}

	BVHTree* getGrandparent() {
		if(this->parent == NULL) {
			return NULL;
		} else {
			return this->parent->parent;
		}
	}

	BVHTree* insertPrimitive(Primitive* p) {
		if(this->parent != NULL) {
			return getRoot()->insertPrimitive(p);
		} 
		if(this->left == NULL && this->right == NULL && this->data == NULL) {
			this->data = new BVHNode(p);
			this->red = 0;
			return this;
		}

		BVHNode* toAdd = new BVHNode(p);
		this->insert(toAdd);

		return this->getRoot();
		// printTreeHelper(0);
	}

	BVHTree* getRoot() {
		if(this->parent == NULL) {
			return this;
		}
		return this->parent->getRoot();
	}

	//Assumes that the tree either has children or data.
	void insert(BVHNode* toAdd) {
		// printf("Inserting new node...\n");
		//Leaf node, so set left to new info and right to my info.
		if(left == NULL && right == NULL) {
			// printf("Parents are null.\n");
			BVHNode* newNode = new BVHNode(this->data);
			this->left = new BVHTree(toAdd);
			this->informChildren();
			this->left->recolour();
			// printf("Nodes made.\n");
			this->right = new BVHTree(newNode);
			this->informChildren();
			this->right->recolour();
			// printf("Added new leaf.\n");

			this->data->resizeToFit(toAdd->getBox());
		} else {
			// printf("Parents were not null.\n");

			AABB* addedBox = toAdd->getBox();

			float leftDif;
			float rightDif;

			if(left != NULL) {
				AABB* leftBox = leftBox = left->data->getBox();
				leftDif = leftBox->getNewVolume(addedBox) - leftBox->getCurrentVolume();
			} else {
				leftDif = addedBox->getCurrentVolume();

			}

			if(right != NULL) {
				AABB* rightBox = rightBox = right->data->getBox();
				rightDif = rightBox->getNewVolume(addedBox) - rightBox->getCurrentVolume();
			} else {
				rightDif = addedBox->getCurrentVolume();
			}

			// float combineDif = addedBox->getCurrentVolume() + leftBox->getNewVolume(rightBox);
			// combineDif -= leftBox->getCurrentVolume() + rightBox->getCurrentVolume();

			////Combining left and right is less volume than adding toAdd to either. 
			//Set my left to left+right (me), and my right to toAdd.
			// if(combineDif < leftDif && combineDif < rightDif) {
			// 	//Cry because this is viable, but too hard to rebalance.
			// 	BVHTree* newLeft = new BVHTree(this);
			// 	BVHTree* newRight = new BVHTree(toAdd);
			// 	newLeft->parent = this;
			// 	newRight->parent = this;
			// 	this->left = newLeft;
			// 	this->right = newRight;
			// 	this->data->resizeToFit(addedBox);
			// 	this->left->recolour();
			// 	this->right->recolour();
			// 	return;
			//}
			this->data->resizeToFit(addedBox);

			// Left is less added volume.
			if(leftDif < rightDif) {
				if(this->left!= NULL) {
					this->left->insert(toAdd);
				} else {
					BVHTree* newTree = new BVHTree(toAdd);
					this->left = newTree;
					this->informChildren();
					newTree->recolour();
				}
			//Right is less added volume.
			} else {
				if(this->right!=NULL) {
					this->right->insert(toAdd);
				} else {
					BVHTree* newTree = new BVHTree(toAdd);
					this->right = newTree;
					this->informChildren();
					newTree->recolour();
				}
			}
		}
		
	}

	void forceCheck() {
		this->informChildren();
		if(this->left != NULL) {
			this->left->forceCheck();
			this->data->resizeToFit(this->left->data->getBox());
		}
		if(this->right != NULL) {
			this->right->forceCheck();
			this->data->resizeToFit(this->right->data->getBox());
		}
	}

	void recolour1(){
		if(this->red == 0) {
			return;
		}
		//I am the root.
		if(this->parent == NULL) {
			this->red = 0;
			return;
		}
		recolour2();
	}

	void recolour2() {
		//My parent is black.
		if(this->parent->red == 0) {
			return;
		}
		recolour3();
	}

	void recolour3() {
		//Both my parent and uncle are red.
		BVHTree* g = this->getGrandparent();
		// this->printTree();
		// printf("My grandparent is required ahead.\n");
		BVHTree* par = this->parent;
		if(g->leftRed() == 1 && g->rightRed() == 1) {
			// printf("Both red.\n");
			g->setLeftRed(0);
			g->setRightRed(0);
			g->red = 1;
			g->recolour();
			return;
		}
		recolour4();
	}

	void recolour4() {
		BVHTree* g = this->getGrandparent();
		BVHTree* par = this->parent;
		// printf("I continued to case 4.\n");
		bool leftChild = par->left == this;
		if(leftChild) {
			//I am a left child of a red right child, and my uncle is black.
			if(g->right == par) {
				// printf("I am case 4 left.\n");
				//Grandparent right becomes me.
				//My right becomes my parent.
				//My parent's left becomes my right.

				//My parent's left becomes my right.
				par->left = this->right;
				// printf("My parent rotated successfully.\n");

				//My right becomes my parent.
				this->right = par;

				// printf("Success 2.\n");

				//Grandperent right becomes me.
				g->right = this;
				this->informChildren();
				par->informChildren();
				g->informChildren();

				// printf("almost there.\n");

				par->recalculateBounds();
				this->recalculateBounds();
				g->recalculateBounds();
				// printf("Bounds recalculated.\n");
				par->recolour5();
				return;
			}
			recolour5();
		} else {
			//I am a right child of a red left child, and my uncle is black.
			if(g->left == par) {
				// printf("I am case 4 right.\n");
				//Grandparent left becomes me.
				//My left becomes my parent.
				//My parent's right becomes my left.

				//My parent's right becomes my left.
				par->right = this->left;
				// printf("My parent rotated successfully.\n");

				//My left becomes my parent.
				this->left = par;

				// printf("Success 2.\n");

				//Grandperent left becomes me.
				g->left = this;
				this->informChildren();
				par->informChildren();
				g->informChildren();

				// printf("almost there.\n");

				par->recalculateBounds();
				this->recalculateBounds();
				g->recalculateBounds();
				// printf("Bounds recalculated.\n");
				par->recolour5();
				return;
			}
			recolour5();
		}
	}

	void recolour5() {
		BVHTree* g = this->getGrandparent();
		BVHTree* par = this->parent;
		bool leftChild = par->left == this;
		if(leftChild) {
			// printf("I am case 5 left.\n");
			//I am the left child of a red left child, and my uncle is black.
			//My grandparent's left becomes my parent's right.
			BVHTree* gp = g->parent;
			g->left = par->right;

			//My parent's right becomes my grandparent.
			par->right = g;

			//My great-grandparent adopts his new grandson accordingly.
			if(gp==NULL) {
				par->parent = NULL;
			} else if(gp->left == g) {
				gp->left = par;
				par->parent = gp;
			} else {
				gp->right = par;
				par->parent = gp;
			}

			par->informChildren();
			g->informChildren();
			if(g->parent!=NULL) {
				g->parent->informChildren();
			}

			g->red = 1;
			par->red = 0;

			g->recalculateBounds();
			par->recalculateBounds();
			if(par->parent != NULL) {
				par->parent->recalculateBounds();
			}
			return;
		} else {
			// printf("I am case 5 right.\n");
			//I am the right child of a red right child, and my uncle is black.
			//My grandparent's right becomes my parent's left.
			BVHTree* gp = g->parent;
			g->right = par->left;

			//My parent's left becomes my grandparent.
			par->left = g;

			//My great-grandparent adopts his new grandson accordingly.
			if(gp==NULL) {
				par->parent = NULL;
			} else if(gp->right == g) {
				gp->right = par;
				par->parent = gp;
			} else {
				gp->left = par;
				par->parent = gp;
			}

			par->informChildren();
			g->informChildren();
			if(g->parent!=NULL) {
				g->parent->informChildren();
			}

			par->red = 0;
			g->red = 1;

			g->recalculateBounds();
			par->recalculateBounds();
			if(par->parent != NULL) {
				par->parent->recalculateBounds();
			}
			return;
		}
	}



	void recolour() {
		recolour1();
		return;
	}

	void informChildren() {
		if(left != NULL) {
			left->parent = this;
		}
		if(right != NULL) {
			right->parent = this;
		}
	}

	void recalculateBounds() {
		if(this->left == NULL || this->right == NULL) {
			return;
		}
		AABB* newBounds = new AABB(this->left->data->getBox());
		newBounds->resizeToFit(this->right->data->getBox());
		this->data->setBox(newBounds);
	}


	~BVHTree() {}

	float intersect(Ray* r) {
		// printf("I am intersecting a new node.\n");
		if(this->data == NULL) {
			// printf("I have no data.\n");
			return -1;
		}
		float check = this->data->getBox()->intersect(r);
		if(check < 0) {
			return -1;
		}
		if(this->left == NULL && this->right == NULL) {
			// printf("Returning my data intersection!\n");
			return this->data->intersect(r);
		}
		float leftNum = -1;
		float rightNum = -1;
		Primitive* leftPrim = NULL;
		Primitive* rightPrim = NULL;

		if(this->left != NULL) {
			leftNum = left->intersect(r);
			leftPrim = left->getPrimitive();
			// printf("I got %.2f from left.\n", leftNum);
		}
		if(this->right != NULL) {
			rightNum = right->intersect(r);
			rightPrim = right->getPrimitive();
			// printf("I got %.2f from right.\n", rightNum);
		}

		if(leftNum < -0.001 && rightNum < -.001) {
			// printf("Both below 0.\n");
			return -1;
		}
		if(leftNum < -0.001) {
			// printf("Returning right.\n");
			this->data->setPrimitive(rightPrim);
			return rightNum;
		}
		if(rightNum < -0.001) {
			// printf("Returning left.\n");
			this->data->setPrimitive(leftPrim);
			return leftNum;
		}
		if(leftNum < rightNum && leftPrim != NULL) {
			// printf("Returning left.\n");
			this->data->setPrimitive(leftPrim);
			return leftNum;
		}
		// printf("Returning right.\n");
		this->data->setPrimitive(rightPrim);
		return rightNum;
	}
	Vector3 getNormal(Ray* r, HitPoint* hp) {
		//This shouldn't be called on a BVHTree. If it is, recall it on hp prim.
		if(hp->p == this) {
			//cry and return up.
			return Vector3(0, 1, 0);
		}
		return hp->p->getNormal(r, hp);
	}

	int leftRed() {
		if(this->left == NULL) {
			return 0;
		}
		return this->left->red;
	}

	int rightRed() {
		if(this->right == NULL) {
			return 0;
		}
		return this->right->red;
	}

	void setLeftRed(int i) {
		if(this->left == NULL) {
			return;
		}
		this->left->red = i;
	}

	void setRightRed(int i) {
		if(this->right == NULL) {
			return;
		}
		this->right->red = i;
	}

	Primitive* getPrimitive() {
		return this->data->getPrimitive();
	}

	Material* getMaterial() {
		//Shouldn't be called on tree, so feel free to return default.
		return new Material();
	}

	Vector3 getMinimumPoint() {
		return data->getMinimumPoint();
	}

	Vector3 getMaximumPoint() {
		return data->getMinimumPoint();
	}

	int getSize() {
		int total = 1;
		if(left != NULL) {
			total += left->getSize();
		}
		if(right != NULL) {
			total += right->getSize();
		}
		return total;
	}

	int getLeaves() {
		if(left == NULL && right == NULL) {
			return 1;
		}
		int total = 0;
		if(left != NULL) {
			total += left->getLeaves();
		}
		if(right != NULL) {
			total += right->getLeaves();
		}
		return total;

	}

	void printTree() {
		printf("Size: %d\n", getSize());
		printf("Number of Leaves: %d\n", getLeaves());
		// printTreeHelper(0);	
	}

	void printTreeHelper(int i) {
		if(left != NULL) {
			left->printTreeHelper(i+1);
		}
		for(int k = 0; k < i; k++) {
			printf(" ");
		}
		int hasParent = 1;
		if(parent == NULL) hasParent = 0;
		printf("(l %d c %d p %d) ", i, red, hasParent);
		if(this->left == NULL && this->right == NULL) {
			int blackNodeCount = reverseColorCheck();
			printf("%d", blackNodeCount);
		}
		printf("\n");
		if(right != NULL) {
			right->printTreeHelper(i+1);
		}
	}

	int reverseColorCheck() {
		int out = 1 - red;
		if(parent!=NULL) {
			out = out + parent->reverseColorCheck();
		}
		return out;
	}

	void printReverseColorCheck() {
		printf("%d ", this->red);
		if(parent != NULL) {
			parent->printReverseColorCheck();
		}
	}

	void printColorCheck() {
		printf("I am %d and my children are %d, %d\n", red, leftRed(), rightRed());
		if(left != NULL) {
			left->printColorCheck();
		}
		if(right != NULL) {
			right->printColorCheck();
		}
	}
};

#endif