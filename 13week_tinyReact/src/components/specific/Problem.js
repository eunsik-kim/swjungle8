import React from 'react';
import Roundbox from '../common/Roundbox';
import MarkdwonRenderer from './MarkdownRenderer';
import { Link, Divider, Text } from '@chakra-ui/react';

const Problembox = ({data, ...rest}) => {
  return (
    <Roundbox {...rest} >
      <Link href={data.link}><Text fontSize='xl'mb={5}>{data.id}. {data.title}</Text></Link>
      <MarkdwonRenderer>{data.content}</MarkdwonRenderer>
      <Divider orientation='horizontal' mt={5} mb={5}/>
      <MarkdwonRenderer>{data.example}</MarkdwonRenderer>
    </Roundbox>
  );
}

export default Problembox;