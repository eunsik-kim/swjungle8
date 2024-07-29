import React from 'react';
import {
  Editable,
  EditableTextarea,
  EditablePreview,
  Checkbox,
} from '@chakra-ui/react'
import Roundbox from 'components/common/Roundbox';

const dataExample = `A. 아이디어
...

B. 해결방법
...`;

const NoteBox = ({note, IsReview, IsSuccess, handleNote, handleIsReview,
                  handleIsSuccess, h, ...rest}) => {
  return (
    <>
      <Roundbox mb="3">
        <Checkbox mr={3} onChange={() => handleIsSuccess(!IsSuccess)} 
          isChecked = {IsSuccess}>성공</Checkbox>
        <Checkbox onChange={() => handleIsReview(!IsReview)} 
          isChecked = {IsReview}>나중에 또 풀기</Checkbox>
      </Roundbox>
      <Editable defaultValue={note} placeholder={dataExample} {...rest}>
        <EditablePreview />
        <EditableTextarea h={h} onChange = {handleNote}/>
      </Editable>
    </>
  );
};

export default NoteBox;