import './App.css';
import Router from './components/layout/Router';
import theme from './styles/theme';
import { ChakraProvider } from '@chakra-ui/react';

function App() {
  return (
    <ChakraProvider theme={theme}>
        <Router/>
    </ChakraProvider>
  );
}

export default App;
