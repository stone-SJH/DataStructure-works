#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H


#include <iostream>        // For NULL
using namespace std;

// Red-black tree class
//
// CONSTRUCTION: with negative infinity object also
//               used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class RedBlackTree
{
  public:
    /**
     * Construct the tree.
     * negInf is a value less than or equal to all others.
     */
    explicit RedBlackTree( const Comparable & negInf )
    {
        nullNode    = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;
        header      = new RedBlackNode( negInf );
        header->left = header->right = nullNode;
    }

    RedBlackTree( const RedBlackTree & rhs )
    {
        nullNode    = new RedBlackNode;
        nullNode->left = nullNode->right = nullNode;
        header      = new RedBlackNode( rhs.header->element );
        header->left = header->right = nullNode;
        *this = rhs;
    }

    ~RedBlackTree( )
    {
        makeEmpty( );
        delete nullNode;
        delete header;
    }

    const Comparable & findMin( ) const
    {
      if( isEmpty( ) ) {}
	  

        RedBlackNode *itr = header->right;

        while( itr->left != nullNode )
            itr = itr->left;

        return itr->element;
    }

    const Comparable & findMax( ) const
    {
      if( isEmpty( ) ){}

        RedBlackNode *itr = header->right;

        while( itr->right != nullNode )
            itr = itr->right;

        return itr->element;
    }

    bool contains( const Comparable & x ) const
    {
        nullNode->element = x;
        RedBlackNode *curr = header->right;

        for( ; ; )
        {
            if( x < curr->element )
                curr = curr->left;
            else if( curr->element < x )
                curr = curr->right;
            else
                return curr != nullNode;
        }
    }

    bool isEmpty( ) const
    {
        return header->right == nullNode;
    } 

    void printTree( ) const
    {
        if( header->right == nullNode )
            cout << "Empty tree" << endl;
        else
            printTree( header->right );
        cout<<endl;
    }
    void printRoot() const
    {
        cout<<"root:"<<header->right->element<<endl;
    }
    void makeEmpty( )
    {
        reclaimMemory( header->right );
        header->right = nullNode;
    }

    /**
     * Insert item x into the tree. Does nothing if x already prsent.
     */
    void insert( const Comparable & x )
    {
        current = parent = grand = header;
        nullNode->element = x;

        while( current->element != x )
        {
            great = grand; grand = parent; parent = current;
            current = x < current->element ?  current->left : current->right;

                // Check if two red children; fix if so
            if( current->left->color == RED && current->right->color == RED )
                handleReorient( x );
        }

            // Insertion fails if already present
        if( current != nullNode )
            return;
        current = new RedBlackNode( x, nullNode, nullNode );

            // Attach to parent
        if( x < parent->element )
            parent->left = current;
        else
            parent->right = current;
        handleReorient( x );
    }

    void remove( const Comparable & x )
    {
      /*
	Your code here.
	
       */ 
	if(header->right->left->parent == NULL) set_parent();
	RedBlackNode *assistant_1 = find(x);	
        RedBlackNode *cur;
	RedBlackNode *assistant_2; 
        if (assistant_1->left == nullNode || assistant_1->right == nullNode) assistant_2 = assistant_1;
	else assistant_2 = get_successor(assistant_1);
	if (assistant_2->left != nullNode) cur = assistant_2->left;
	else cur = assistant_2->right;
	cur->parent = assistant_2->parent;
	if (assistant_2->parent == nullNode)
		header->right = cur; 
	else if (assistant_2 == assistant_2->parent->left)
		assistant_2->parent->left = cur;
	else 
		assistant_2->parent->right = cur;
	if (assistant_2 != assistant_1){ 
		//cout<<assistant_1->element<<" "<<assistant_2->element<<endl;
		assistant_1->element = assistant_2->element;
	}
        //cout<<assistant_2->color<<endl;
        //if (cur != nullNode) cout<<cur->element<<endl;
        //else cout<<"error!nullnode!"<<endl;
	if (assistant_2->color == BLACK){ 
		delete_fixup(cur);
	        //cout<<"finish fixup"<<endl;
        } 
    }
	int countTree(){
		return recursive_countTree(header->right);
	}
	
		
    enum { RED, BLACK };

    const RedBlackTree & operator=( const RedBlackTree & rhs )
    {
        if( this != &rhs )
        {
            makeEmpty( );
            header->right = clone( rhs.header->right );
        }

        return *this;
    }

  private:
    struct RedBlackNode
    {
        Comparable    element;
        RedBlackNode *left;
        RedBlackNode *right;
	RedBlackNode *parent;
        int           color;

        RedBlackNode( const Comparable & theElement = Comparable( ),
                            RedBlackNode *lt = NULL, RedBlackNode *rt = NULL,
                            int c = BLACK )
          : element( theElement ), left( lt ), right( rt ), color( c ) { }
    };

    RedBlackNode *header;   // The tree header (contains negInf)
    RedBlackNode *nullNode;

        // Used in insert routine and its helpers (logically static)
    RedBlackNode *current;
    RedBlackNode *parent;
    RedBlackNode *grand;
    RedBlackNode *great;

//my added method
//stone
   	RedBlackNode *findTree( const Comparable & x){
      	  	nullNode->element = x;
      	  	RedBlackNode *curr = header->right;
     	  	 for(;;){
       	        	 if (x < curr->element) curr = curr->left;
        	   	     else if (curr->element < x) curr = curr->right;
        	    	    else return curr;
       		 }
 	}
	int recursive_countTree(RedBlackNode *knode){
		int n1, n2;
		if (knode == NULL ||  knode == nullNode) return 0;
		else{
			n1 = recursive_countTree(knode->left);
			n2 = recursive_countTree(knode->right);
			return n1+n2+1;
		}
	}
	RedBlackNode* find( Comparable x){
		RedBlackNode *node = recursive_find(x, header->right);
		return node;
	}
	RedBlackNode* recursive_find( Comparable x, RedBlackNode *start ){
		if (start != nullNode){
			if ( start->element > x ){
				start = recursive_find (x, start->left);
			}
			else if ( start->element < x ){
				start = recursive_find (x, start->right);
			}
			else if ( start->element == x){
				return start;
			}
		}
		//return start;
	}
	void set_parent(){
		nullNode->parent = nullNode;
		header->right->parent = header;
		recursive_set_parent(header->right);
		return;
	}
	void recursive_set_parent( RedBlackNode *node){
		if (node == nullNode) return;
		node->left->parent = node;
		node->right->parent = node;
		if (node->left != nullNode) recursive_set_parent(node->left);
		if (node->right != nullNode) recursive_set_parent(node->right);
		return;
	}
	void left_rotate( RedBlackNode *x){
		RedBlackNode *y = x->right;
		x->right = y->left;
		if (y->left != nullNode)
			y->left->parent = x;
		y->parent = x->parent;
		if (x->parent == nullNode)
			header->right = y;
		else{
			if ( x == x->parent->left ) x->parent->left = y;
			else x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}
	void right_rotate( RedBlackNode *x){
		RedBlackNode *y = x->left;
		x->left = y->right;
		if (y->right != nullNode)
			y->right->parent = x;
		y->parent = x->parent;
		if (x->parent == nullNode)
			header->right = y;
		else{
			if ( x == x->parent->right ) x->parent->right = y;
			else x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}
	void delete_fixup( RedBlackNode *x ){
		RedBlackNode * w;
		while ( x != header->right && x->color == BLACK){		
			if ( x == x->parent->left ){
                                //cout<<1<<endl;
				w = x->parent->right;
				if(w->color == RED){
					w->color = BLACK;
					x->parent->color = RED;
					left_rotate(x->parent);
					w = x->parent->right;
				}
				if ((w->left != nullNode || w->left->color == BLACK) && (w->right != nullNode || w->right->color == BLACK)){
					x -> color = RED;
					x = x->parent;
				}
				else{
					if (w->right !=nullNode || w->right->color == BLACK){
						w->left->color = BLACK;
 						w->color = RED;
 						right_rotate(w);
						w = x->parent->right;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					left_rotate(x->parent);
			 		x = header->right;
				}
			}
			else{
                                //cout<<2<<endl;
				w = x->parent->left;
				if(w->color == RED){
					w->color = BLACK;
					x->parent->color = RED;
					right_rotate(x->parent);
					w = x->parent->left;
				}
				if ((w->right != nullNode || w->right->color == BLACK) && (w->left != nullNode || w->left->color == BLACK)){
					x -> color = RED;
					x = x->parent;
				}
				else{
					if (w->left != nullNode || w->left->color == BLACK){
						w->right->color = BLACK;
 						w->color = RED;
 						left_rotate(w); 
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					right_rotate(x->parent);
				 	x = header->right;
				}
			}	
		}
		x->color = BLACK;			
	}
	RedBlackNode *get_min( RedBlackNode * x){
		while (x->left != nullNode){
			x = x->left;
		}
		return x;
	}
	RedBlackNode *get_successor( RedBlackNode * x){
		RedBlackNode * y;
		if (x->right != nullNode)
			return get_min(x->right);
		y = x->parent;
		while (y != nullNode && x == y->right){
 			x = y;
			y = y->parent;
		}
		return y;
	}
	
        // Usual recursive stuff
    void reclaimMemory( RedBlackNode *t ){
        /*if( t != t->left )
        {
            reclaimMemory( t->left );
            reclaimMemory( t->right );
            delete t;
        }*/
	delete t;
    }
//stone end  
    void printTree( RedBlackNode *t ) const
    {
        if( t != t->left )
        {
            printTree( t->left );
            cout << t->element << " "/*endl*/;
            printTree( t->right );
        }
    }
 
    RedBlackNode * clone( RedBlackNode * t ) const
    {
        if( t == t->left )  // Cannot test against nullNode!!!
            return nullNode;
        else
            return new RedBlackNode( t->element, clone( t->left ),
                                     clone( t->right ), t->color );
    }

        // Red-black tree manipulations
    /**
     * Internal routine that is called during an insertion if a node has two red
     * children. Performs flip and rotatons. item is the item being inserted.
     */
    void handleReorient( const Comparable & item )
    {
            // Do the color flip
        current->color = RED;
        current->left->color = BLACK;
        current->right->color = BLACK;

        if( parent->color == RED )   // Have to rotate
        {
            grand->color = RED;
            if( item < grand->element != item < parent->element )
                parent = rotate( item, grand );  // Start dbl rotate
            current = rotate( item, great );
            current->color = BLACK;
        }
        header->right->color = BLACK; // Make root black
    }

    /**
     * Internal routine that performs a single or double rotation.
     * Because the result is attached to the parent, there are four cases.
     * Called by handleReorient.
     * item is the item in handleReorient.
     * theParent is the parent of the root of the rotated subtree.
     * Return the root of the rotated subtree.
     */
    RedBlackNode * rotate( const Comparable & item, RedBlackNode *theParent )
    {
        if( item < theParent->element )
        {
            item < theParent->left->element ?
                rotateWithLeftChild( theParent->left )  :  // LL
                rotateWithRightChild( theParent->left ) ;  // LR
            return theParent->left;
        }
        else
        {
            item < theParent->right->element ?
                rotateWithLeftChild( theParent->right ) :  // RL
                rotateWithRightChild( theParent->right );  // RR
            return theParent->right;
        }
    }

    void rotateWithLeftChild( RedBlackNode * & k2 )
    {
        RedBlackNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    void rotateWithRightChild( RedBlackNode * & k1 )
    {
        RedBlackNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2; 
    }
};

#endif
