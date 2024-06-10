import React from "react";
import {
  Table, Thead, Tbody, Tr, Th, Td, TableContainer,
  Alert, AlertIcon, AlertDescription,
} from '@chakra-ui/react';
import TableRow from './TableRow';
import TableSkeleton from './TableSkeleton';

const MakeTable = ({data, titles, isLoading}) => {
  return (
    <TableContainer>
      {isLoading ? ( 
        <TableSkeleton titles = {titles}/>
      ) : data ? (
      <Table variant='striped' size='md'>
        <Thead>
          <Tr>
            {titles.map((title, key)=>{
              return <Th key={key}>{title}</Th>
            })}
          </Tr>
        </Thead>
        <Tbody>
          <TableRow data={data} />
        </Tbody>
      </Table>
      ) : (
        <Alert status="info">
          <AlertIcon />
          <AlertDescription>
            내용을 추가해 주세요!
          </AlertDescription>
        </Alert>
      )
    }
    </TableContainer>
  );
};

export default MakeTable;