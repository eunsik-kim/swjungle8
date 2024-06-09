import axios from "axios";
import {apiUrl, refreshTokenUrl} from "./Urlconfig";
import Cookies from "js-cookie";

const instance = axios.create({
  baseURL: apiUrl
});

instance.interceptors.request.use(
  axiosConfig => {
    const token = Cookies.get('accessToken');
    if (token) {
      axios.defaults.headers.common['Authorization'] = `Bearer ${token}`;
    }
    return axiosConfig;
  },
  error => Promise.reject(error)
);

instance.interceptors.response.use(
  response => {
    // header에 access token 포함
    const newAccessToken = response.headers['accessToken'];
    if (newAccessToken) 
      Cookies.set('accessToken', newAccessToken, { httpOnly: true, secure: true, sameSite: 'Lax'});

    return response;
  },

  async error => {
    const originalRequest = error.config;
    if (error.response.status === '401' && !originalRequest._retry) {
      originalRequest._retry = true;   // stop infinite loop
      try {
        const refreshToken = Cookies.get('refreshToken');
        const response = await axios.post(refreshTokenUrl, { token : refreshToken });

        // header에 access token 포함
        const newToken = response.headers['accessToken'];
        if (newToken) {
          Cookies.set('accessToken', newToken, { httpOnly: true, secure: true, sameSite: 'Lax'});
          axios.defaults.headers.common['Authorization'] = `Bearer ${newToken}`;
        }
        return instance(originalRequest);
      } catch (e) {
        return Promise.reject(e);
      }
    }
    return Promise.reject(error);
  }
);

export default instance;