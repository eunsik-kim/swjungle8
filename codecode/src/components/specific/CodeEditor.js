import Editor from '@monaco-editor/react';
import { useState } from 'react';
import {
  Box,
  Menu,
  MenuButton,
  MenuList,
  MenuItem,
  Button,
} from '@chakra-ui/react'
import {
  ChevronDownIcon
} from '@chakra-ui/icons';

function CodeEditor({defaultCode, h}) {
  const [code, setCode] = useState('');
  const [language, setLang] = useState('python');

  const SELECT_LANGS = ["c++", "java", "python", "c", "javascript"];

  function handleEditorChange(value, event) {
    setCode(value);
  }

  return (
    <>
      <Menu>
        <MenuButton size='sm' as={Button} rightIcon={<ChevronDownIcon />}>
          {language}
        </MenuButton>
        <MenuList>
          {SELECT_LANGS.map((select)=> 
            <MenuItem key={select} onClick={()=>setLang(select)}>
              {select}
            </MenuItem>)}
        </MenuList>
      </Menu>
      <Editor 
        defaultLanguage={language}
        defaultValue={defaultCode}
        value={code}
        onChange={handleEditorChange}
        theme="vs"
        height={h}
        overflowX="auto"
      />
    </>
  );
}

export default CodeEditor;