import axios from "./AxiosConfig";
import { postUrl } from "./Urlconfig";

const UpdateProblem = async (problem_no, data) => {
  try {
    const newPostUrl = `${postUrl}/${problem_no}`
    await axios.put(newPostUrl, data);
    alert('저장 완료!')
  } catch (e) {
    const emsg = (e.response?.data?.message || 'update 요청 실패!'); // 에러 메시지 설정
    console.log(e);
    alert(emsg);
  }; 
};

export default UpdateProblem;