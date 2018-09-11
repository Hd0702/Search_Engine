#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <iostream>
using namespace std;
template <typename K, class T>
/**
 * @brief The dstree class : this is my AVLtree data structure \n
 * built similarly to Dr. Weiss' AVL tree, but with implementation for keys and values\n
 * Weiss, Mark Allen. Data Structures and Algorithm Analysis in C . Pearson, 2014.
 */
class dstree
{
public:
    dstree();
    dstree(const dstree &);
    ~dstree();
    dstree & operator =(const dstree &);
    dstree & operator =( dstree &&);
    T& operator [](const K &);
    const K & findMin() const;
    bool contains(const K &x) const;
    bool isEmpty() const;
    void makeEmpty();
    void insert(const K & );
    void remove(const K &);
private:
    struct AvlNode
    {
        K element;
        T value{};
        AvlNode   *left;
        AvlNode   *right;
        int       height;

        AvlNode( const K & ele, AvlNode *lt, AvlNode *rt, int h = 0 )
          : element{ ele }, left{ lt }, right{ rt }, height{ h } { }
    };
    AvlNode *root;
    void insert(const K & , AvlNode * &);
    void remove(const K &, AvlNode * &);
    void balance(AvlNode *&);
    T & find(AvlNode *&, const K &);
    AvlNode * findMin(AvlNode *) const;
    bool contains(const K &, AvlNode *) const;
    void makeEmpty(AvlNode *&);
    AvlNode * copy(AvlNode *) const;
    int height(AvlNode *) const;
    int max(int, int ) const;
    void case1(AvlNode *& );
    void case2(AvlNode *&);
    void case3(AvlNode *&);
    void case4(AvlNode *&);
};
template <typename K, class T>
/**
 * @brief dstree<K, T>::insert: this is the private insert method \n
 * it takes the current node and the value, inserts it into the proper tree location
 * and then balances the tree
 * @param x : x is our key that we are searching for
 * @param t : this is our recursive Node
 */
