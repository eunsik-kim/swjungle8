import { atom } from "recoil";
import { recoilPersist } from "recoil-persist";

const { persistAtom } = recoilPersist();

const LoginState = atom({
  key: 'LoginInfo',
  default: {'isLogin': false, 'username': ''},
  effects_UNSTABLE: [persistAtom],
});

export default LoginState;