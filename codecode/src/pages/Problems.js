import React from "react";
import MakeTable from "components/specific/MakeTable";
import Bodybox from "components/common/Bodybox";

const dummyData = {
  code: 200,
  message: "Success",
  data: {
    posts: [
      {
        id: 1,
        title: "Two Sum",
        content: "too hard to solve",
        solved: "O",
        source: "leet",
        link: "https://leetcode.com/problems/two-sum/",
        category: "Data Structure",
        score: 100,
        author: "eunsik",
        created_at: "2024-06-07T12:00:00Z",
        updated_at: "2024-06-07T12:30:00Z"
      },
      {
        id: 2,
        title: "정ㅋ벅ㅋ",
        content: "이상하게 어려운 문제이다.",
        solved: "X",
        source: "백준",
        link: "https://www.acmicpc.net/problem/1237",
        category: "exception",
        score: 100,
        author: "eunsik",
        created_at: "2024-06-08T12:00:00Z",
        updated_at: "2024-06-08T12:30:00Z"
      },
    ]
  }
};

const Problem = () => {
  const PROBLEM_TABLE_TITLE = ['문제 번호', '제목', '해결', '출처', '난이도', '날짜'];
  const SELECT_KEYS = ['id', 'title', 'solved',  'source', 'level', 'updated_at']; // first key should be id 
  
  // get tableData selected by keys
  const selectedData = dummyData.data.posts.map(post => (
    SELECT_KEYS.map((key) => {
        if (key === 'updated_at')
          return post[key].split('T')[0]
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