import Cookies from "js-cookie";
import axios from "./AxiosConfig";

const HandleLogout = (navigate) => {
  Cookies.remove('accessToken');
  Cookies.remove('refreshToken');
  delete axios.defaults.headers.common['Authorization'];
  navigate('/');
};

export default HandleLogout;