
//   Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
  public:
    ListNode *cur;
    bool check(ListNode *head) {
        if (head == nullptr)
            return true;
        bool res = check(head->next);
        if (head->val == cur->val) {
            cur = cur->next;
            return true && res;
        }
        return false;
    }
    bool isPalindrome(ListNode *head) {
        cur = head;
        return check(head);
    }
};
