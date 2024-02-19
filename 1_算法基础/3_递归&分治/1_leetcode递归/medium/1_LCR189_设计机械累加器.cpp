class Solution {
public:
    int mechanicalAccumulator(int target) {
        target && (target += mechanicalAccumulator(target-1));
        return target;
    }
};