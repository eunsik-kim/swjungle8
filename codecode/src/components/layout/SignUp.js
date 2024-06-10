import Button from "../common/Button";
import React, { useEffect, useState } from "react";
import { Input } from "@chakra-ui/react";
import Roundbox from "components/common/Roundbox";
import HandleSignup from "services/HandleSignup";
import InputErrorText from "../common/InputErrorText";
import { useNavigate } from "react-router-dom";
import { Box, Flex, Center, Divider } from "@chakra-ui/react";
import { CheckUserName, CheckPassword, CheckDoublePassword } from '../../utils/validUserNamePw';

const SignUp = () => {
	const navigate = useNavigate(); 
	const [username, setUsername] = useState('');
	const [password, setPassword] = useState('');
	const [checkPassword, setCheckPassword] = useState('');
	const handleUsernameChange = (event) => setUsername(event.target.value);
	const handlePasswordChange = (event) => setPassword(event.target.value);
	const handleCheckPasswordChange = (event) => setCheckPassword(event.target.value);

  const [usernameError, setUsernameError] = useState('');
  const [passwordError, setPasswordError] = useState('');
  const [checkPasswordError, setCheckPasswordError] = useState('');

	useEffect(()=>{setUsernameError(CheckUserName(username))}, [username]);
	useEffect(()=>{setPasswordError(CheckPassword(password))}, [password]);
	useEffect(()=>{setCheckPasswordError(CheckDoublePassword(password, checkPassword))}, [password, checkPassword]);
	
	return (
		<Center>
			<Roundbox>
				<Flex direction="column">
					<Center>
						<Box h={10} fontSize={18} fontWeight='semibold'>회원가입</Box>
					</Center>
					<Divider/>
					<Divider mb={4}/>
					<Input placeholder='아이디' 
						value={username} onChange={handleUsernameChange}/>
					<InputErrorText> {usernameError} </InputErrorText>
					<Input placeholder='비밀번호' type='password' 
						value={password} onChange={handlePasswordChange}/>
					<InputErrorText> {passwordError} </InputErrorText>
					<Input placeholder='비밀번호 확인' type='password' 
						value={checkPassword} onChange={handleCheckPasswordChange}/>
					<InputErrorText> {checkPasswordError} </InputErrorText>
					<Center>
						<Button color='teal' isDisabled = {usernameError || passwordError || checkPasswordError} 
							onClick={async () => {
								const isSignupSuccess = await HandleSignup(username, password);
								if (isSignupSuccess){
									alert('회원가입 성공');
									navigate('/login');
								}
							}} fontSize='13'>회원가입</Button>
					</Center>
				</Flex>
			</Roundbox>
		</Center>
	);
}

export default SignUp;