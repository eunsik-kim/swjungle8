import axios from "./AxiosConfig";
import { postUrl } from "./Urlconfig";

const GetProblems = async ({problem_no}) => {
  try {
    if (problem_no)
      postUrl += '/'+ problem_no
    const response = await axios.get(postUrl);
    return response.data
  } catch (e) {
    const emsg = (e.response?.data?.message || '회원가입 실패!'); // 에러 메시지 설정
    alert(emsg);
  }; 
};

export default GetProblems;