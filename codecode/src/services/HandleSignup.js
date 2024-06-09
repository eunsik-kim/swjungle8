import axios from "axios";
import { signupUrl } from "services/Urlconfig";

const HandleSignup = async (username, password, navigate) => {
  try {
    const response = await axios.post(signupUrl, {username, password});
    console.log(response);
    // if (response.data.status === '200')
    alert("회원가입 완료!");
    navigate('/login');
    // else
    //   alert("회원가입 실패!", response.data.error);
    
  } catch (e) {
    console.log(e)
    alert("회원가입 실패!", e);
  } 
};

export default HandleSignup;