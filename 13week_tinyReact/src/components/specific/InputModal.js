import React, { useState } from 'react';
import useInput from '../../hooks/useInput'
import Button from 'components/common/Button';
import CreateProblem from 'services/CreateProblem';
import useInputBool from '../../hooks/useInputBool'
import {
  Flex, Modal, ModalOverlay, ModalContent, ModalHeader, ModalFooter, Checkbox,
  ModalBody, ModalCloseButton, Input, FormControl, FormLabel, Center, Textarea,
} from '@chakra-ui/react';
import { useSetRecoilState } from 'recoil';
import RefreshState from 'atoms/RefreshState';

function InputModal() {
  const [isOpen, setIsOpen] = useState(false);
  const [title, handleTitleChange] = useInput('');
  const [source, handleSourceChange] = useInput('');
  const [link, handleLinkChange] = useInput('');
  const [content, handleContentChange] = useInput('');
  const [example, handleExampleChange] = useInput('');
  const [is_success, handleIsSuccessChange] = useInputBool(false);
  const [is_review, handleIsReviewChange] = useInputBool(false);
  const SetRefresh = useSetRecoilState(RefreshState);
  
  const onClose = () => setIsOpen(!isOpen);
  return (
    <>
    <Center>
      <Button onClick={() => setIsOpen(true)} pr={3} pl={3}>+</Button>
    </Center>
    <Modal isOpen={isOpen} onClose={onClose}>
      <ModalOverlay />
      <ModalContent w={1500}>
        <ModalHeader>문제 추가</ModalHeader>
        <ModalCloseButton />
        <ModalBody>
          <FormControl>
            <FormLabel>제목</FormLabel>
            <Input mb={1} value={title} onChange={handleTitleChange} />
            <FormLabel>출처</FormLabel>
            <Flex>
              <Input mr={3} mb={1} w={40} value={source} onChange={handleSourceChange} />
              <Checkbox mr={3} onChange={handleIsSuccessChange} 
                isChecked = {is_success}>성공</Checkbox>
              <Checkbox onChange={handleIsReviewChange} 
                isChecked = {is_review}>또 풀기</Checkbox> 
            </Flex>
  
            <FormLabel>Link</FormLabel>
            <Input mb={1}  value={link} onChange={handleLinkChange} />
            <FormLabel>문제 내용</FormLabel>
            <Textarea mb={1} h={40} overflowY='auto' value={content} onChange={handleContentChange} />
            <FormLabel>문제 입출력</FormLabel>
            <Textarea h={60} overflowY='auto' value={example} onChange={handleExampleChange} />
          </FormControl>          
        </ModalBody>

        <ModalFooter>
          <Button color="teal" pr={3} pl={3} 
            onClick={async () => {
              await CreateProblem({title, source, link, content, example, 
                  'is_success':!!is_success, 'is_review': !!is_review});
              SetRefresh(prevRefresh => !prevRefresh);
              setIsOpen(false);
            }}>
            추가
          </Button>
        </ModalFooter>
      </ModalContent>
    </Modal>
    </>
  );
}

export default InputModal;
