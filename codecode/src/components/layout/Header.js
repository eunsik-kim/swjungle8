import React from "react";
import Link from '../common/Link';
import LoginHeader from './LoginHeader'
import {Flex, HStack } from '@chakra-ui/react';

const Header = () => {
  return (
    <Flex h={20} alignItems="center" justifyContent="space-between">
      <Link to="/" style={{ textDecoration: 'none', fontSize: 25 }}>Codecode</Link>
      <HStack spacing={10}>
        <Link to="/problem">문제</Link>
        <Link to="/solve">풀기</Link>
        <Link to="/algorithm">알고리즘</Link>
      </HStack>
      <LoginHeader />
    </Flex>
  );
}

export default Header;