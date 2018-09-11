#ifndef HASHMAP_H
#define HASHMAP_H
#include <list>
#include <functional>
#include <utility>
#include <iostream>
using namespace  std;
template<typename key, typename value>
/**
 * @brief The hash_map class : Same methods as the AVL tree
 * this allows for easy implementation in the index files
 */
class hash_map {
public:
    struct Node {
        key first;
        value second;
    };

    hash_map();
    hash_map(const   hash_map &);
    ~hash_map();
    void makeEmpty();
    hash_map & operator=(const  hash_map &);
    bool contains(const  key &);
    bool isEmpty() const;
    void insert(const  key &);
    void remove(const  key &);
    value& operator[](const  key &);

private:
    size_t setHash(const  key &);
    void resize();
    int capacity;
    int size;
    list<pair<key,value>> * data;

};
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::makeEmpty:
 * this clears every single value in data and resets size
 */
void hash_map<key,value>::makeEmpty() {
    for(int i =0; i < capacity; i ++) {
        data[i].clear();
    }
    size = 0;
}

template<typename key, typename value>
/**
 * @brief hash_map<key, value>::hash_map : default sets array of list for easy chaining
 */
hash_map<key,value>::hash_map() {
    capacity = 10000; //default capacity
    size = 0;
    data = new list<pair<key,value>>[capacity];
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::hash_map : this goes through every item in one hash_map and copies it
 * into another hash_map
 * @param copy : this is the item that we will be copying from
 */
hash_map<key,value>::hash_map(const hash_map & copy) {
    capacity = copy.capacity;
    size = copy.size;
    data = new list<pair<key,value>>[capacity];
    for(int i =0; i < capacity; i ++) {
        data[i] = copy.data[i];
    }
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::contains : this looks for an item in a hash_map by hashing it and
 * checking the list the item might be in
 * @param look : this is the key value we are looking for
 * @return : true means that we found the value and false means we did not find it
 */
bool hash_map<key,value>::contains(const key & look) {
    size_t Hsh = setHash(look);
    for(auto iter = data[Hsh].begin(); iter != data[Hsh].end(); iter++){
        if (iter->first == look)
            return true;
    }
    return false;
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::isEmpty : this simply checks the size and returns true/false corresponding to it
 * @return : true if the size is 0, flase if the size is not 0
 */
bool hash_map<key,value>::isEmpty() const{
   return (size == 0) ? true : false;
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::remove : this searches for an item that might exist
 * in a hash_map and removes it from the list
 * @param look : look is the key value that we will be searching for
 */
void hash_map<key,value>::remove(const key & look) {
    size_t loc = setHash(look);
    for(auto iter = data[loc].begin(); iter != data[loc].end(); iter ++) {
          if (iter->first == look) {
            data[loc].remove(*iter);
            size--;
            break;
        }
    }
}

template<typename key, typename value>
/**
 * @brief hash_map<key, value>::operator =: this resets all the data in current object and copies
 * all the data from the copy item
 * @param copy : this is the hash_map that we will be copying
 * @return : we will be returning this object
 */
hash_map<key,value> & hash_map<key,value>::operator =(const hash_map & copy) {
    delete [] data;
    size = copy.size;
    capacity = copy.capacity;
    data = new list<pair<key,value>>[capacity];
    int counter = 0;
    for(auto iter = data->begin(); iter != data->end(); iter ++){ //every list
        data[counter] = copy.data[counter];
        counter++;
    }
    return *this;
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::~hash_map : this deletes all of our data and resets parameters
 */
hash_map<key,value>::~hash_map() {
    delete [] data;
    size = 0;
    capacity = 10000;
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::operator []: this returns an editable value from the bracket operator
 * if an item exists
 * @param look : we will be searching for this key value in our hash_map
 * @return : we will return the value by refrence for editing
 */
value & hash_map<key,value>::operator [](const key & look) {
    size_t hsh = setHash(look);
    try {
        for(auto iter = data[hsh].begin(); iter != data[hsh].end(); iter ++) {
            if(iter->first == look)
                return iter->second;
        }
    }
    catch(exception & e) {
        std::cerr << "Out of Range Exception" << e.what() << endl;
    }
    exit(EXIT_FAILURE);
}

template<typename key, typename value>
/**
 * @brief hash_map<key, value>::insert : this hashes our ins member at stores it at the end of
 * the list in that particular hash location
 * @param ins : this is the item that we will be inserting in our hash_map
 */
void hash_map<key,value>::insert(const  key & ins) {
    size_t hash_find = setHash(ins);
    pair<key,value> place;
    place.first = ins;
    data[hash_find].push_back(place);
    size++;
    if(size == (capacity * .75)) { //we want to go and resize our entire table this is very timely operation
        resize();
    }
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::resize: this resize method is very taxing:
 *  it takes every item from every list, rehashes it, and stores it into a new data item 10 times the size
 */
void hash_map<key,value>::resize() {
    capacity  *= 10;
    list<pair<key,value>> * temp = data;
    data = new list<pair<key,value>>[capacity];
    for(int i =0; i <capacity/10; i ++) {
        for(auto iter = temp[i].begin(); iter != temp[i].end(); iter++) {
            size_t hsh = setHash(iter->first);
            data[hsh].push_back(*iter);
        }
    }
    delete [] temp;
}
template<typename key, typename value>
/**
 * @brief hash_map<key, value>::setHash: this is our hashing function, we will be using the stl hash
 * we hash our item and then mod it by the total array size
 * @param look : this is the key that we will be hashing
 * @return : this is the index for our data object that has been hashed
 */
size_t hash_map<key,value>::setHash(const key & look) {
    hash<key> key_hash;
    size_t hash_find = key_hash(look); //this is our hash value
    hash_find = hash_find % capacity;  //now mod it to fit in the table
    return hash_find;
}

#endif // HASHMAP_H
