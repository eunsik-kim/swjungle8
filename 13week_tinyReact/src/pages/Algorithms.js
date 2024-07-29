import Bodybox from "components/common/Bodybox";
import Roundbox from "components/common/Roundbox";
import MakeTable from "components/specific/MakeTable";
import React, { useEffect, useState } from "react";
import GetAlgorithms from "services/GetAlgorithms";
import { useNavigate } from "react-router-dom"
import { useRecoilState, useRecoilValue } from "recoil";
import AlRefreshState from "atoms/AlRefreshState";

const dummyDataList = {
  code: "200",
  message: "Success",
  data: [
    {
      id: 1,
      title: "Stable marriage problem",
      author: "eunsik-kim",
      updatedAt: "2024-06-11T12:30:00Z"
    },
  ]
};

// first key should be id 
const SELECT_KEYS = ['id', 'title', 'author', 'updatedAt']; 
const Al_TABLE_TITLE = ['번호', '제목', '작성자', '날짜'];

const Algorithm = () => {
  const navigate = useNavigate();
  const [data, setData] = useState(null);
  const [isLoading, setIsLoading] = useState(true);
  const refresh = useRecoilValue(AlRefreshState);

  // useEffect(() => {
  //   const fetchData = async () => {
  //     try {
  //       const fetchedData = await GetAlgorithms("");
  //       if (fetchedData) {
  //         setData(fetchedData);
  //       } else {
  //         navigate('/');
  //       }
  //     } finally {
  //       setIsLoading(false); 
  //     }
  //   };
  //   fetchData();
  // }, [refresh]); 

  // const selectedData = data && data.data ? data.data.map(post => (
  //   SELECT_KEYS.map((key) => {
  //       if (key === 'updatedAt')
  //         return post[key] ? post[key].split('T')[0] : "없음"
  //       else if (key === 'is_success' || key === 'is_review') 
  //         return post[key] ? 'O' : 'X'
  //       return post[key];
  //     }
  //   )
  // )) : null;

  const selectedData = dummyDataList.data.map(post => (
      SELECT_KEYS.map((key) => {
          if (key === 'updatedAt')
            return post[key] ? post[key].split('T')[0] : "없음"
          else if (key === 'is_success' || key === 'is_review') 
            return post[key] ? 'O' : 'X'
          return post[key];
        }
      )
    ));

  return (
    <Bodybox>
      <Roundbox>
        <MakeTable data={selectedData} titles={Al_TABLE_TITLE} isLoading={false}>
        </MakeTable>
      </Roundbox>
    </Bodybox>
  )
}
export default Algorithm;