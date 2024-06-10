import { useNavigate } from 'react-router-dom';
import { useRecoilValue } from 'recoil';
import LoginState from '../../atoms/LoginState'
import styled from 'styled-components';

const LoginOnlyLink = ({ to, children }) => {
  const navigate = useNavigate();
  const isLogin = useRecoilValue(LoginState);

  const StyledButton = styled.button`
  cursor: pointer;
  font-size: inherit;
  &:hover {
    text-decoration: underline;
  }
  `;

  const handleClick = () => {
    if (isLogin) { 
      navigate(to); 
    } else {
      alert('로그인이 필요합니다.'); 
      navigate('/login');
    }
  };

  return (
    <StyledButton onClick={handleClick}> {children}
    </StyledButton>
  );
};


export default LoginOnlyLink;