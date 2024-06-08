import React from "react";
import Roundbox from "../common/Roundbox";
import Link from "../common/Link";
import { useLocation  } from "react-router-dom";
import {
  Table, Thead, Tbody, Tr, Th, Td, TableContainer
} from '@chakra-ui/react';

const MakeTable = ({data, titles}) => {
  const location = useLocation();
  return (
    <Roundbox>
      <TableContainer>
        <Table variant='striped' size='md'>
          <Thead>
            <Tr>
              {titles.map((title, key)=>{
                if (title === titles[1])
                  return <Th key={key}>{title}</Th>
                return <Th key={key}>{title}</Th>
              })}
            </Tr>
          </Thead>
          <Tbody>
            {data.map((row, rowIndex)=>{
              return (
                <Tr key={rowIndex}>
                  {Object.values(row).map(
                    (value, index)=>{
                      if (index == 1)
                        return (<Td key={index} width='1000px'><Link to={`${location.pathname}/${row[0]}`}>{value}</Link></Td>);  
                      else if (value)
                        return (<Td key={index}><Link to={`${location.pathname}/${row[0]}`}>{value}</Link></Td>);  
                      return (<Td key={index}>없음</Td>);
                    }
                  )}
                </Tr>
              );
            })}
          </Tbody>
        </Table>
      </TableContainer>
    </Roundbox>
  );
};

export default MakeTable;