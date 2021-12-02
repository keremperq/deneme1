/******************************************************************************/
/* Student Name: Kerem Berk Guclu                                             */
/* Student ID: 150160058                                                      */
/* Date: 10/01/2021                                                           */
/******************************************************************************/
/* main.cpp                                                                   */
/******************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <tuple> //returning multiple values from a function


using namespace std;

struct node
{
    string name;
    int point, rebound, assist;

    string color;
    node *left, *right, *parent;
    node();
    node(string name, int rebound, int assist, int point, string color);

};
node::node()
{
    this->name = '\0';
    this->rebound = 0;
    this->assist = 0;
    this->point = 0;
    this->color = '\0';

    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
}

node::node(string name, int rebound, int assist, int point, string color = "RED")
{
    this->name = name;
    this->rebound = rebound;
    this->assist = assist;
    this->point = point;
    this->color = color;

    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;

}

node* root; //initialize a global pointer for root

void insert_case1(node*);
void insert_case2(node*);
void insert_case3(node*);
void insert_case4(node*);
void insert_case5(node*);
 
node* grandparent(node* x) {    //gets the grandparent of the given node in the RB tree
    if (x == NULL || x->parent == NULL)
        return NULL;
    return x->parent->parent;
}

node* sibling(node* x) {        //gets the sibling of the given node in the RB tree
    if (x == NULL || x->parent == NULL)
        return NULL;
    if (x == x->parent->left)
        return x->parent->right;
    else
        return x->parent->left;
}

node* uncle(node* x) {  //gets the uncle of the given node in the RB tree
    if (x == NULL)
        return NULL;
    return sibling(x->parent);
}

string node_color(node* x) {        //gets the color of the given node in the RB tree
    if (x == NULL)
        return "BLACK";
    return x->color;
}


void right_rotate(node* y) {	//right rotate subtree rooted with y
    
    if (y == NULL || y->left == NULL)	// check if the given node and it's left branch are NULLs
        return;
    node* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    // replace node y with node x
    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->parent = y->parent;
    // perform rotation
    x->right = y;
    y->parent = x;
}

void left_rotate(node* x) {		//left rotate subtree rooted with x
    
    if (x == NULL || x->right == NULL)	// check if the given node and it's left branch are NULLs
        return;
    node* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    // replace node x with node y
    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->parent = x->parent;
    // perform rotation
    y->left = x;
    x->parent = y;
}

void insert_case1(node* x) {
    if (x->parent == NULL)
        x->color = "BLACK";
    else
        insert_case2(x);
}
void insert_case2(node* x) {
    if (node_color(x->parent) == "BLACK")
        return;
    else
        insert_case3(x);
}
void insert_case3(node* x) {
    if (node_color(uncle(x)) == "RED") {
        x->parent->color = "BLACK";
        uncle(x)->color = "BLACK";
        grandparent(x)->color = "RED";
        insert_case1(grandparent(x));
    }
    else
        insert_case4(x);
}
void insert_case4(node* x) {
    if (x == x->parent->right && x->parent == grandparent(x)->left) {
        left_rotate(x->parent);
        x = x->left;
    }
    else if (x == x->parent->left && x->parent == grandparent(x)->right) {
        right_rotate(x->parent);
        x = x->right;
    }
    insert_case5(x);
}
void insert_case5(node* x) {
    x->parent->color = "BLACK";
    grandparent(x)->color = "RED";
    if (x == x->parent->left && x->parent == grandparent(x)->left)
        right_rotate(grandparent(x));
    else
        left_rotate(grandparent(x));
}

void insert_node(node* data) {	// This function inserts a new node with given key in the Red Black tree
    // create a new node
    node* new_node = new node();
    new_node->name = data->name;
    new_node->point = data->point;
    new_node->rebound = data->rebound;
    new_node->assist = data->assist;
    new_node->color = "RED";
    
    if (root == NULL) {	// if the tree is empty
        root = new_node;
        root->color = "BLACK";
        return;
    }
    node* current = root;
    while (current != NULL) {
        // repeat the same definition of insert at left subtrees
        if (data->name < current->name) {
            if (current->left == NULL) {
                current->left = new_node;
                break;
            }
            current = current->left;
        }
        // repeat the same definition of insert at right subtrees
        else if (data->name > current->name) {
            if (current->right == NULL) {
                current->right = new_node;
                break;
            }
            current = current->right;
        }
        // duplicate keys are not allowed in the Red Black tree
        else
            return;
    }
    new_node->parent = current;
    insert_case1(new_node);
}

// This function searches if the given data in the Red Black tree
tuple<node*,bool>  search(node* current, string data) {
    // base case we reach a null node
    if (current == NULL)
        return make_tuple(current,false);
    // check if we find the data at the current node
    if (current->name == data)
        return make_tuple(current,true);
    // repeat the same definition of search at left or right subtrees
    if (data < current->name)
        return search(current->left, data);
    else
        return search(current->right, data);
}


void printTree(node *curr, int level){ //preorder traversal
	if (curr != NULL){
		for (int i = 0; i < level; i++){
			cout << "-";
		}
		cout << "(" << curr->color << ") " << curr->name <<endl;
		level++;
		printTree(curr->left, level);
		printTree(curr->right, level);
	}
}


node* findMax(node* curr,int choice) //choice 1: rebound, choice 2: assist, choice 3: point 
{
    node* temp = new node();
    if(choice == 1)
    {
        node* res = curr; 
        if (res == NULL) 
            return temp;

        
        node* lres = findMax(res->left,1); 
        node* rres = findMax(res->right,1); 
        if (lres->rebound > res->rebound) 
            res = lres; 
        if (rres->rebound > res->rebound) 
            res = rres; 
        return res; 
    }
    else if(choice == 2)
    {
        node* res = curr; 
        if (res == NULL) 
            return temp; 

        
        node* lres = findMax(res->left,2); 
        node* rres = findMax(res->right,2); 
        if (lres->assist > res->assist) 
            res= lres; 
        if (rres->assist > res->assist) 
            res = rres; 
        return res;
    }
    else
    {
        node* res = curr; 
        if (res == NULL) 
            return temp; 

        
        node* lres = findMax(res->left,3); 
        node* rres = findMax(res->right,3); 
        if (lres->point > res->point) 
            res = lres; 
        if (rres->point > res->point) 
            res = rres; 
        return res;
    }
    
} 

//print the max values according to season
void print_max(node* curr,string season_name)
{
    node* temp = new node();
    cout << "End of the " << season_name << " " << "Season" << endl;
    temp = findMax(curr,3);
    cout << "Max Points: " << temp->point << " - " << "Player Name: " << temp->name << endl;
    temp = findMax(curr,2);
    cout << "Max Assists: " << temp->assist << " - " << "Player Name: " << temp->name  << endl;
    temp = findMax(curr,1);
    cout << "Max Rebs: " << temp->rebound << " - " << "Player Name: " << temp->name << endl;
}

int main(int argc, char *argv[])
{
    ifstream file;
    
    file.open(argv[1]);

    if (!file) {
        cerr << "File cannot be opened.";
        exit(1); 
    }
    
    string line;
    getline(file,line); //this is the header line

    string season_name;
    string name;
    string team;
    string comma; //to read values seperated with comma
    char temp[9] = "";  //sadece ilk sezonun ismini tutmak ve daha sonra agaci bastirmak icin kullaniliyor
    char c;
    for(int j = 0;j < 9;j++){   //karakter karakter okuyup listeye atiyorum
        file.get(c);
        temp[j] = c;
    }
    string str(temp);   //char listesini stringe cevir
    string temp_season = temp;
    int rebound, assist, point;
    
    int i = 0;
    while(!file.eof())
    {
        getline(file, season_name, ',');
        
        string s0 (temp_season);
        string s2 (s0.begin()+5,s0.begin()+9);
        string s3 (season_name);
        string s4 (s3.begin(),s3.begin()+4);
        if(s2 == s4)    //sezon degisiminde onceki sezonun 2.yili ile yeni sezonun 1.yilini karsilastirip ayni ise sezon bitmistir ve max'lari bastir
        {
            i++;
            print_max(root,temp_season);
        }
        if(i == 1)  //sadece ilk sezonun listesini bastir
        {
            printTree(root,0);
            i++;    //bir kere bastirdiktan sonra bir daha bastirmamasi icin
        }
        getline(file, name, ',');
        getline(file, team, ',');
        file>>rebound;
        getline(file, comma, ',');
        file>>assist;
        getline(file, comma, ',');
        file>>point;
        getline(file, line, '\n');
        
		node *X = new node(name,rebound,assist,point);
        node *temp_node = root;
        bool temp_bool;
        tie(temp_node,temp_bool) = search(temp_node,X->name); //var olan oyuncuyu kontrol ederken node'u elimizde tutmak icin cift deger donduren "tie" metodunu kullandım
        if(temp_bool == true)   //eger oyuncu var ise node'u guncelle
        {
            temp_node->rebound += X->rebound;
            temp_node->assist += X->assist;
            temp_node->point += X->point;
        }
        else    //oyuncu yok ise yeni node ekle
        {
            insert_node(X);
        }
        if(file.eof() == true){	//dosyanin sonuna gelindiyse max'ı bastir
            print_max(root,temp_season);
        }
        temp_season = season_name;
        
    }

    file.close();
    return 0;
}