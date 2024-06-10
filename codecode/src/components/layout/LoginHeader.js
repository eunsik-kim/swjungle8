import {HStack} from '@chakra-ui/react';
import Link from 'components/common/Link';
import HandleLogout from 'services/HandleLogout';
import React from 'react';
import { useRecoilState } from 'recoil';
import LoginState from 'atoms/LoginState';

const LoginHeader = () => {
  const [isLogin, setIsLogin] = useRecoilState(LoginState);

  return (
    <HStack spacing={5}>
      {isLogin ? (
        <Link to="/" onClick={() => {
          HandleLogout();
          setIsLogin(false);
        }}>로그아웃</Link>
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