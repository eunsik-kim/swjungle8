import React from "react";
import Button from "components/common/Button";
import Roundbox from "components/common/Roundbox";
import NoteBox from "components/specific/NoteBox";
import {
  Tabs, TabList, TabPanels, Tab, TabPanel, Spacer, Flex,
} from '@chakra-ui/react'
import CodeEditor from './CodeEditor';

const InputBox = ({data, ...rest}) => {
  return (
    <Roundbox {...rest}>
      <Tabs variant='soft-rounded' colorScheme='green'>
        <Flex>
            <TabList>          
              <Tab>노트</Tab>
              <Tab>코드</Tab>
            </TabList>
            <Spacer/>
            <Button color='teal'px='7px'>
              저장
            </Button>
        </Flex>
        <TabPanels>
          <TabPanel>
            <NoteBox defaultNote={data.note} is_success = {data.is_success} is_reviewd = {data.is_review} {...rest}/>
          </TabPanel>
          <TabPanel>
            <CodeEditor defaultCode={data.code} {...rest}/>
          </TabPanel>
        </TabPanels>
      </Tabs>
    </Roundbox>
  );
}

export default InputBox;