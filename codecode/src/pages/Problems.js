import React from "react";
import MakeTable from "components/specific/MakeTable";
import Bodybox from "components/common/Bodybox";

const dummyData = {
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

const Problem = () => {
  const PROBLEM_TABLE_TITLE = ['문제 번호', '제목', '해결', '복습', '출처', '난이도', '날짜'];
  const SELECT_KEYS = ['id', 'title', 'is_success',  'is_review', 'source', 'level', 'updated_at']; // first key should be id 
  
  // get tableData selected by keys
  const selectedData = dummyData.data.posts.map(post => (
    SELECT_KEYS.map((key) => {
        if (key === 'updated_at')
          return post[key].split('T')[0]
        else if (key === 'is_success' || key === 'is_review') {
          if (post[key])
            return 'O'
          return 'X'
        } 
        return post[key];
      }
    )
  ));

  return (
    <Bodybox>
      <MakeTable data={selectedData} titles={PROBLEM_TABLE_TITLE} />
    </Bodybox>
  )
}
export default Problem;