#include<iostream>
using namespace std;

class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);

private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();
private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->right = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
    node *p = root, *previous_p = NULL;
    //find the position to insert
    while(p && p!=head && p!=tail){
        previous_p = p;
        if(s > p->number && p->is_threadr==0) p=p->right;
        else if(s < p->number && p->is_threadl==0) p=p->left;
        else if(s == p->number) ;//ignore
        else break;
    }
    //perform insertion
    num++;
    p = new node(s);
    if(root){//not empty tree
        if(s > previous_p->number){//insert as right child
            p -> right = previous_p ->right;
            p -> left = previous_p;
            previous_p -> is_threadr = 0;  previous_p ->right = p;
            if(p->right == tail){
                tail->left = p;
                p->is_threadr=-1;
            }
        }
        else if(s < previous_p->number){//insert as left child
            p -> left = previous_p -> left;
            p -> right = previous_p;
            previous_p -> is_threadl = 0;  previous_p ->left = p;
            if(p->left == head){
                head->left = p;
                p->is_threadl=-1;
            }
        }
        else return;//number s already exist, ignore
    }
    else{//empty tree(the first node)
        p -> is_threadr = -1;//if p link to head/tail, p->is_thread = -1
        p -> right = tail;
        p -> is_threadl = -1;
        p -> left = head;
        head->left = p;
        tail->left = p;
        root = p;
    }
}

