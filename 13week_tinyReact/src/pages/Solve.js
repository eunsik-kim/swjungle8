import { Stack, Checkbox, VStack, CheckboxGroup, Box, NumberInputStepper, HStack,
  NumberInput, NumberInputField, 
} from "@chakra-ui/react";

import Roundbox from "components/common/Roundbox";
import React, { useState } from "react";
import Bodybox from "components/common/Bodybox";
import Button from "components/common/Button";
import GetRandomProblem from 'utils/GetRandomProblem';
import useInputBool from "hooks/useInputBool";
import { useNavigate } from "react-router-dom";

const Solve = () => {
  const [is_success, handleIsSuccessChange] = useInputBool(true);
  const [is_review, handleIsReviewChange] = useInputBool(true);
  const [time, SetTime] = useState(30);
  const handleTime = (v) => SetTime(v);
  const navigate = useNavigate();
  return (
    <Bodybox>
      <Roundbox>
        <VStack>
          <Box fontSize={20} mb={3}>랜덤 문제 풀기</Box>
          <Stack spacing={[1, 5]} direction={['column', 'row']}>
            <CheckboxGroup colorScheme='green' defaultValue={['is_success', 'is_review']}>
            <Checkbox mr={3} onChange={handleIsSuccessChange} value='is_success'
                  isChecked = {is_success}>미해결 문제만</Checkbox>
                <Checkbox onChange={handleIsReviewChange} value='is_review'
                  isChecked = {is_review}>복습 문제만</Checkbox> 
            </CheckboxGroup>
          </Stack>
          <HStack spacing={[1, 5]} >
            <NumberInput min={5} max={80} value={time} onChange={handleTime}
              size='sm' allowMouseWheel sx={{ input: { textAlign: 'right' } }}>
              <NumberInputField />
              <NumberInputStepper>
              </NumberInputStepper>
            </NumberInput>
            <>분</>
            <Button color="teal" pr='3' pl='3' 
              onClick={async ()=> {
                const problem_no = await GetRandomProblem(is_success, is_review);
                if (problem_no)
                  navigate(`/problem/${problem_no}`);
                else {
                  alert("풀 문제가 없습니다. 문제를 추가해 주세요!")
                  navigate('/problem');
                }}}>시작</Button>
          </HStack>
        </VStack>
      </Roundbox>
    </Bodybox>
  )
}
export default Solve;