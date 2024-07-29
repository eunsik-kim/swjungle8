import { useState } from 'react';

const useInputBool = (initValue)=> {
  const [value, setValue] = useState(initValue);
  const handleChange = () => setValue(!value);
  return [value, handleChange];
}

export default useInputBool;