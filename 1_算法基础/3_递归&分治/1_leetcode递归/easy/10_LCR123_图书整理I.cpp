
#include <vector>
//   Definition for singly-linked list.
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
  public:
    vector<int> arr;
    void solve(ListNode *head) {
        if (head == nullptr)
            return;
        solve(head->next);
        arr.push_back(head->val);
    }
    vector<int> reverseBookList(ListNode *head) {
        solve(head);
        return arr;
    }
};