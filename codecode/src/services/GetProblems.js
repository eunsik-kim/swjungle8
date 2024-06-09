import axios from "./AxiosConfig";

const GetProblems = async () => {
  try {
    const response = await axios.get(postUrl);
    return 
  } catch (e) {
    alert(e.message);
  }; 
};

export default GetProblems;