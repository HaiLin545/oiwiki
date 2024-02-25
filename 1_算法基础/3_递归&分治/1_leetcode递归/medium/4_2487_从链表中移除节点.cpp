
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
    ListNode *cur = nullptr;
    ListNode *removeNodes(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            cur = head;
            return head;
        };
        ListNode *p = removeNodes(head->next);
        if (head->val < p->val) {
            return p;
        } else {
            head->next = p;
            cur = head;
            return head;
        }
    }
};