import { extendTheme } from '@chakra-ui/react';
import "@fontsource/noto-sans-kr/400.css";

const theme = extendTheme({
  fonts: {
    heading: `'Noto Sans KR', sans-serif`,
    body: `'Noto Sans KR', sans-serif`,
  },
});

export default theme;
