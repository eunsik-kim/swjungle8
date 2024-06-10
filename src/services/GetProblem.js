import axios from "./AxiosConfig";
import { postUrl } from "./Urlconfig";

const GetProblem = async (problem_no) => {
  try {
    let newPostUrl = postUrl;
    if (problem_no)
      newPostUrl = `${newPostUrl}/${problem_no}`
    const response = await axios.get(newPostUrl);
    return response.data
  } catch (e) {
    const emsg = (e.response?.data?.message || '잘못된 접근!'); // 에러 메시지 설정
    console.log(e);
    alert(emsg);
    return false;
  }; 
};

export default GetProblem;