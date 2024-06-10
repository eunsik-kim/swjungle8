// const baseUrl = 'http://localhost:4000';
const baseUrl = 'http://192.249.28.171:8080';
const config = {
  apiUrl: baseUrl,
  loginUrl: baseUrl + '/api/auth/login',
  signupUrl: baseUrl + '/api/auth/signup',
  postUrl: baseUrl + '/api/post', 
  refreshTokenUrl: baseUrl + 'api/refresh',
  AlgoUrl: baseUrl + 'api/algo'
}

export const {apiUrl, loginUrl, signupUrl, postUrl, refreshTokenUrl, AlgoUrl} = config;