
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
    bool end = false;
    void recursive(ListNode *head) {
        if (head == nullptr)
            return;
        recursive(head->next);
        if (end)
            return;
        if (cur == nullptr || cur == head || cur->next == head) {
            end = true;
            head->next = nullptr;
            return;
        }
        head->next = cur->next;
        cur->next = head;
        cur = head->next;
    }
    void reorderList(ListNode *head) {
        cur = head;
        recursive(head);
    }
};