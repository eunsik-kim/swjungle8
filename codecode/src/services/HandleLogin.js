import axios from "axios";
import Cookies from "js-cookie";
import { loginUrl } from "services/Urlconfig";

const HandleLogin = async (username, password, navigate) => {
  try {
    const response = await axios.post(loginUrl, {username, password});
    const newAccessToken = response.headers['authorization'];
    Cookies.set('accessToken', newAccessToken, { httpOnly: true, secure: true, sameSite: 'Lax'});
  
    // refresh는 아직 미구현
    // const newRefreshToken = response.headers['refreshToken'];
    // Cookies.set('refreshToken', newRefreshToken, { httpOnly: true, secure: true, sameSite: 'Strict'});
    navigate('/');
  } catch (e) {
    console.log(e)
    alert(e.message);
    // if (e.response)
    //   alert(e.response.data.errors[0].reason);
  } 
};

export default HandleLogin;