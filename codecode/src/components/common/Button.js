import React from "react";
import { Box } from "@chakra-ui/react";

const Button = ({children, onClick, isDisabled = false, color = 'gray', ...rest}) => {
  const buttonColor = {
    'navy': ["#263CD3", "#1E30A9", '#FCF6F5'],
    'teal': ['#319795', '#2C7A7B', '#FCF6F5'],
    'gray' : ["#9CA3AF", "#6B7280", '#FFFFFF'],
  }

  const [background, hoverground, fontcolor] = buttonColor[color];
  const buttonStyle = {
    fontWeight: 'semibold',
    borderWidth:'1px',
    borderRadius:'md',
    padding:'5px',
    background: background,
    color: fontcolor,
    _hover: {
      bg: hoverground,
      color: fontcolor,
    },

    ...(isDisabled && {
      background: '#E2E8F0',
      color: '#A0AEC0',
      cursor: 'not-allowed',
      _hover: {
        bg: '#E2E8F0',
        color: '#A0AEC0',
      },
    }),
  };

  if (isDisabled)
    onClick = ()=>{};

  return (
    <button onClick={onClick}>
      <Box {...buttonStyle} {...rest}>
        {children}
      </Box>
    </button>
  );
}

export default Button;