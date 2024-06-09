import {HStack} from '@chakra-ui/react';
import Link from 'components/common/Link';
import Cookies from 'js-cookie';
import HandleLogout from 'services/HandleLogout';
import React, { useState, useEffect, } from 'react';
import { useNavigate } from 'react-router-dom';

const LoginHeader = () => {
  const navigate = useNavigate();
  const [isLoggedIn, setIsLoggedIn] = useState(false);

  useEffect(() => {
    const accessToken = Cookies.get('accessToken');
    setIsLoggedIn(!!accessToken);
  }, [Cookies.get('accessToken')]); 

  return (
    <HStack spacing={5}>
      {isLoggedIn ? (
        <Link to="/" onClick={() => HandleLogout(navigate)}>로그아웃</Link>
      ) : (
        <>
          <Link to="/login">로그인</Link>
          <Link to="/signup">회원가입</Link>
        </>
      )}
    </HStack> 
  );
}

export default LoginHeader;