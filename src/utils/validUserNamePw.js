const CheckUserName = (username) => {
  if (!/^(?=.*[a-z])(?=.*\d)[a-z0-9]+$/.test(username))
    return "알파벳 소문자, 숫자로 구성되어야 합니다.";
  else if (username.length < 4) 
    return "아이디는 최소 4자 이상입니다.";
  else if (username.length > 10)
    return "아이디는 최대 10자 이하입니다.";
  else
    return "";
};

const CheckPassword = (password) => {
  if (!/^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)[a-zA-Z0-9]+$/.test(password))
    return "알파벳 대소문자, 숫자로 구성되어야 합니다.";
  else if (password.length < 8) 
    return "비밀번호는 최소 8자 이상입니다.";
  else if (password.length > 15)
    return "비밀번호는 최대 15자 이하입니다.";
  else
    return "";
};

const CheckDoublePassword = (password, checkpassword) => {
  if (password === checkpassword)
    return "";
  return "비밀번호가 다릅니다.";
}

export {CheckUserName, CheckPassword, CheckDoublePassword}; 