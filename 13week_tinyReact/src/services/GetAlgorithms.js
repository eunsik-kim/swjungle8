import axios from "./AxiosConfig";
import { AlgoUrl } from "./Urlconfig";

const GetAlgorithms = async () => {
  try {
    const response = await axios.get(AlgoUrl);
    return response.data
  } catch (e) {
    const emsg = (e.response?.data?.message || '잘못된 접근!'); // 에러 메시지 설정
    console.log(e);
    alert(emsg);
    return false;
  }; 
};

export default GetAlgorithms;