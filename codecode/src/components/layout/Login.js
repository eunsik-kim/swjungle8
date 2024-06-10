import Button from "../common/Button";
import SingupText from "../specific/SignupText";
import React, { useState } from "react";
import { Input } from "@chakra-ui/react";
import { useNavigate } from "react-router-dom";
import HandleLogin from "services/HandleLogin";
import Roundbox from "components/common/Roundbox";
import { useSetRecoilState } from "recoil";
import LoginState from "atoms/LoginState";
import { Box, Flex, Center, Divider, InputRightElement, InputGroup } from "@chakra-ui/react";


const Login = () => {
	const navigate = useNavigate(); 
	const [username, setUsername] = useState('');
	const [password, setPassword] = useState('');
	const [show, setShow] = useState(false)
	const handleClick = () => setShow(!show)
	const handleUsernameChange = (event) => setUsername(event.target.value);
	const handlePasswordChange = (event) => setPassword(event.target.value);
	const setIsLogin = useSetRecoilState(LoginState);

	return (
		<Center>
			<Roundbox>
				<Flex direction="column">
					<Center>
						<Box h={10} fontSize={18} fontWeight='semibold'>로그인</Box>
					</Center>
					<Divider/>
					<Divider mb={4}/>
					<Input placeholder='아이디' mb={4} value={username} onChange={handleUsernameChange}/>
					<InputGroup size='md'>
						<Input placeholder='비밀번호' mb={4} type={show ? 'text' : 'password'} 
							value={password} onChange={handlePasswordChange}/>
						<InputRightElement width='4.5rem'>
							<Button onClick={handleClick} fontSize='13'>
								{show ? '숨기기' : '보이기'}
							</Button>
						</InputRightElement>
					</InputGroup>

					<Flex justify="flex-end" mb={4}>
						<Button color='teal' isDisabled = {!username || ! password} 
							onClick={async () =>{
								const isLoginSuccess = await HandleLogin(username, password);
								if (isLoginSuccess) {
									setIsLogin(true);
									navigate('/');
								}}} fontSize='13'>로그인</Button>
					</Flex>
					<Divider/>
					<Divider mb={4}/>
					<SingupText/>
				</Flex>
			</Roundbox>
		</Center>
	);
}

export default Login;