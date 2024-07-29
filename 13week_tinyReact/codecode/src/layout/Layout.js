import React from "react";
import styled from 'styled-components';
import Header from "./Header";
import Footer from "./Footer";
import { Divider } from '@chakra-ui/react';

const LayoutStyles = styled.div`
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
`;

function Layout({children}) {
    return (
      <div>
        <Header/>
        <Divider/>
        <Divider/> 
        <LayoutStyles>
          {children}
        </LayoutStyles>
        <Divider/>
        <Divider mb={5}/>         
        <Footer/>
      </div>
    );
  }

export default Layout;