import { Text } from "@chakra-ui/react";

const InputErrorText = ({children}) => {
  return (
    <Text fontSize={12} color="red.500" minHeight="1.5rem">
      {children}
    </Text>
  );
};

export default InputErrorText;