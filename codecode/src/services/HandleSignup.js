import axios from "axios";
import { signupUrl } from "services/Urlconfig";

const HandleSignup = async (username, password) => {
  try {
    const response = await axios.post(signupUrl, {username, password});
    console.log(response);
    console.log(response.response.status);
    return true; 
  } catch (e) {
    console.log(e)
    alert(`회원가입 실패! ${e.response.data.message}`);
    return false;
  } 
  
};

export default HandleSignup;