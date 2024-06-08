import React from "react";
import Header from "./Header";
import Footer from "./Footer";
import { Divider, Box } from '@chakra-ui/react';
import Bodybox from "components/common/Bodybox";

function Layout({children}) {
    return (
      <div>
        <Bodybox>
          <Header/>
        </Bodybox>
        <Divider/>
        <Divider mb='20px'/>
          {children}
        <Divider mt='20px'/>
        <Divider mb={5}/>         
        <Footer/>
      </div>
    );
  }

export default Layout;