#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

/**
This class creates the Node object which is used to create the tree for the algorithm. It holds values for the tree such as weight, profit, upperBound, inOptSol, and height.
ALso holds pointers for node: parent, childL, childR 
Methods:
	Node() -default constructor for class. Initializes all values to 0.
	operator<() -overloads the < operator to do comparisons for nodes based on the value of the 		upperBound field.
*/
class Node{
	public:	
		Node *parent;
		Node *childL;
		Node *childR;		
		int weight;
		int profit;
		int upperBound;
		int height;
		bool inOptSol;
		vector<int> hist;
		Node(){
			upperBound = 0;
			weight = 0;
			profit = 0;
			height = 0;
		}
		bool operator<(const Node node1 )const{
			return (this->upperBound < node1.upperBound);
		}
};

/**
Item is a struct that holds two fields: weight and profit of an item
*/
struct Item{
	int weight;	
	int profit;
};

//Function declaration tags
int bb_knapsack(int n, Item *items, int cap, ofstream &out);
float compute_upper_bound(int i, Item *items, int cap, Node node);
void sort_items(Item *items, int &numItems);
float get_pw_ratio(Item &item);

/**
Main function handles parsing, and function calls to sort method and best first search method
*/
int main(int argc, char **argv){
  	int cap, numItems;	//vars for max capacity and items

	//Handles parsing input file
	ifstream infile(argv[1]);
	string line;
	int lineIndex = 0, itemIndex = 0;
	vector<int> profit;
	vector<int> weight;
	while(getline(infile, line)){	
		stringstream in(line);
		string item;
		while (getline(in, item, ',')){
			if (lineIndex == 0 && itemIndex == 0){
				numItems = stoi(item);
				itemIndex++;
			}
			else if (lineIndex == 0 && itemIndex == 1){
				cap = stoi(item);
				itemIndex++;
				lineIndex++;
			}
			else{
				if ((itemIndex%2) == 0){
					weight.push_back(stoi(item));
				}
				else{
					profit.push_back(stoi(item));
				}
				itemIndex++;
			}
		}
	}
	//Creates items array and initalizes it
	Item items[numItems];	
	for (int i=0; i<numItems; i++){
		items[i].weight = weight[i];
		items[i].profit = profit[i]; 
	}
	//Sorts items on P/W ratio
	sort_items(items, numItems);
	
	//Create outfile stream
	ofstream outfile;
	outfile.open(argv[2]);
	bb_knapsack(4, items, cap, outfile);
	
	//close file streams
	infile.close();
	outfile.close();
	
	return 0;
}
/**
sort_items is used to sort arrays of the type Item. It sorts these objects by profit/weight in ascending order. It uses helper function get_pw_ratio to make these calculations. It uses the insertion sort technique which runs in O(n^2).
@param items (Item*) a pointer to an array of items
@param numItems (int) the total number of items to be sorted
*/
void sort_items(Item *items, int &numItems){
	for (int i=1; i<numItems; i++){
		Item key = items[i];	//Key holds the first item to be compared
		int k = i-1;			//k holds the index of the second item being compared
		while(k>=0 && get_pw_ratio(items[k]) < get_pw_ratio(key)){
			items[k+1] = items[k];
			k--;
		}
		items[k+1] = key;	
	}
}

/**
get_pw_ratio is used to calculate the profit over weight ratio for the sort_items function. It returns a float.
@param item (Item) is a Item object whose values are used to calculate the profit over weight
*/
float get_pw_ratio(Item &item){
	return (((float)item.profit)/((float)item.weight));
}

/**
compute_upper_bound is used to find the upper bound of the node using the fractional knapsack method. If the weight of the node is greater than or equal to the capacity of the knapsack, it returns 0 as the bound. Otherwise, it fills the knapsack with the item with the greatest price/weight ratio which it computes in the function. 
@param i (int) represents the number of items
@param item (Item) item object that holds weight and profit for individual item
@param cap (int) is the total capacity of the knapsack
@param node (Node) is the node of which the function is computing the bound
**/
float compute_upper_bound(int i, Item *items, int cap, Node node){
	int a=0, b=0;	//a and b are used as indexes for the items array	
	int sumW = 0;	//Sum of the weight of the current items
	int bound = 0;	//bound of the input node
	
	if (node.weight >= cap){
		bound = 0;
	}
	else{
		bound = node.profit;
		a = node.height +1;
		sumW = node.weight;
		while((a < i) && ((sumW + items[a].weight) <= cap)){
			sumW += items[a].weight;
			bound += items[a].profit;
			a++;
		}
		b=a;
		if (b<i){
			bound += ((cap-sumW)*(items[b].profit/items[b].weight));
		}
	}
	return bound;
}

