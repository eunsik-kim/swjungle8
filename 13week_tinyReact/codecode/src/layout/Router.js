import React from "react";
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import Layout from './Layout'
import Home from "../pages/Home";
import Problem from "../pages/Problems";
import Solve from "../pages/Solve";
import Algorithm from "../pages/Algorithms";
import SignIn from "../components/SignIn";
import SignUp from "../components/SignUp";

const Router = () => {
  return (
    <BrowserRouter>
      <Layout>
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/problem" element={<Problem />} />
          <Route path="/solve" element={<Solve />} />
          <Route path="/algorithm" element={<Algorithm />} />        
          <Route path="/signin" element={<SignIn />} />
          <Route path="/signup" element={<SignUp />} />
        </Routes>
      </Layout>
    </BrowserRouter>
  )
}

export default Router;