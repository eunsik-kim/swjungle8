import './App.css';
import Router from './layout/Router';
import theme from './styles/theme';
import { ChakraProvider, Box } from '@chakra-ui/react';

function App() {
  return (
    <ChakraProvider theme={theme}>
        <Router/>
    </ChakraProvider>
  );
}

export default App;
