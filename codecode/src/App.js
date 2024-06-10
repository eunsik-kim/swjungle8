import './App.css';
import Router from './components/layout/Router';
import theme from './styles/theme';
import { ChakraProvider } from '@chakra-ui/react';
import { RecoilRoot } from 'recoil';

function App() {
  return (
    <ChakraProvider theme={theme}>
      <RecoilRoot>
        <Router/>
      </RecoilRoot>
    </ChakraProvider>
  );
}

export default App;
