import React from "react";
import styled from 'styled-components';
import Header from "./Header";
import Footer from "./Footer";
import { Divider } from '@chakra-ui/react';
import Bodybox from "components/common/Bodybox";

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
        <Bodybox>
          <Header/>
        </Bodybox>
        <Divider/>
        <Divider/>
        <Bodybox>
          <LayoutStyles>
            {children}
          </LayoutStyles>
        </Bodybox>
        <Divider/>
        <Divider mb={5}/>         
        <Footer/>
      </div>
    );
  }

export default Layout;