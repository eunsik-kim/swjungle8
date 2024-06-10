import React from 'react';
import Link from 'components/common/Link';
import { Center, Text } from '@chakra-ui/react';

const SignupText = () => {
  return (
    <Center fontSize={12}>
      <Text>
        회원 가입은 <Link to="/signup" color="blue">여기서</Link> 해주세요
      </Text>
    </Center>
  );
};

export default SignupText;
