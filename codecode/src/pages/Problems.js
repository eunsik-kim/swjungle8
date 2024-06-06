import React from "react";
import MkTable from "components/common/MkTable";

const dummyData = {
  code: 200,
  message: "Success",
  data: {
    posts: [
      {
        id: 1,
        title: "Two Sum",
        content: "too hard to solve",
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
  const PROBLEM_TABLE_TITLE = ['문제 번호', '제목', '난이도', '날짜'];
  const SELECT_KEYS = ['id', 'title', 'level', 'updated_at']; // first key should be id 
  
  // get tableData selected by keys
  const selectedData = dummyData.data.posts.map(post => (
    SELECT_KEYS.map((key) => {
        if (key === SELECT_KEYS[3])
          return post[key].split('T')[0]
        return post[key];
      }
    )
  ));

  return (
    <MkTable data={selectedData} titles={PROBLEM_TABLE_TITLE} />
  )
}
export default Problem;