/**
bb_knapsack computes the optimal solution to the knapsack problem based on the upper bound of the node.


@param n (int) the number of items
@param item (Item) item object that holds weight and profit for individual item
@param cap (int) is the total capacity of the knapsack
@param put (ofstream) stream for writing to file
*/
int bb_knapsack(int n, Item *items, int cap, ofstream &out){
	int maxP = 0;	//Holds the maximum profit of the algorithm	
	priority_queue<Node> que;	//priority queue 

	//creates two nodes for traversing the tree
	Node root = Node();
	Node tempNode = Node();
	Node *optNode;	//holds node with optimal solution
	que.empty();

	//Initialze the root nodes values
	root.profit = 0; root.weight = 0; root.height = -1;
	root.upperBound = compute_upper_bound(n,items,cap,root);
	cout << root.upperBound << endl;
	root.parent = NULL;
	bool lFlag=false, rFlag = false; //boolean flags to check for leaves
	que.push(root);
	vector<Item> myKnapsack;	//vector to hold items in optimal knapsack
	int visitCount = 0, leafCount=0;	//counter to hold number of visited nodes and leaves of tree
	while(!que.empty()){
		lFlag = rFlag = false;
		//Takes node off queue		
		tempNode = que.top();
		que.pop();
	
		//cout << "L: " <<tempNode.height << " W: " <<tempNode.weight << " P: " << tempNode.profit << "B: "<<tempNode.upperBound<< endl;
		
		visitCount++;
		
		//Handle for when item can be placed in knapsack
		if ((tempNode.weight <= cap) && (tempNode.profit > maxP)){
			maxP = tempNode.profit;

			//set best
			optNode = &tempNode;
			//
			if (optNode->inOptSol) myKnapsack.push_back(items[tempNode.height]);
		}
		//left subtree
		tempNode.childL = new Node();
		(tempNode.childL)->parent = &tempNode;
		(tempNode.childL)->height = tempNode.height+1;
		(tempNode.childL)->weight = tempNode.weight + items[(tempNode.childL)->height].weight;
		(tempNode.childL)->profit = tempNode.profit + items[(tempNode.childL)->height].profit;
		(tempNode.childL)->upperBound = compute_upper_bound(n, items, cap, *(tempNode.childL));
		(tempNode.childL)->inOptSol = true;
		if ((tempNode).upperBound > maxP){
			que.push(*(tempNode.childL));
		}
		else{
			lFlag = true;
			
		}
		//right subtree
		tempNode.childR = new Node();
		(tempNode.childR)->parent = &tempNode;
		(tempNode.childR)->height = tempNode.height+1;
		(tempNode.childR)->weight = tempNode.weight;
		(tempNode.childR)->profit = tempNode.profit;
		(tempNode.childR)->inOptSol = false;
		(tempNode.childR)->upperBound = compute_upper_bound(n, items, cap, *(tempNode.childR));
		if ((tempNode).upperBound > maxP){
			que.push(*(tempNode.childR));
		}
		else{
			rFlag = true;
			
		}
		if (rFlag && lFlag){leafCount++;}
	}

	//Node *temp = optNode;
	/*while (optNode->height > 0){
		if (optNode->inOptSol == true){
			myKnapsack.push_back(items[optNode->height]);
		}		
		optNode = optNode->parent;
				
		//cout << temp->parent << endl;
	}*/
	
	//Writes formatted output to file
	out <<n <<','<<maxP<<','<< myKnapsack.size() << endl;
	out<< visitCount<<','<<leafCount << maxP << endl;
	for(unsigned int d=0; d<myKnapsack.size(); d++){
		out << myKnapsack[d].weight << ',' << myKnapsack[d].profit << endl; 	
	}
	return maxP;
}
























