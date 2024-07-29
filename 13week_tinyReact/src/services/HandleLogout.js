import Cookies from "js-cookie";
import axios from "./AxiosConfig";

const HandleLogout = () => {
  Cookies.remove('accessToken');
  Cookies.remove('refreshToken');
  delete axios.defaults.headers.common['Authorization'];
};

export default HandleLogout;