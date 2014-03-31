#include <typeinfo>
#include <deque>
#include <sstream>
#include <string>
#include <memory>

#include "search.hpp"
#include "maybe.hpp"

class TestNode : public search::Node {
public:
    TestNode(int x, int y);
    TestNode(TestNode* parent, int xd, int yd);

    std::vector<search::Node*> explore() {
        return { new TestNode(this, 1, 0), new TestNode(this, -1, 0),
                 new TestNode(this, 0, 1), new TestNode(this, 0, -1) };
    };

    search::Node* parent() {
        return p;
    };

    bool equals(search::Node* o) {
        TestNode* other = dynamic_cast<TestNode*>(o);
        return (other != nullptr) && (x == other->x) && (y == other->y);
    };

    std::string toString() {
        std::stringstream str;
        str << "(" << x << ", " << y << ")";
        return str.str();
    }

    size_t hash() {
        return x + (10 * y);
    };

private:
    search::Node* p;
    int x, y;
};

TestNode::TestNode(int x, int y)
    : p(nullptr), x(x), y(y) {}

TestNode::TestNode(TestNode* parent, int xd, int yd)
    : p(parent), x(parent->x + xd), y(parent->y + yd) {}

class BFFrontier : public search::Frontier {
public:
    BFFrontier() : nodes() {}

    void clear() {
        for(search::Node* node : nodes) {
            delete node;
        }

        nodes.clear();
    }

    void push(search::Node* n) {
        nodes.push_back(n);
    }

    Maybe<search::Node*> pop() {
        if(nodes.empty()) {
            return None<search::Node*>();
        } else {
            search::Node* n = nodes.front();
            nodes.pop_front();
            return Just(n);
        }
    }

private:
    std::deque<search::Node*> nodes;
};

void printPath(std::deque<search::Node*> path) {
    for(search::Node* node : path) {
        std::cout << dynamic_cast<TestNode*>(node)->toString() << std::endl;
        delete node;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    auto s1 = new TestNode(0, 0);
    auto s2 = new TestNode(2, 5);
    auto g1 = new TestNode(0, 0);
    auto g2 = new TestNode(7, 6);
    auto f = new BFFrontier();

    auto path = search::search(s1, g1, f);
    path.callElse(
        printPath,
        []() { std::cout << "No Path Found" << std::endl; });

    f->clear();
    path = search::search(s2, g2, f);
    path.callElse(
        printPath,
        []() { std::cout << "No Path Found" << std::endl; });

    delete s1;
    delete s2;
    delete g1;
    delete g2;
    delete f;
}
