import React from 'react';
import Roundbox from '../common/Roundbox';
import { Link, Box, Divider, Text } from '@chakra-ui/react';

const Problembox = ({data, ...rest}) => {
  return (
    <Roundbox {...rest} >
      <Link to={data.link}><Text fontSize='xl'mb={5}>{data.id}. {data.title}</Text></Link>
      <Box mb={5}>{data.content}</Box>
      <Divider orientation='horizontal' mb={5}/>
      <Box>{data.example}</Box>
    </Roundbox>
  );
}

export default Problembox;