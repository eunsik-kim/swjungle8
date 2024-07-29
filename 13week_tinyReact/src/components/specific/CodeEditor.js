import Editor from '@monaco-editor/react';
import {
  Menu,
  MenuButton,
  MenuList,
  MenuItem,
  Button,
} from '@chakra-ui/react'
import {
  ChevronDownIcon
} from '@chakra-ui/icons';

const SELECT_LANGS = ["c++", "java", "python", "c", "javascript"];

function CodeEditor({code, language, setLang, handleEditorChange, h}) {
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
        defaultValue={code}
        onChange={handleEditorChange}
        theme="vs"
        height={h}
        overflowX="auto"
      />
    </>
  );
}

export default CodeEditor;