void op_tree_totum::deletion(int s){
    if(root==0) return;//empty tree, ignore

    node *p=root, *parent=tail, *tmp;
   //find the position of number s, ans its parent
    while(p!=head && p!=tail){
        parent=p;
        if(s>p->number){
            if(p->is_threadr!=0) return;//s does not exist, ignore
            p=p->right;
        }
        else if(s<p->number){
            if(p->is_threadl!=0) return;//s does not exist, ignore
            p=p->left;
        }
        if(s==p->number) break;
    }
    if(p==head || p==tail) return;//s does not exist, ignore

    //perform deletion
    num--;
    if(p==root){
        if(p->is_threadl==-1 && p->is_threadr==-1) root=0;//empty

        else if(p->is_threadl==0 && p->is_threadr==0){//p has 2 child
            //biggest is the biggest node in p's left subtree
            node* biggest = p->left;
            node *biggest_parent;
            while(biggest->is_threadr==0){
                biggest_parent=biggest;
                biggest=biggest->right;
            }
            //tmp is the smallest node in p's right subtree
            tmp = p->right;
            while(tmp->is_threadl==0) tmp=tmp->left;

            //replace p with biggest
            if(biggest->is_threadl!=0){//biggest has no child
                root=biggest;
                biggest->right=p->right;
                biggest->is_threadr=0;
                tmp->left = biggest;

                if(p->left != biggest){
                    biggest->left=p->left;
                    biggest->is_threadl=0;
                    biggest_parent->right=biggest;
                    biggest_parent->is_threadr=1;
                }
            }
            else{//biggest has one left child
                root=biggest;
                biggest->right=p->right;
                biggest->is_threadr=0;
                tmp->left = biggest;
                if(p->left!=biggest){
                    biggest_parent->right=biggest->left;
                    biggest->left=p->left;
                }
            }
        }
        else if(p->is_threadl==-1){//1 right child
            root=p->right;
            tmp = p->right;
            while(tmp->is_threadl==0)tmp=tmp->left;//make tmp to be the leftmost node
            head->left=tmp;
            tmp->left = p->left;
            tmp->is_threadl = p->is_threadl;
        }
        else if(p->is_threadr==-1){//1 left child
            root=p->left;
            tmp = p->left;
            while(tmp->is_threadr==0) tmp=tmp->right;//make tmp to be the rightmost node
            tail->left=tmp;
            tmp->right = p->right;
            tmp->is_threadr = p->is_threadr;
        }
    }
    else if(p->left==head){//(p must be a left child)
        if(p->is_threadr){
            parent->left=head;
            parent->is_threadl=-1;
            head->left=parent;
        }
        else{
            parent->left=p->right;
            node *smallest=p->right;
            while(smallest->is_threadl==0) smallest=smallest->left;
            smallest->left=head;
            smallest->is_threadl=-1;
            head->left=smallest;
        }
    }
    else if(p->right==tail){//(p must be a right child)
        if(p->is_threadl){
            parent->right=tail;
            parent->is_threadr=-1;
            tail->left=parent;
        }
        else{
            parent->right=p->left;
            node *biggest=p->left;
            while(biggest->is_threadr==0) biggest=biggest->right;
            biggest->right=tail;
            biggest->is_threadr=-1;
            tail->left=biggest;
        }
    }
    else if(parent->left==p){//p is a left child
        if(p->is_threadl==0 && p->is_threadr==0){//p has 2 child
            //biggest is the biggest node in p's left subtree
            node* biggest = p->left;
            node *biggest_parent;
            while(biggest->is_threadr==0){
                biggest_parent=biggest;
                biggest=biggest->right;
            }
            //tmp is the smallest node in p's right subtree
            tmp = p->right;
            while(tmp->is_threadl==0) tmp=tmp->left;

            //replace p with biggest
            if(biggest->is_threadl){//biggest has no child
                parent->left=biggest;
                biggest->right=p->right;
                biggest->is_threadr=0;
                tmp->left = biggest;

                if(p->left != biggest){
                    biggest->left=p->left;
                    biggest->is_threadl=0;
                    biggest_parent->right=biggest;
                    biggest_parent->is_threadr=1;
                }
            }
            else{//biggest has one left child
                parent->left=biggest;
                biggest->right=p->right;
                biggest->is_threadr=0;
                tmp->left = biggest;
                if(p->left!=biggest){
                    biggest_parent->right=biggest->left;
                    biggest->left=p->left;
                }
            }
        }
        else if(p->is_threadl && p->is_threadr){//p has no child
            parent->is_threadl=1;
            parent->left=p->left;
        }
        else if(p->is_threadl && p->is_threadr==0){//p has 1 right child
            parent->left=p->right;
            tmp = p->right;
            while(tmp->is_threadl==0)tmp=tmp->left;//make tmp to be the leftmost node
            tmp->left = p->left;
        }
        else if(p->is_threadl==0 && p->is_threadr){//p has 1 left child
            parent->left=p->left;
            tmp = p->left;
            while(tmp->is_threadr==0) tmp=tmp->right;//make tmp to be the rightmost node
            tmp->right = p->right;
        }
    }
    else if(parent->right==p){//p is a right child
        if(p->is_threadl==0 && p->is_threadr==0){//p has 2 child
            //biggest is the biggest node in p's left subtree
            node* biggest = p->left;
            node *biggest_parent;
            while(biggest->is_threadr==0){
                biggest_parent=biggest;
                biggest=biggest->right;
            }
            //tmp is the smallest node in p's right subtree
            tmp = p->right;
            while(tmp->is_threadl==0) tmp=tmp->left;

            //replace p with biggest
            if(biggest->is_threadl){//biggest has no child
                parent->right=biggest;
                biggest->right=p->right;
                biggest->is_threadr=0;
                tmp->left = biggest;

                if(p->left != biggest){
                    biggest->left=p->left;
                    biggest->is_threadl=0;
                    biggest_parent->right=biggest;
                    biggest_parent->is_threadr=1;
                }
            }
            else{//biggest has one left child
                parent->right=biggest;
                biggest->right=p->right;
                biggest->is_threadr=0;
                tmp->left = biggest;
                if(p->left!=biggest){
                    biggest_parent->right=biggest->left;
                    biggest->left=p->left;
                }
            }
        }
        else if(p->is_threadl && p->is_threadr){//p has no child
            parent->is_threadr=1;
            parent->right=p->right;
        }
        else if(p->is_threadl && p->is_threadr==0){//p has 1 right child
            parent->right=p->right;
            tmp = p->right;
            while(tmp->is_threadl==0)tmp=tmp->left;//make tmp to be the leftmost node
            tmp->left = p->left;
        }
        else if(p->is_threadl==0 && p->is_threadr){//p has 1 left child
            parent->right=p->left;
            tmp = p->left;
            while(tmp->is_threadr==0) tmp=tmp->right;//make tmp to be the rightmost node
            tmp->right = p->right;
        }
    }
    delete p;
}

void op_tree_totum::inorder_run(){
    if(root==0){//empty
        printf("\n");
        return;
    }

    node *p = head->left;//start from head to tail
    printf("%d",p->number);
    while(1){
        if(p->is_threadr==0){//not thread, move to the smallest node of p's right subtree
            p=p->right;
            while(p->is_threadl==0) p=p->left;
        }
        else p=p->right;//thread, simply go to right

        if(p==tail) break;//if move to thread, end
        else printf(" %d",p->number);
    }
    printf("\n");
}

void op_tree_totum::reverseorder_run(){
    if(root==0){//empty
        printf("\n");
        return;
    }

    node *p = tail;//start from tail to head
    p = p->left;
    printf("%d",p->number);
    while(1){
        if(p->is_threadl==0){//not thread, move to the biggest node of p's left subtree
            p=p->left;
            while(p->is_threadr==0) p=p->right;
        }
        else p=p->left;

        if(p==head) break;//if move to head, end
        else printf(" %d",p->number);
    }
    printf("\n");
}

int op_tree_totum::size(){
	return num;
}
