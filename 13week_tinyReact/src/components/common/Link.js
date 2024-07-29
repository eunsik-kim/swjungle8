import { Link as ReactRouterLink } from 'react-router-dom';
import { Link as ChakraLink} from '@chakra-ui/react';

const Link = ({to, children, ...rest}) => {
  return (
    <ChakraLink as={ReactRouterLink} to={to} {...rest}>
      {children}
    </ChakraLink>
  );
};

export default Link;
