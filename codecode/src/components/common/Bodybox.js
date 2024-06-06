import React from "react";
import { Box } from "@chakra-ui/react";

const Bodybox = ({children}) => {
  return (
    <Box px={20} 
    maxW={{ base: "100%", sm: "750px", md: "970px", lg: "1200px" }}
    mx="auto">
      {children}
    </Box>
  );
};

export default Bodybox;