import { useNavigate } from 'react-router-dom';
import { useRecoilValue } from 'recoil';
import LoginState from '../../atoms/LoginState'
import StyledButton from '../../styles/LinkButton'

const LoginOnlyLink = ({ to, children }) => {
  const navigate = useNavigate();
  const Logininfo = useRecoilValue(LoginState);

  const handleClick = () => {
    if (Logininfo.isLogin) { 
      navigate(to); 
    } else {
      alert('로그인이 필요합니다.'); 
      navigate('/login');
    }
  };

  return (
    <StyledButton onClick={() => {handleClick()}}> {children}
    </StyledButton>
  );
};

export default LoginOnlyLink;