import axios from "axios";
import Cookies from "js-cookie";
import { loginUrl } from "services/Urlconfig";

const HandleLogin = async (username, password) => {
  try {
    const response = await axios.post(loginUrl, {username, password});
    const newAccessToken = response.headers['authorization'];
    Cookies.set('accessToken', newAccessToken, { httpOnly: true, secure: true, sameSite: 'Lax'});
  
    // refresh는 아직 미구현
    // const newRefreshToken = response.headers['refreshToken'];
    // Cookies.set('refreshToken', newRefreshToken, { httpOnly: true, secure: true, sameSite: 'Strict'});
    return true;
  } catch (e) {
    console.log(e)
    alert("로그인 실패! 아이디 비밀번호가 유효하지 않습니다.");
    // if (e.response)
    //   alert(e.response.data.errors[0].reason);
    return false;
  } 
};

export default HandleLogin;