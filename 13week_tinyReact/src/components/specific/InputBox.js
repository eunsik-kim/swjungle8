import React, { useState } from "react";
import Button from "components/common/Button";
import Roundbox from "components/common/Roundbox";
import NoteBox from "components/specific/NoteBox";
import UpdateProblem from "services/UpdateProblem";
import useInputBool from "hooks/useInputBool";
import {
  Tabs, TabList, TabPanels, Tab, TabPanel, Spacer, Flex,
} from '@chakra-ui/react'
import CodeEditor from './CodeEditor';
import useInput from "hooks/useInput";

const InputBox = ({data, ...rest}) => {
  const [note, handleNote] = useInput(data.note);
  const [IsReview, handleIsReview] = useInputBool(data.is_review);
  const [IsSuccess, handleIsSuccess] = useInputBool(data.is_success);

  const [language, setLang] = useState(data.language);
  const [code, setCode] = useState(data.code);
  const handleEditorChange = (value) => setCode(value);

  return (
    <Roundbox {...rest}>
      <Tabs variant='soft-rounded' colorScheme='green'>
        <Flex>
            <TabList>          
              <Tab>코드</Tab>
              <Tab>노트</Tab>
            </TabList>
            <Spacer/>
            <Button color='teal'px='7px' onClick={() => {UpdateProblem(
              data.id, {...data, note, 'is_review': IsReview,
                'is_success': IsSuccess, code, language, })}}>
              저장
            </Button>
        </Flex>
        <TabPanels>
          <TabPanel>
            <CodeEditor code={code} language = {language} setLang = {setLang} 
              handleEditorChange = {handleEditorChange} {...rest}/>
          </TabPanel>
          <TabPanel>
            <NoteBox note={note}  IsReview = {IsReview} IsSuccess = {IsSuccess} 
              handleNote={handleNote} handleIsReview = {handleIsReview} 
              handleIsSuccess = {handleIsSuccess} {...rest}/>
          </TabPanel>
        </TabPanels>
      </Tabs>
    </Roundbox>
  );
}

export default InputBox;