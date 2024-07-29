import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import { useParams } from 'react-router-dom';
import Problembox from '../components/specific/Problem';
import InputBox from '../components/specific/InputBox';
import { HStack, Box, Skeleton } from '@chakra-ui/react';
import GetProblem from "services/GetProblem";

const ProblemSolve = () => {
  const {id} = useParams();
  const navigate = useNavigate();
  const [data, setData] = useState(null);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const fetchedData = await GetProblem(id);
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
  }, []); 

  return (
    <HStack mx={10}>
      {isLoading || !data || !data.data ? (
        <>
          <Box flexBasis="40%" h='80vh'>
            <Skeleton height="100%" />
          </Box>
          <Box flexBasis="60%" h='80vh'>
            <Skeleton height="100%" />
          </Box>
        </>
      ) : ( 
        <>
          <Problembox flexBasis="40%" h='80vh' whiteSpace="pre-line" data={data.data} />
          <InputBox flexBasis="60%" h='80vh' whiteSpace="pre-line" data={data.data} />
        </>
      )}
    </HStack>
  );
}
export default ProblemSolve;
