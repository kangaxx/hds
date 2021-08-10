#ifndef FIXED2WAYLIST_H
#define FIXED2WAYLIST_H
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <string>
#include <assert.h>
#include "common.h"

namespace commonfunction_c {
    template<class T>
    class Fixed2WayList {
    private:
        class Node {
        public:
            T data;
            Node* prior;
            Node* next;
            Node(const T& element, Node* prior, Node* next) :data(element) { this->next = next; this->prior = prior; prior->next = next->prior = this; }
            Node() :data(data) {}
            ~Node() { prior->next = next; next->prior = prior; }
        };
        Node* head;
        Node* current;
        unsigned int maxSize = INT_FIXED2WAYLIST_DEFAULT_SIZE;
        inline bool isOutOfSize() {
            if (size() > this->maxSize)
                return true;
            return false;
        }

        void popFirst() { 
            assert(this->head->next != this->head->prior);
            this->delElement(0);
        }
        void popLast() {
            assert(this->head->next != this->head->prior);
            this->delElement(size() - 1);
        }
    public:
        Fixed2WayList(unsigned int max = INT_FIXED2WAYLIST_DEFAULT_SIZE) :head(new Node()) {
            head->prior = head->next = head; 
            current = NULL; 
            if (max == 0) max = INT_FIXED2WAYLIST_DEFAULT_SIZE;
            this->maxSize = max; 
        }

        Fixed2WayList(const Fixed2WayList<T>& right) :head(new Node()){
            this->head->prior = this->head->next = this->head;
            Node* p = right.head->next;
            this->maxSize = right.maxSize;
            for (unsigned int i = 0; i < right.size(); ++i) {
                this->insertElement(right.getElement(i), i);
            }
            setCurrentIdx(right.getCurrentIdx());
        }


        int getCurrentIdx() const{
            if (current == head)
                return INT_FIXED2WAYLIST_CURRENT_IDX_ERROR;
            int result = 0;
            Node* p = head->next;
            while (p != NULL && p != head) {
                if (p == current)
                    return result;
                p = p->next;
                result++;
            }
            return INT_FIXED2WAYLIST_CURRENT_IDX_ERROR; 
        }

        void setCurrentIdx(int idx) {
            if (idx == INT_FIXED2WAYLIST_CURRENT_IDX_ERROR || idx < 0)
                current = NULL;
            current = head->next;
            while (idx > 0 && current->next != head) {
                --idx;
                current = current->next;
            }
        }

        unsigned int size() const {
            unsigned int count = 0;
            for (Node* p = head->next; p != head; p = p->next)
                count++;
            return count;
        }

        T& operator [](unsigned int idx) { return getElement(idx); }
        bool isEmpty()const { return head == head->next ? true : false; }

        void addToLast(const T& element) { 
            Node* ne = new Node(element, head->prior, head); 
            if (this->isOutOfSize())
                this->popFirst();
        }
        inline void insertToLast(const T& element) { return this->addToLast(element); }

        T getLastElement()const { assert(!isEmpty()); return head->prior->data; }

        void delLastElement() { assert(!isEmpty()); Node* p = head->prior; delete p; }

        void alterLastElement(const T& newElement) { assert(!isEmpty()); head->prior->data = newElement; }

        inline void insert(T element, unsigned int pos) { this->insertElement(element, pos); }
        void insertElement(T element, unsigned int pos) {
            assert(pos >= 0);
            assert(pos <= size());
            if (pos == size()) {
                this->addToLast(element);
                return;
            }
            Node* p = head->next;
            while (pos-- != 0)
                p = p->next;
            new Node(element, p->prior, p);
            if (this->isOutOfSize())
                this->popLast();
        }


        T& get(unsigned int idx) { return this->getElement(idx); }
        T& getElement(unsigned int idx) const{
            assert(idx < size());
            Node* p = head->next;
            while (idx-- != 0)
                p = p->next;
            return p->data;
        }

        T del(unsigned int idx) { return this->delElement(idx); }
        T delElement(unsigned int idx) {
            assert(idx < size());
            assert(size() > 0);
            Node* t = head->next;
            while (idx-- != 0)
                t = t->next;
            T ret = t->data;
            t->next->prior = t->prior;
            t->prior->next = t->next;
            delete t;
            return ret;
        }


        void alter(const T& newElement, unsigned int idx) { this->alterElement(newElement, idx); }
        void alterElement(const T& newElement, unsigned int idx) {
            assert(idx < size());
            Node* t = head->next;
            while (idx-- != 0)
                t = t->next;
            t->data = newElement;
            return;
        }

        int findElement(const T& element) const {
            char* ori, * in;
            int result = 0;
            Node* t = head->next;
            while (t != head) {
                ori = reinterpret_cast<char*>(&(t->data));
                in = reinterpret_cast<char*>(&element);
                if (strncmp(ori, in, sizeof(T)) == 0)
                    return result;
                result++;
                t = t->next;
            }
            return -1;
        }
        void Traverse(void (*visit)(T& element)) {
            Node* t = head->next;
            while (t != head) {
                visit(t->data);
                t = t->next;
            }
        }

        void TraverseBack(void (*visit)(T& element)) {
            Node* t = head->prior;
            while (t != head) {
                visit(t->data);
                t = t->prior;
            }
        }

        void clear() { this->clearAllElement(); }
        void clearAllElement() {
            Node* t, * p;
            p = head->next;
            while (p != head) {
                t = p->next;
                delete p;
                p = t;
            }
        }

        T& getCurrent() {
            assert(size() > 0);
            assert(head != head->next);
            if (current == NULL || current == head)
                current = head->next;
            return current->data;
        }
        T& goNext() {
            assert(size() > 0);
            assert(head != head->next);
            this->getCurrent();
            Node* p = current->next;
            if (p == head)
                p = p->next;
            current = p;
            return current->data;
        }

        T& goPrior() {
            assert(size() > 0);
            assert(head != head->prior);
            this->getCurrent();
            Node* p = current->prior;
            if (p == head)
                p = p->prior;
            current = p;
            return current->data;
        }

        ~Fixed2WayList() {
            this->clearAllElement();
            if (head != NULL) {
                delete head;
                head = NULL;
            }
        }
    };
}

#endif //FIXED2WAYLIST_H