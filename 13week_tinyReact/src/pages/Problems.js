import React, { useEffect, useState } from "react";
import MakeTable from "components/specific/MakeTable";
import Bodybox from "components/common/Bodybox";
import Roundbox from "components/common/Roundbox";
import GetProblem from "services/GetProblem";
import InputModal from "../components/specific/InputModal";
import { useNavigate } from "react-router-dom"
import { useRecoilState, useRecoilValue } from "recoil";
import RefreshState from "atoms/RefreshState";

// first key should be id 
const SELECT_KEYS = ['id', 'title', 'is_success',  'is_review', 'source', 'updatedAt']; 
const PROBLEM_TABLE_TITLE = ['문제 번호', '제목', '해결', '복습', '출처', '날짜', '삭제'];

const Problem = () => {
  const navigate = useNavigate();
  const [data, setData] = useState(null);
  const [isLoading, setIsLoading] = useState(true);
  const refresh = useRecoilValue(RefreshState);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const fetchedData = await GetProblem("");
        if (fetchedData) {
          setData(fetchedData);
        } else {
          navigate('/');
        }
      } finally {
        setIsLoading(false); 
      }
    };
    fetchData();
  }, [refresh]); 

  const selectedData = data && data.data ? data.data.map(post => (
    SELECT_KEYS.map((key) => {
        if (key === 'updatedAt')
          return post[key] ? post[key].split('T')[0] : "없음"
        else if (key === 'is_success' || key === 'is_review') 
          return post[key] ? 'O' : 'X'
        return post[key];
      }
    )
  )) : null;

  return (
    <Bodybox>
      <Roundbox>
        <MakeTable data={selectedData} titles={PROBLEM_TABLE_TITLE} isLoading={isLoading}/>
        <InputModal />
      </Roundbox>
    </Bodybox>
  )
}
export default Problem;