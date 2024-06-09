import { useState, useCallback } from "react";
import { CheckUserName, CheckPassword, CheckDoublePassword } from '../../utils/validUserNamePw';

// useFormValidation 커스텀 훅
function FormValidation(initialValues, validators) {
  const [values, setValues] = useState(initialValues);
  const [errors, setErrors] = useState({});
  const validate = useCallback(
    (newValues) => {
      const newErrors = {};
      for (const field in validators) {
        newErrors[field] = validators[field](newValues[field]);
      }
      setErrors(newErrors);
    },
    [validators]
  );

  const handleChange = (event) => {
    const { name, value } = event.target;
    setValues({ ...values, [name]: value });
    validate(values); 
  };

  return { values, errors, handleChange };
}

export default FormValidation;