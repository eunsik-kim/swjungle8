const dummyData = {
  code: "200",
  message: "Success",
  data: {
    posts: [
      {
        id: 1,
        title: "Two Sum",
        content: `Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.
You can return the answer in any order.`,
        example: `Example 1:

Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].

Example 2:

Input: nums = [3,2,4], target = 6
Output: [1,2]

Example 3:

Input: nums = [3,3], target = 6
Output: [0,1]

Example 2:

Input: nums = [3,2,4], target = 6
Output: [1,2]

Example 3:

Input: nums = [3,3], target = 6
Output: [0,1]

        `,
        link: "https://leetcode.com/problems/two-sum/",
        is_success: true,
        is_review: false,        
        note: 
        `A. 아이디어

        두 숫자의 합이 target에 해당하는 값을 얻으려먼 최소 1번 nums배열의 값을 모두 순회하여야 합니다. 
        순회를 하면서 현재의 값을 나중에 사용할 수 있도록 저장을 해둡니다. 여기선 dictionary를 사용하는것이 시간적으로 효율적입니다.

        B. 해결 방법

        1. nums 배열을 순회를 하면서 isin dictionary에 현재 값이 있으면 idx를 반환하고 종료합니다.
        2. target과 차이를 isin dictionary에 추가합니다.
        `,
        language: 'python',
        code:`
        class Solution(object):
          def twoSum(self, nums, target):
              """
              :type nums: List[int]
              :type target: int
              :rtype: List[int]
              """
              isin = {}
      
              for idx, num in enumerate(nums):
                  if isin.get(target-num) is not None:
                      return sorted([isin[target-num], idx])
                  isin[num] = idx 
        `,
        updated_at: "2024-06-07T12:30:00Z"
      },
    ]
  }
};

const dummyDataList = {
  code: "200",
  message: "Success",
  data: {
    posts: [
      {
        id: 1,
        title: "Two Sum",
        is_success: true,
        is_review: false,
        source: "leet",
        link: "https://leetcode.com/problems/two-sum/",
        updated_at: "2024-06-07T12:30:00Z"
      },
      {
        id: 2,
        title: "정ㅋ벅ㅋ",
        is_success: false,
        is_review: false,
        source: "백준",
        link: "https://www.acmicpc.net/problem/1237",
        updated_at: "2024-06-08T12:30:00Z"
      },
    ]
  }
};