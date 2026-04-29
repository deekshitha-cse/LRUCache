#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

// LRU Cache using HashMap + Doubly Linked List for O(1) operations
class Node {
public:
    // Stores cache entry and links for Doubly Linked List navigation
    int key;
    int value;
    Node* prev;
    Node* next;
    Node(int k, int v) {
        key = k;
        value = v;
        prev = nullptr;
        next = nullptr;
    }
};

class LRUCache {
private:
    int capacity;
    unordered_map <int, Node*> map;

    Node* head;
    Node* tail;

    // Removes node from current position
    void removeNode(Node* node) {
        if(!node || node == head || node == tail) return;
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // Insert node as most recently used
    void addToFront(Node* node) {
        node->prev = head;
        node->next = head->next;
        head->next = node;
        node->next->prev = node;
    }

    public:

    // Access value and mark as recently used
    int get(int key) {
        if(map.find(key) == map.end()) return -1;
        Node* node = map[key];
        removeNode(node);
        addToFront(node);
        return node->value;
    }

    // Insert/update cache entry with LRU eviction
    void put(int key, int value) {
        if(capacity == 0) return;

        if(map.find(key) != map.end()) {
            Node* node = map[key];
            node->value = value;
            removeNode(node);
            addToFront(node);
        } else {
            if(map.size() == capacity) {
                // Cache full then evict least recently used node
                Node* lru = tail->prev;
                removeNode(lru);
                map.erase(lru->key);
                delete lru;
            }

            Node* newNode = new Node(key, value);
            addToFront(newNode);
            map[key] = newNode; 
        }
    }

    void display() {
        Node* current = head->next;
        while(current != tail) {
            cout << "(" << current->key << "," << current->value << ") ";
            current = current->next;
        }

        cout << endl;
    } 

    // Initialize cache with dummy head and tail nodes
    LRUCache(int cap) {
        capacity = cap;

        head = new Node(0,0);
        tail = new Node(0,0);

        head->next = tail;
        tail->prev = head;
    }

    // Free allocated memory
    ~LRUCache() {
        Node* current = head;
        Node* nextNode;
        while(current != nullptr) {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

};

// Simple CLI to test cache operations 
int main () {
    cout << "Enter the capacity :" << endl;
    int cap;
    cin >> cap;
    LRUCache cache(cap);

    while(true) {
        cout << "Enter the operation:" << endl;
        string operation;
        cin >> operation;
        
        if(operation == "get") {
            cout << "Enter the key:" << endl;
            int key;
            cin >> key;

            cout << cache.get(key) << endl;
        } else if(operation == "put") {
            cout << "Enter the key and value" << endl;
            int key, value;
            cin >> key >> value;

            cache.put(key, value);
        } else if(operation == "display") {
            cache.display();
        } else if(operation == "exit") {
            break;
        } else {
            cout << "Invalid operation" << endl;
        }
    }
    return 0;
}
