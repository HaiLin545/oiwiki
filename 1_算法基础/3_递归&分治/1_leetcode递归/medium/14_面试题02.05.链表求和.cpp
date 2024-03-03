
#include <iostream>
// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
  public:
    void work(ListNode *l1, ListNode *l2, int c) {
        int x = l1->val + l2->val + c;
        c = x / 10;
        x = x % 10;
        l1->val = x;
        if (l1->next == nullptr && l2->next == nullptr) {
            if (c)
                l1->next = new ListNode(c);
            return;
        }
        if (l1->next == nullptr) {
            l1->next = new ListNode(0);
        }
        if (l2->next == nullptr) {
            l2->next = new ListNode(0);
        }
        work(l1->next, l2->next, c);
    }
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        work(l1, l2, 0);
        return l1;
    }
};