void dstree<K,T>::insert( const K & x, AvlNode * & t ) //adds key without value
{
    if( t == nullptr )
        t = new AvlNode{ x, nullptr, nullptr };
    else if( x < t->element )
        insert( x, t->left );
    else if( t->element < x )
        insert( x, t->right );

    balance( t );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::remove : this is the private remove function\n
 * it takes a key and recursively moves the tree until we find the key, removes it then balances the tree
 * @param x : x is the key we are searching for
 * @param t : t is the recurisve node
 */
void dstree<K,T>::remove( const K & x, AvlNode * & t )
{
    //if the item does not exist
    if( t == nullptr )
        return;
    if( x < t->element )
        remove( x, t->left );
    else if( t->element < x )
        remove( x, t->right );
    else if( t->left != nullptr && t->right != nullptr ) // Two children
    {
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
    }
    else
    {
        AvlNode *oldNode = t;
        t = ( t->left != nullptr ) ? t->left : t->right;
        delete oldNode;
    }

    balance( t );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::balance : this is our balance function, it is called after every remove/insert
 * @param t : t is the recurisve node that is changed every cycle
 */
void dstree<K,T>::balance( AvlNode * & t )
{
    if( t == nullptr )
        return;

    if( height( t->left ) - height( t->right ) > 1 ) {
        if( height( t->left->left ) >= height( t->left->right ) )
            case1( t );
        else
            case2( t );
    }
    else {
        if( height( t->right ) - height( t->left ) > 1 ) {
            if( height( t->right->right ) >= height( t->right->left ) )
                case4( t );
            else
                case3( t );
        }
    }
    t->height = max( height( t->left ), height( t->right ) ) + 1;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::findMin: private minimum function\nmainly used for searching for remove function
 * @param t : t is the recursive node that we will surch down
 * @return : we are returning the lowest node in the current tree
 */
typename dstree<K,T>::AvlNode * dstree<K,T>::findMin( AvlNode *t ) const
{
    if( t == nullptr )
        return nullptr;
    if( t->left == nullptr )
        return t;
    return findMin( t->left );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::contains : this method searches for a key value and returns true or false if it exists
 * @param x : x is the key that we will be searching for
 * @param t : we will search using this Node and our value until we found one that matches, or when the
 * tree is empty
 * @return : we return true if the item exists, false if the item does not exist
 */
bool dstree<K,T>::contains( const K & x, AvlNode *t ) const
{
    if( t == nullptr )
        return false;
    else if( x < t->element )
        return contains( x, t->left );
    else if( t->element < x )
        return contains( x, t->right );
    else
        return true;    // Match, we found it
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::makeEmpty : this is our safe delete for every node
 * @param t : we will go through every node starting from the root downwards and delete it
 */
void dstree<K,T>::makeEmpty( AvlNode * & t )
{
    if( t != nullptr )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullptr;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::copy : copy method copys every pointer and creates a similar tree\n
 * like a copy constructor
 * @param t : our recursive node that we copy
 * @return : we will return the new node and attach new leaves to it
 */
typename dstree<K,T>::AvlNode * dstree<K,T>::copy( AvlNode * t ) const
{
    if( t == nullptr )
        return nullptr;
    else {
        AvlNode * h= new AvlNode{ t->element, copy( t->left ), copy( t->right ), t->height };
         h->value = t->value;
         return h;
    }
}
template <typename K, class T> /**
 * @brief dstree<K, T>::height : this essential assigns -1 to new nodes and otherwise we adjust our new height
 * @param t : our node we are checking the height of
 * @return : return the total heigh
 */
int dstree<K,T>::height( AvlNode *t ) const
{
    return t == nullptr ? -1 : t->height;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::max : compares two sides of a current node
 * @param lhs : total height on the left side
 * @param rhs : total heigh on the right side
 * @return  : return the larger size
 */
int dstree<K,T>::max( int lhs, int rhs ) const
{
    return lhs > rhs ? lhs : rhs;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::case1 : this is our LL rotate
 * it takes two different nodes and then it moves k2 down one and then changes both height values
 * then we assign new node value
 * @param k2 : k2 is our alpha that will be changed
 */
void dstree<K,T>::case1( AvlNode * & k2 )
{
    AvlNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::case4 : this is our RR rotation
 * it takes alpha and essentially dangles it to the left and changes its subtrees
 * @param k1 : k1 is our node that we will shift to the left
 */
void dstree<K,T>::case4( AvlNode * & k1 )
{
    AvlNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->height = max( height( k2->right ), k1->height ) + 1;
    k1 = k2;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::case2 : case this is our LR rotation, we need to fix the right nodes\n
 * and shift them in order before we do our LL rotation
 * @param k3 : this is our alpha node we will dangle
 */
void dstree<K,T>::case2( AvlNode * & k3 )
{
    case4( k3->left );
    case1( k3 );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::case3 : this is our case 3 rotation, we need to fix our right subtree order before we
 * shift alpha to a RR
 * @param k1 : this is alpha
 */
void dstree<K,T>::case3( AvlNode * & k1 )
{
    case1( k1->right );
    case4( k1 );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::find : this is our recursive searching function, we look for our node\n
 * key and then we return the value for editing
 * @param cNode : this is our recurisve node that we will check
 * @param data  : this is our key that we are looking for
 * @return : return our value by refrence
 */
T& dstree<K,T>::find(AvlNode * & cNode, const K & data) {
    if( cNode->element == data )
        return cNode->value;
    else if( data < cNode->element )
        return find( cNode->left, data );
    else if( cNode->element < data )
        return find(cNode->right, data );
    else {
        cerr << "item does not exist" << endl;
        throw;
    }
}

template <typename K, class T>
/**
 * @brief dstree<K, T>::dstree: our constructor makes an empty tree for adding on to later
 */
dstree<K,T>::dstree() : root{ nullptr }
{}
template <typename K, class T>
/**
 * @brief dstree<K, T>::dstree : we want to call our copy function for every node in our compare tree
 * @param rhs : this is our tree that we will be copying
 */
dstree<K,T>::dstree( const dstree & rhs ) :
    root{ nullptr }
{
  root = copy(rhs.root);
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::~dstree : the safest way to do this is to delete every node dynamically allocated
 * so we can also call the makeEmpty function
 */
dstree<K,T>::~dstree( )
{
  makeEmpty( );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::operator = : This operator takes two different trees, creates a temp, and places that into this object
 * @param rhs : this is our tree that we will be copying
 * @return : this object
 */
dstree<K,T> & dstree<K,T>::operator=(const dstree & rhs )
{
  dstree copy = rhs;
  std::swap( *this, copy );
  return *this;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::operator = : this is passing in the raw tree value, we just need to copy one data adress and what its doing
 * @param rhs : this is our tree that we will be swapping
 * @return  : this object
 */
dstree<K,T> & dstree<K,T>::operator=( dstree && rhs )
{
  std::swap( root, rhs.root );

  return *this;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::findMin : calls the findMin for the root value
 * @return  : return the node that is the smallest
 */
const K & dstree<K,T>::findMin( ) const
{
  if( isEmpty( ) )
      throw;
  return findMin( root )->element;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::contains : calls our contains private method from the root to recursively sort down the tree
 * until we found our key
 * @param x : our key we will be searching for
 * @return  : true if the item exists or false if the item does not exist
 */
bool dstree<K,T>::contains( const K & x ) const
{
  return contains( x, root );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::isEmpty : Checks and sees if the root has any branches, if it doesnt return true
 * @return : true if root is not connected to anything, false if it has connections
 */
bool dstree<K,T>::isEmpty( ) const
{
  return root == nullptr;
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::makeEmpty : this calls our make empty private function to recursively move down
 * the tree and delete every node
 */
void dstree<K,T>::makeEmpty( )
{
  makeEmpty( root );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::insert : takes a key value and recurisvely goes down tree until it finds value
 * or its proper location
 * @param x : key that is being inserted
 */
void dstree<K,T>::insert( const K & x)
{
  insert( x, root );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::remove : calls private recursion method to look in tree until we find our
 * key and then removes it and balances the tree
 * @param x : this is the key we are looking for
 */
void dstree<K,T>::remove( const K & x )
{
  remove( x, root );
}
template <typename K, class T>
/**
 * @brief dstree<K, T>::operator [] : calls private find method using key value from user input
 * @param data : this is the key we will be looking for recursively in tree
 * @return : return the value by refrence so we can edit it
 */
T& dstree<K,T>::operator [](const K& data) {
  return find(root, data);
}
#endif
