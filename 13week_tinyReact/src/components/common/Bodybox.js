import React from "react";
import { Box } from "@chakra-ui/react";

const Bodybox = ({children, ...rest}) => {
  return (
    <Box px={20} 
    maxW={{ base: "100%", sm: "750px", md: "970px", lg: "1200px" }}
    mx="auto" {...rest}>
      {children}
    </Box>
  );
};

export default Bodybox;