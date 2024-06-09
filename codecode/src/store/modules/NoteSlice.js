import { createSlice } from '@reduxjs/toolkit';

const dataExample = `A. 아이디어
...
B. 해결방법
...`;

export const NoteSlice = createSlice({
    name: "Note",
    initialState: {data: dataExample},
    reduces: {
        
    }
});

export default NoteSlice;