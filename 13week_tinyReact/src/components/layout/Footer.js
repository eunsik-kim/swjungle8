import React from "react";
import styled from 'styled-components';

const FooterStyles = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 12px;
`;

const Footer = () => {
    return <FooterStyles>
      <div>No copyright, in Jungle</div>
      </FooterStyles>
}

export default Footer;