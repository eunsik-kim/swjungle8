import { atom } from "recoil";

const RefreshState = atom({
  key: 'Refresh',
  default: false,
});

export default RefreshState;