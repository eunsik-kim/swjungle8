import React from "react";
import {
  Table, Thead, Tbody, Tr, Th, Td, Skeleton
} from '@chakra-ui/react';

const TableSkeleton = ({titles}) => {
  const skeletonRows = Array.from({ length: 5 }, (_, i) => i); 
  return (
    <Table variant='striped' size='md'>
      <Thead>
        <Tr>
          {titles.map((title, key) => (
            <Th key={key}>{title}</Th>
          ))}
        </Tr>
      </Thead>
      <Tbody>
        {skeletonRows.map((row) => (
          <Tr key={row}>
            {titles.map((_, colIndex) => (
              <Td key={colIndex}>
                <Skeleton height='20px' />
              </Td>
            ))}
          </Tr>
        ))}
      </Tbody>
    </Table>
  );
}

export default TableSkeleton;