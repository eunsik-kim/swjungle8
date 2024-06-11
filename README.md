### Codecode

알고리즘 문제 연습, 복습, 분석을 하는 사이트 입니다.
백준과 leetcode를 참고하여 만들었습니다. [chakra](https://v2.chakra-ui.com/)를 사용해서 대부분의 인터페이스를 구성하였습니다.

### 시연영상
![video](https://github.com/eunsik-kim/Codecode/assets/153556378/8efbf195-babe-497d-8b07-eb06f41853e6)

### 요구 사항
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/f1879f84-6c73-45cc-a1c2-9f911e175f47)

#### UI/UX
문제를 풀고 기록할 수 있는 문제page, 문제를 random으로 풀 수 있는 풀이page, 공유게시판인 알고리즘page(미구현)과 로그인을 구현하였습니다.

#### 게시물 조회/추가/삭제/수정 

![image](https://github.com/eunsik-kim/Codecode/assets/153556378/5ee17070-99ea-430e-8e17-ce0931b121a4)
문제 페이지의 조회, 추가, 삭제

![image](https://github.com/eunsik-kim/Codecode/assets/153556378/ff24211a-74fc-4505-9f5c-76f246ab2bf4)
문제 상세 페이지의 조회, editable 속성을 사용하여 메모장과 같이 수정

#### 상태관리 라이브러리
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/b36d9c36-66fd-4000-9827-2986b4c4490c)

사용이 용이한 recoil을 사용해서 login 과 refresh state를 관리하였습니다.
1. login state는 header의 로그인/로그아웃 ui, Link 접속시 로그인 접근 권한을 위해 사용했습니다.
2. 새로 고침에도 로그인 상태를 지속하기 위해 [persistAtom]를 사용하여 로컬 스토리지에 저장하였습니다.
3. refresh state는 문제를 추가, 삭제한 뒤에 새로고침이 되지않던 문제를 해결하기 위해 사용했습니다.

#### 동적 라우팅
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/7558994b-5dfa-4f05-a9bb-46a7e7a100a8)
header를 제외한 본문에 해당하는 내용들을 Link로 routing하였습니다.

#### 유효성 검증
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/80166c0b-1618-4279-9cdd-462f9a18bb13)
Spring과제에서 지시된 대로 유효성 검증하였습니다.
1. 아이디, 비밀번호 길이제한. 2. 아이디, 비밀번호 사용문자 종류 검증. 3. 비밀번호 확인 검증
useState를 사용하여 위의 경우를 검증하였고 문제시 회원가입 버튼을 비활성화 하였습니다.
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/a9e13741-8692-4c66-b38e-2444f095606a)

#### 로그인/회원가입 기능
회원가입은 위에서 보는대로 만들었습니다.
로그인 세션 유지 방법에 대해 고민을 하였습니다.
1. JWT or 세션방식 중 어떤 방법으로 로그인 유지할지.
2. JWT인 경우 cookie나 local 혹은 session storage 중 어디에 저장할지.

구현에 용이한 JWT방식으로 로그인을 유지하기로 했습니다. server에서 cookie에 저장을 하지 않고 보냈기 때문에
cookies에 저장할 때 httponly를 false로 설정하였고 xss 공격에 유의하도록 입력값을 검증하여 사용하도록 했습니다.

#### Custom Hook
아직 react의 Hook에 대해선 많이 익숙치 않아서 좋은 응용을 하진 못하였습니다. 
state와 onchange를 같이 사용하는 경우가 많은데 쉽게 적용할 수 있는 hook을 만들어 사용했습니다.
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/d412908d-8e72-436f-b9dd-bafada604634)
이외에 onchange에서 rerendering을 줄일 수 있는 debounce를 적용하고 싶었지만 잘 되지 못하여 아쉽습니다.

#### Button
![image](https://github.com/eunsik-kim/Codecode/assets/153556378/0c4bfe68-83d9-428f-9f93-b055eecd5685)
만능 버튼은 아니지만 지정된 색조합, hover와 disable가능하도록 설정했습니다.

### 개선해야 될점
전반적으로 시간에 쫓겨 효과적인 코드를 작성하지 못했습니다.
1. Algorithm 게시판과 Timer를 구현하지 못하였습니다.
2. memoization을 고려하지 못했습니다.
3. component를 최소한의 part로 split 해야 하지만 구현의 용이성을 위해 (상태 관리가 힘들기 때문에) 많이 나누지 못하였습니다.
4. axios를 사용하여 server와 통신하는 함수를 하나로 구현할 수 있었지만 수정하지 못했습니다.
5. directory 구조와 naming이 적절하지 못하기 때문에 수정에 많은 시간을 소모하였습니다.
6. chakra library에 대한 의존성이 큽니다.


