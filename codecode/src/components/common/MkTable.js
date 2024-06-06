import React from "react";
import { Box } from '@chakra-ui/react';
import Link from "./Link";
import { useLocation  } from "react-router-dom";
import {
  Table, Thead, Tbody, Tr, Th, Td, TableContainer,
} from '@chakra-ui/react';

const MkTable = ({data, titles}) => {
  const location = useLocation();
  return (
    <Box borderWidth='1px' borderRadius='lg' overflow='hidden'>
      <TableContainer>
        <Table variant='striped' size='md'>
          <Thead>
            <Tr>
              {titles.map((title)=><Th>{title}</Th>)}
            </Tr>
          </Thead>
          <Tbody>
            {data.map((row)=>{
              return (
                <Tr>
                  {Object.values(row).map(
                    (value)=>{
                      if (value)
                        return (<Td><Link to={`${location.pathname}/${row[0]}`}>{value}</Link></Td>);  
                      else
                        return (<Td>없음</Td>);
                    }
                  )}
                </Tr>
              );
            })}
          </Tbody>
        </Table>
      </TableContainer>
    </Box>
  );
};

export default MkTable;