#ifndef SEARCH_SEARCH_HPP
#define SEARCH_SEARCH_HPP

#include <exception>
#include <string>
#include <vector>
#include <unordered_set>
#include <functional>

#include "maybe.hpp"

namespace search {

class Exception : public std::exception {
public:
    Exception(std::string message) : std::exception(), message(message) {}

    virtual const char* what() const throw() {
        return message.c_str();
    }

private:
    std::string message;
};

class Node {
public:
    virtual std::vector<Node*> explore() = 0;
    virtual Node* parent() = 0;
    virtual bool equals(Node* other) = 0;
    virtual size_t hash() = 0;
};

class Frontier {
public:
    virtual void clear() = 0;
    virtual void push(Node* n) = 0;
    virtual Maybe<Node*> pop() = 0;
};

typedef std::unordered_set<Node*, std::function<size_t(Node*)>, std::function<bool(Node*, Node*)>> ExploredSet;

std::deque<Node*> getPath(Node* node, ExploredSet& explored) {
    std::deque<Node*> vect;

    do {
        vect.push_front(node);
        explored.erase(node);
    } while((node = node->parent()) != nullptr);

    return vect;
}

Maybe<std::deque<Node*>> search(Node* start, Node* goal, Frontier* frontier) {
    if(frontier->pop()) {
        throw Exception("Frontier not initially empty");
    }

    frontier->push(start);
    ExploredSet explored(
        10, 
        [](Node* node){ return node->hash(); },
        [](Node* l, Node* r){ return l->equals(r); });

    while(Maybe<Node*> node = frontier->pop()) {
        explored.insert(node.get());

        if(node.get()->equals(goal)) {
            std::deque<Node*> path = getPath(node.get(), explored);

            for(Node* n : explored) {
                delete n;
            }

            frontier->clear();

            return Just(path);
        }

        for(Node* newNode : node.get()->explore()) {
            if(explored.count(newNode) == 0) {
                frontier->push(newNode);
            } else {
                delete newNode;
            }
        }
    }

    for(Node* n : explored) {
        delete n;
    }

    return None<std::deque<Node*>>();
}

}

#endif
