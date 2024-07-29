import React from "react";
import { Box } from '@chakra-ui/react';

const Roundbox = ({children, ...rest}) => {
  return (<Box overflowY="auto" borderWidth='1px' borderRadius='lg' padding='4'{...rest} >
          {children}
        </Box> 
  );
}

export default Roundbox;