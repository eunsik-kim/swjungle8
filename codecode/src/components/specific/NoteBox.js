import React, { useState, useRef } from 'react';
import {
  defaultChecked,
  Editable,
  EditableTextarea,
  EditablePreview,
  Checkbox,
} from '@chakra-ui/react'
import Roundbox from 'components/common/Roundbox';

const NoteBox = ({defaultNote, h, is_review, is_success, ...rest}) => {
  const [Is_Review, setIs_Review] = useState(is_review);
  const [Is_Success, setIs_Success] = useState(is_success);
  const inputRef = useRef(null);

  return (
    <>
      <Roundbox mb="3">
        <Checkbox mr={3} onChange={() => setIs_Success(!Is_Success)} defaultChecked = {Is_Success}>성공</Checkbox>
        <Checkbox onChange={() => setIs_Review(!Is_Review)} defaultChecked = {Is_Review}>나중에 또 풀기</Checkbox>
      </Roundbox>
      <Editable defaultValue={defaultNote} ref = {inputRef} {...rest}>
        <EditablePreview />
        <EditableTextarea h={h}/>
      </Editable>
    </>
  );
};

export default NoteBox;